#pragma once
#include <SFML/Graphics.hpp>
#include <complex>



class MandelbrotExplorer {
// class variables
private:
    int size;
    int maxiter;
    int checkperiod;
    long double zoomscale;
    std::complex<long double> lc, rc;

    sf::RenderWindow* window;
    sf::Event windowevent;
    sf::Uint8* pixels;
    sf::Texture texture;
    sf::Sprite fractal;


// methods
public:
    // constructor
    MandelbrotExplorer(int size_in=1024, long double zoomscale_in=0.75, int maxiter_in=256,
                       int checkperiod_in=32);

    // life-cycle of the explorer
    void start();

private:
    // renders a single frame (a complete Mandelbrot Fractal)
    void draw();

    // handles zooming
    void zoom(bool zoomin);

    // returns the color of a pixel in the fractal
    sf::Color get_mb_pixel(const std::complex<long double> c);

    // checks if the given point is in a cardoid/bulb
    static bool in_cardoid(const std::complex<long double> c);
};