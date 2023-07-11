# MandelbrotExplorer
A Mandelbrot Fractal Explorer written in C++ and SFML.

Here are the list of optimizations and improvements planned/already implemented, taken mostly from [this Wikepedia article](https://www.wikiwand.com/en/Plotting_algorithms_for_the_Mandelbrot_set):
- [x] Normalized iteration count
- [ ] LCH coloring
- [x] Cardoid checking
- [x] Periodicity checking
- [ ] Multithreading


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