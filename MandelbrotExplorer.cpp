#include "MandelbrotExplorer.h"
#include <SFML/Graphics.hpp>
#include <complex>
#include <iostream>
#include <chrono>




// constructor
MandelbrotExplorer::MandelbrotExplorer(int size_in, long double zoomscale_in, int maxiter_in,
                                       int checkperiod_in) {
    size = size_in;
    zoomscale = zoomscale_in;
    maxiter = maxiter_in;
    checkperiod = checkperiod_in;
    lc = {-2.5, -2};
    rc = {1.5, 2};
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
        while (window->pollEvent(windowevent)) {
            // close window
            if (windowevent.type == sf::Event::Closed)
                window->close();
            
            // zoom
            else if (windowevent.type==sf::Event::MouseWheelScrolled && !redraw) {
                redraw = true;
                zoom(windowevent.mouseWheelScroll.delta > 0);
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
    long double dreal = (rc.real() - lc.real()) / size;
    long double dimag = (rc.imag() - lc.imag()) / size;

    for (int y=0; y<size; y++) {
        for (int x=0; x<size; x++) {
            long double real = lc.real() + dreal*x;
            long double imag = lc.imag() + dimag*y;
            sf::Color col = get_mb_pixel({real, imag});
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
    printf("Rendered in %.3fs\n", elapsed.count()/1000.0);
}


// handles zooming
void MandelbrotExplorer::zoom(bool zoomin) {}


// returns the color of a pixel in the fractal (static)
sf::Color MandelbrotExplorer::get_mb_pixel(const std::complex<long double> c) {
    // pre-checks
    if (in_cardoid(c))
        return sf::Color(0, 0, 0);

    std::complex<long double> z = c;        // iterative z
    std::complex<long double> z_old = z;    // old value to check for periodicity
    double z_l2 = std::norm(z);             // l2 norm of z
    int itercount = 0;                      // current iteration count

    while (z_l2 < 4 && itercount < maxiter) {
        z = z*z + c;
        z_l2 = std::norm(z);
        itercount++;

        // periodicity checking
        if (z == z_old)
            return sf::Color(0, 0, 0);

        // update check value for periodicity
        if (itercount%checkperiod == 0)
            z_old = z;
    }

    // inside the set, return black
    if (itercount >= maxiter)
        return sf::Color(0, 0, 0);

    // outside the set, compute color
    //double s = (double)itercount / double(maxiter);
    double s = (itercount + 1 - std::log(std::log(z_l2)) / std::log(2)) / maxiter;

    return sf::Color(s*255, s*255, s*255);
}


// checks if the given point is in a cardoid/bulb
bool MandelbrotExplorer::in_cardoid(const std::complex<long double> c) {
    double x_s = c.real() - 0.25;
    double y_2 = c.imag()*c.imag();
    double q = x_s*x_s + y_2;

    return q * (q + x_s) <= 0.25 * y_2;
}