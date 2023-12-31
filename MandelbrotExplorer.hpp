#include "BS_thread_pool_light.hpp"
#include <SFML/Graphics.hpp>
#include <math.h>



class MandelbrotExplorer {
// class variables
private:
    // mandelbrot
    int size;
    int maxiter;
    int iterincr;
    int checkperiod;
    long double zoomscale;
    long double c0_re, c0_im, c1_re, c1_im;
    BS::thread_pool_light pool;
    // visual
    sf::RenderWindow* window;
    sf::Event event;
    sf::Uint8* pixels;
    sf::Texture texture;
    sf::Sprite fractal;



// methods
public:
    // constructor
    MandelbrotExplorer(const int size_in, const double zoomscale_in, const int maxiter_in,
    const int iterincr_in, const int checkperiod_in) {
        size = size_in;
        zoomscale = zoomscale_in;
        maxiter = maxiter_in;
        iterincr = iterincr_in;
        checkperiod = checkperiod_in;
        c0_re = -2.0;
        c0_im = -1.5;
        c1_re =  1.0;
        c1_im =  1.5;
        pixels = new sf::Uint8[size * size * 4];
        texture.create(size, size);
        fractal.setTexture(texture);
    }


    // life-cycle of the explorer
    void start() {
        window = new sf::RenderWindow(sf::VideoMode(size, size), "Mandelbrot Explorer");
        bool redraw = true;

        // until window closed
        while (window->isOpen()) {

            // events
            while (window->pollEvent(event)) {
                // close window
                if (event.type == sf::Event::Closed)
                    window->close();
                
                // zoom
                else if (event.type==sf::Event::MouseWheelScrolled && !redraw) {
                    redraw = true;
                    zoom(event.mouseWheelScroll.delta > 0);
                }

                else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Add) && !redraw) ||
                        (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !redraw) ||
                        (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !redraw)) {
                    redraw = true;
                    maxiter += iterincr*8;
                    printf("Set maxiter to %d\n", maxiter);
                }
                
                else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract) && !redraw) ||
                        (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !redraw) ||
                        (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !redraw)) {
                    redraw = true;
                    maxiter -= iterincr*8;
                    printf("Set maxiter to %d\n", maxiter);
                }
            }

            // rendering
            if (redraw) {
                draw();
                redraw = false;
            }
        }

        // clean up
        delete window;
        delete pixels;
    }

private:
    // renders a single frame (a complete Mandelbrot Fractal)
    void draw() {
        // timing start
        auto tstart = std::chrono::_V2::high_resolution_clock::now();

        // start drawing
        double dc = (c1_re - c0_re) / size;
        
        for (int y=0; y<size; y++) {
            // multi thread each row
            pool.push_task([this, dc, y] {
                for (int x=0; x<size; x++) {
                    double real = c0_re + dc*x;
                    double imag = c0_im + dc*y;
                    this->set_mb_pixel(4*(size*y + x), real, imag);
                }
            });
        }
        pool.wait_for_tasks();

        window->clear();
        texture.update(pixels);
        window->draw(fractal);
        window->display();

        // timing end
        auto tend = std::chrono::_V2::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(tend - tstart);
        printf("Time: %.3fs,\tmaxiter: %d,\tzoom: 10^%.2f\n",
                elapsed.count()/1000.0, maxiter, std::log10(dc));
    }


    // handles zooming
    void zoom(const bool zoomin) {
        // mouse position ratio
        double mx = (double) event.mouseWheelScroll.x / size;
        double my = (double) event.mouseWheelScroll.y / size;

        // scale
        double sc = c1_re - c0_re;
        double newsc = (zoomin) ? sc*zoomscale / 2 : sc / (2*zoomscale);
        maxiter += (zoomin) ? iterincr/zoomscale : -iterincr/zoomscale;

        // clicked spot
        double creal = c0_re + mx*sc;
        double cimag = c0_im + my*sc;

        // update lc and rc
        c0_re = creal - newsc;
        c0_im = cimag - newsc;
        c1_re = creal + newsc;
        c1_im = cimag + newsc;
    }


    // sets the color of a pixel in the fractal
    void set_mb_pixel(const int p_index, const double cre, const double cim) {
        // pre-checks
        if (in_cardoid(cre, cim)) {
            pixels[p_index+0] = 0;    // r
            pixels[p_index+1] = 0;    // g
            pixels[p_index+2] = 0;    // b
            pixels[p_index+3] = 255;  // a
            return;
        }

        // iterative z
        double zre = cre;
        double zim = cim;
        // for optimization
        double zre_sq = cre*cre;
        double zim_sq = cim*cim;
        // squared l2 norm of z
        double z_l2 = zre*zre + zim*zim;
        // old norm to check for periodicity
        double z_l2_old = z_l2;
        // current iteration count
        int itercount = 0;

        while (z_l2 < 4 && itercount < maxiter) {
            // z = z*z + c
            zim = 2*zre*zim + cim;
            zre = zre_sq - zim_sq + cre;
            zre_sq = zre*zre;
            zim_sq = zim*zim;
            z_l2 = zre*zre + zim*zim;
            itercount++;

            // periodicity checking
            if (z_l2 == z_l2_old) {
                pixels[p_index+0] = 0;    // r
                pixels[p_index+1] = 0;    // g
                pixels[p_index+2] = 0;    // b
                pixels[p_index+3] = 255;  // a
                return;
            }

            // update check value for periodicity
            if (itercount%checkperiod == 0)
                z_l2_old = z_l2;
        }

        // inside the set, return black
        if (itercount >= maxiter) {
            pixels[p_index+0] = 0;    // r
            pixels[p_index+1] = 0;    // g
            pixels[p_index+2] = 0;    // b
            pixels[p_index+3] = 255;  // a
            return;
        }

        // outside the set, compute color (color band repeats 4 times)
        double s = 4*(itercount + 1 - std::log(std::log(z_l2)) / std::log(2)) / maxiter;
        s -= (int)s;

        pixels[p_index+0] = 9*(1.0-s)*std::pow(s,3)*255;             // r
        pixels[p_index+1] = 15*std::pow(1.0-s,2)*std::pow(s,2)*255;  // g
        pixels[p_index+2] = 8.5*std::pow(1.0-s,3)*s*255;             // b
        pixels[p_index+3] = 255;                                     // a
        return;
    }


    // checks if the given point is in a cardoid/bulb
    static bool in_cardoid(const double cre, const double cim) {
        double x_s = cre - 0.25;
        double y_2 = cim*cim;
        double q = x_s*x_s + y_2;

        return q * (q + x_s) <= 0.25 * y_2;
    }
};  // MandelbrotExplorer