#pragma once
#include <SFML/Graphics.hpp>



class MandelbrotExplorer {
// class variables
private:
    int size;
    int maxiter;
    int iterincr;
    int checkperiod;
    long double zoomscale;
    long double c0_re, c0_im, c1_re, c1_im;

    sf::RenderWindow* window;
    sf::Event event;
    sf::Uint8* pixels;
    sf::Texture texture;
    sf::Sprite fractal;


// methods
public:
    // constructor
    MandelbrotExplorer(int size_in, double zoomscale_in, int maxiter_in, int iterincr_in, int checkperiod_in);

    // life-cycle of the explorer
    void start();

private:
    // renders a single frame (a complete Mandelbrot Fractal)
    void draw();

    // handles zooming
    void zoom(bool zoomin);

    // returns the color of a pixel in the fractal
    sf::Color get_mb_pixel(const double cre, const double cim);

    // checks if the given point is in a cardoid/bulb
    static bool in_cardoid(const double cre, const double cim);
};