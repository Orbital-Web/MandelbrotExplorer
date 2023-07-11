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
    MandelbrotExplorer(int size_in, long double zoomscale_in, int maxiter_in,
                       int checkperiod_in);

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