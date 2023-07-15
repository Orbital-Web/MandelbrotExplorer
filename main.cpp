#include "MandelbrotExplorer.hpp"
#include <iostream>
#include <string.h>


int main(int argc, char** argv) {
    int size = 1024;
    double zoomscale = 0.5;
    int maxiter = 128;
    int iterincr = 32;
    int checkperiod = 32;

    // parse argument
    int i=1;
    while (i < argc) {
        if (!strcmp(argv[i], "-size"))
            size = std::atoi(argv[i+1]);

        else if (!strcmp(argv[i], "-zoom"))
            zoomscale = std::atof(argv[i+1]);

        else if (!strcmp(argv[i], "-maxiter"))
            maxiter = std::atoi(argv[i+1]);
        
        else if (!strcmp(argv[i], "-iterincr"))
            checkperiod = std::atoi(argv[i+1]);

        else if (!strcmp(argv[i], "-period"))
            checkperiod = std::atoi(argv[i+1]);

        else {
            std::cout << "Usage: main.exe [options]\n\n"
                      << "Options:\n"
                      << "  --help\tDisplay this information.\n"
                      << "  -size n\tSet window size to n by n\n"
                      << "  -zoom f\tSet zoom scale to f\n"
                      << "  -maxiter n\tSet maximum iteration count to n\n"
                      << "  -iterincr n\tSet value maxiter increases by each zoom\n"
                      << "  -period n\tSet period checking length to n\n";
            return -1;
        }
        i += 2;
    }

    printf("Starting Explorer with size: %d, zoomscale: %.3f, maxiter: %d, iterincr: %d, checkperiod: %d\n",
           size, zoomscale, maxiter, iterincr, checkperiod);
    MandelbrotExplorer explorer(size, zoomscale, maxiter, iterincr, checkperiod);
    explorer.start();
    return 0;
}