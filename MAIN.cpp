#include "brent.h"

int main(){

    //Settings START
    //increasing quality increases the quality of the picture but slows the function (I have made a 10000 pixel but it took 5-6 hours on an i5 12th gen)
    int halfquality = 2000; 

    //increasing bits allows one to see more into the overflow region but it is recommended to be no more than 20000
    int bits = 100; 

    double rmin = -3; 

    double rmax = 1; 

    double imax = 2; 

    double imin = -2; 

    //Settings END
    
    //Choose which fractal make function to use, the last three functions produce almost the exact same picture

    //this function will make the fractal image array using brent algorithim  without looking one iteration deeper (MATHEMATICALLY DERIVED BUT VERY SLOW)
    //This will result in an image similar to the fractal seen in https://orcca.on.ca/LambertW/
    //fractalMake(rmin, rmax, imin, imax, halfquality, "test_data.csv"); 

    //this function will make the fractal image array using brent algorithim with one more iteration into the overflow region (MATHEMATICALLY DERIVED BUT VERY VERY SLOW)
    //fractalMake2(rmin, rmax, imin, imax, halfquality, "test_data1.csv", bits); 

    //this function will make the fractal image array using brent algorithim with one more iteration into the overflow region (WE ASSUME SOME THINGS BUT FAST)
    //fastFractalMake2(rmin, rmax, imin, imax, halfquality, "test_data2.csv", bits); 

    //this function will make the fractal image array using brent algorithim with one more iteration into the overflow region (WE ASSUME SOME THINGS WITH DIFFERENT ERROR FUNCTION BUT VERY FAST)
    fastFractalMake3(rmin, rmax, imin, imax, halfquality, "test_data3.csv", bits); 

    return 0; 
}

