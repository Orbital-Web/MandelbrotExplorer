#pragma once
#include "BS_thread_pool_light.hpp"
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
    BS::thread_pool_light pool;

    sf::RenderWindow* window;
    sf::Event event;
    sf::Uint8* pixels;
    sf::Texture texture;
    sf::Sprite fractal;


// methods
public:
    // constructor
    MandelbrotExplorer(const int size_in, const double zoomscale_in, const int maxiter_in,
    const int iterincr_in, const int checkperiod_in);

    // life-cycle of the explorer
    void start();

private:
    // renders a single frame (a complete Mandelbrot Fractal)
    void draw();

    // handles zooming
    void zoom(const bool zoomin);

    // sets the color of a pixel in the fractal
    void set_mb_pixel(const int p_index, const double cre, const double cim);

    // checks if the given point is in a cardoid/bulb
    static bool in_cardoid(const double cre, const double cim);
};