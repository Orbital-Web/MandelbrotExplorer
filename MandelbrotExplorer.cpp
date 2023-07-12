#include "MandelbrotExplorer.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include <math.h>




// constructor
MandelbrotExplorer::MandelbrotExplorer(int size_in, double zoomscale_in, int maxiter_in,
int iterincr_in, int checkperiod_in) {
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
void MandelbrotExplorer::start() {
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


// renders a single frame (a complete Mandelbrot Fractal)
void MandelbrotExplorer::draw() {
    // timing start
    auto tstart = std::chrono::_V2::high_resolution_clock::now();

    // start drawing
    double dc = (c1_re - c0_re) / size;
    
    for (int y=0; y<size; y++) {
        for (int x=0; x<size; x++) {
            double real = c0_re + dc*x;
            double imag = c0_im + dc*y;
            sf::Color col = get_mb_pixel(real, imag);
            pixels[4*(size*y + x) + 0] = col.r;
            pixels[4*(size*y + x) + 1] = col.g;
            pixels[4*(size*y + x) + 2] = col.b;
            pixels[4*(size*y + x) + 3] = 255;
        }
    }

    window->clear();
    texture.update(pixels);
    window->draw(fractal);
    window->display();

    // timing end
    auto tend = std::chrono::_V2::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(tend - tstart);
    printf("Time: %.3fs,\tmaxiter: %d,\tzoom: 10^%.2f\n",
            elapsed.count()/1000.0, maxiter, (double)std::log10(dc*size));
}


// handles zooming
void MandelbrotExplorer::zoom(bool zoomin) {
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


// returns the color of a pixel in the fractal (static)
sf::Color MandelbrotExplorer::get_mb_pixel(const double cre, const double cim) {
    // pre-checks
    if (in_cardoid(cre, cim))
        return sf::Color(0, 0, 0);

    // iterative z
    double zre = cre;
    double zim = cim;
    // for optimization
    double zre_sq = cre*cre;
    double zim_sq = cim*cim;
    // l2 norm of z
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
        if (z_l2 == z_l2_old)
            return sf::Color(0, 0, 0);

        // update check value for periodicity
        if (itercount%checkperiod == 0)
            z_l2_old = z_l2;
    }

    // inside the set, return black
    if (itercount >= maxiter)
        return sf::Color(0, 0, 0);

    // outside the set, compute color (color band repeats 4 times)
    double s = 4*(itercount + 1 - std::log(std::log(z_l2)) / std::log(2)) / maxiter;
    s -= (int)s;
    //return sf::Color(255*s, 255*s, 255*s);
    return sf::Color(
        9*(1.0-s)*std::pow(s,3)*255,
        15*std::pow(1.0-s,2)*std::pow(s,2)*255,
        8.5*std::pow(1.0-s,3)*s*255);
}


// checks if the given point is in a cardoid/bulb
bool MandelbrotExplorer::in_cardoid(const double cre, const double cim) {
    double x_s = cre - 0.25;
    double y_2 = cim*cim;
    double q = x_s*x_s + y_2;

    return q * (q + x_s) <= 0.25 * y_2;
}