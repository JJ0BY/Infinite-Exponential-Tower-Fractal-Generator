#include "brent.h"

int main(){

    //Settings START

    //Number of pixels in each unit. For example, there are 4 units on the real axis using the default option, hence (4 * 1000 * 2 = 8000) pixels on the x-direction. I recommend that the number of pixels in the x and y direction should be less than 10000, 
    //so if you are calculating a lot of units, the qualityperUnit number should be smaller.
    int qualityPerUnit = 1000; 

    //increasing bits allows one to see more into the overflow region but it is recommended to be no more than 20000
    int bits = 100; 

    //Choose which type of function to use to generate the fractal. The values can only be in [0, 3]

    /*
    | 0 | Will make the data in a mathematically derived way but is slow. This will result in an image similar to the fractal seen in https://orcca.on.ca/LambertW/. |
    | 1 | Will make the data using some assumptions after looking at the fractal image we produced from `FunctionMode = 0` and it uses an improper error function but it is very faster. |
    | 2 | Will make the data in a mathematically derived way and allow us to look one cycle deeper into any overflow regions but is slower than `fractalMake`. This will result in an image similar to the fractal seen in the example at the very bottom. |
    | 3 | Will make the data using some assumptions after looking at the fractal image we produced from `FunctionMode = 2` and it uses an improper error function but it is faster. |
    */

    int functionMode = 1; 

    double rmin = -3; 

    double rmax = 1; 

    double imax = 2; 

    double imin = -2; 

    string fileName = "test_data1_full"; 

    //Settings END

    fractalMake(rmin, rmax, imin, imax, qualityPerUnit, fileName, functionMode, bits); 

    cout << "\n Enter any button to quit the program.\n"; 

    cin.get(); 

    return 0; 
}

