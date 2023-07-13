# MandelbrotExplorer
A Mandelbrot Fractal Explorer written in C++ and [SFML](https://www.sfml-dev.org/), and [Thread Pools](https://github.com/bshoshany/thread-pool).
It can view up to a zoom (pixel width) of `10^-16` before encountering floating point errors (although this could be increased using `long double` at the cost of speed and inconsistency across compilers).


## Features
- Customizable size, max iteration count, iteration increment on zoom, scale factor when zooming, and period checking frequency. These can be set with command-line arguments. For more details, run `main.exe --help`. 
- Zoom in/out with the mouse scroll wheel. It will place the region the mouse was over when scrolling at the center of the window after the zoom.
- Increase/decrease maxiter with any of the `arrow` keys or with the numpad `+` and `-` keys.

Here are the list of optimizations and improvements implemented, taken mostly from [this Wikepedia article](https://www.wikiwand.com/en/Plotting_algorithms_for_the_Mandelbrot_set):
- [x] Normalized iteration count
- [x] Continuous coloring
- [x] Cardoid checking
- [x] Periodicity checking
- [x] Multithreading


## Getting Started (Windows)
1. Clone the repository
2. Install g++
3. Download SFML-2.6.0 (GCC MinGW) from https://www.sfml-dev.org/download/sfml/2.6.0/ and add it to the root directory
4. Compile and run `main.exe` with the following commands
    ```
    g++ -c main.cpp MandelbrotExplorer.cpp -ISFML-2.6.0/include -DSFML_STATIC
    g++ -o main main.o MandelbrotExplorer.o -LSFML-2.6.0/lib -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lwinmm -lgdi32
    del main.o MandelbrotExplorer.o
    main.exe
    ```