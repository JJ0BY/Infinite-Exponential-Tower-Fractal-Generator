#ifndef brent_h    // To make sure you don't declare the function more than once by including the header multiple times.
#define brent_h 

//include modules 
#include <iostream>
#include <fstream>
#include <complex>
#include <cmath>
#include <gmp.h>
#include <mpfr.h>
#include <chrono>
#include <omp.h>
#include <string>
#include <limits>

//Function to go to specific line in a text file (DO NOT USE)
std::fstream& GotoLine(std::fstream& file, unsigned int num){
    file.seekg(std::ios::beg);
    for(int i=0; i < num - 1; ++i){
        file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    }
    return file;
}

using namespace std;
using namespace std::chrono;

//a = real part and b = imaginary part

//brent cycle algorithim 
int FindCycle(double a, double b){ 

    //return 0 if value not in image of -W(-xe^-x)=x 
    if ((b == 0) and (a > 1)){

        return 0; 

     }
    
    //return 0 if value not in image of -W(-xe^-x)=x 
    if (a >= b/tan(b)){

        return 0; 
    }

    //Intialize variables 
    int l1 = 0, i = 0, l0 = 200, multiple = 1, rate = 0; 

    double error = 1;

    double const minErr = pow(10, -30);  

    int const maxCycleLen = l0;

    double real = exp(-a)*(a*cos(b)+b*sin(b));

    double imag = exp(-a)*(b*cos(b)-a*sin(b));

    double const Ox = atan2(imag, real); 

    double const rx = sqrt(pow(real, 2)+pow(imag, 2)); 

    double phr = 0, hr = 0, tr = 0, phO = 0, hO = 0, tO = 0, O, R; 

    //While error is too big do the following 
    while (error > minErr) {

        //If the iteration lasts too long return 0 
        if (i >= 20000){

            return 0; 
        }

        //If the cycle is equal to multiple then do the following 
        if ((l1 >= multiple) and (tr != hr) and (tO != hO)){

            tr = hr; 

            tO = hO;

            multiple *= 2; 

            l1 = 0; 

        }

        //Find the next iteration 
        O = Ox+phO;

        R = phr*rx; 

        hr = exp(R*cos(O));

        hO = R*sin(O);

        //if hr is infinity return maxCycleLen which will be represented as Overflow 
        if (isinf(hr) == 1){
            

            return maxCycleLen; 
        }

        //calculate absolute percentage error of r and O 
        error = pow(hr,2) + pow(tr, 2) - 2*hr*tr*cos(hO-tO);

        //if error is not calculatable, then set error = 1 
        if ((isinf(error) == 1) or (isnan(error) == 1)){

            error = 1; 
        }

        //change the corresponding variables 
        phr = hr; 

        phO = hO; 

        i++; 

        l1++; 

        //Find the lowest period length 
        if(error < pow(10, (-5-rate))){

            if((l0 > l1) and (l1 != 0)){
                l0 = l1;
                rate++;
            }
        }
    }

    //return the period length 
    return l0; 

}

//A faster version of FindCycle but is using an improper error function (but produces an almost identitcal image much faster)
int fastFindCycle(double a, double b){

    //fast return rectangele of period 2 
    if ((abs(b) < 0.6) and (a < -1.3)){

        return 2; 
    }
    
    //fast return circle period 1 
    if (sqrt((pow(a, 2)+pow(b,2))) <= 1){

        return 1; 
    }
    
    //fast return 0 if value not in image of -W(-xe^-x)=x 
    if (a >= b/tan(b)){

        return 0; 
    }

    //intialize variables 
    int l1 = 0, i = 0, l0 = 200, multiple = 1, rate = 0, l; 

    double error = 1;

    double const minErr = pow(10, -30), doubPi2 = 2*M_PI;  

    int const maxCycleLen = l0;

    double real = exp(-a)*(a*cos(b)+b*sin(b));

    double imag = exp(-a)*(b*cos(b)-a*sin(b));

    double const Ox = atan2(imag, real); 

    double const rx = sqrt(pow(real, 2)+pow(imag, 2)); 

    double phr =0, hr=0, tr=0, phO=0, hO=0, tO=0, O, R; 

    //while error is too big do the follwoing 
    while (error > minErr) {

        //if the iteration takes too long return 0 
        if (i >= 20000){

            return 0; 
        }

        //If the cycle is equal to multiple then do the following 
        if (l1 >= multiple) {

            tr = hr; 

            tO = hO;

            multiple *= 2; 

            l1 = 0; 

        }

        //Find the next iteration  

        O = Ox+phO;

        R = phr*rx; 

        hr = exp(R*cos(O));

        hO = R*sin(O);

        //if hr is infinity return maxCycleLen which will be represented as Overflow 
        if (isinf(hr) == 1){
            
            return maxCycleLen; 

        }

        //calculate absolute error percentage of previous and current iteration 
        error = abs(tr - hr) + abs(hO-tO);

        //error = pow(tr, 2)+pow(hr, 2)-2*tr*hr*cos(tO-hO); 

        //if error is not calculatable, set error to 1 
        if ((isinf(error) == 1) or (isnan(error) == 1)){

            error = 1; 
        }

        //cout << hr << " e2 " << error << "\n"; 

        //reinatilize the variables 
        phr = hr; 

        phO = hO; 

        i++; 

        l1++; 

        //Find the lowest period length greater than 0 
        if(error < pow(10, (-5-rate))){

            if((l0 > l1) and (l1 != 0)){
                l0 = l1;
                rate++;

            }
        }
    }

    //return the period length 
    return l0; 

}

//deepbrent function to look one iteration into the overflow region. 
//This function is not be used on its own 
int deepFindCycle(double hr, double hO, double rx, double Ox, int bits){

    //intialize variables 
    double hO2, angle; 

    int l = 0, sign;                      

    double const doubPi2 = 2*M_PI; 

    //find if next iteration is 0 
    hO2 = hr*rx*sin(fmod(hO+Ox, doubPi2));

    angle = cos(fmod(hO2+Ox, doubPi2)); 

    if (angle < 0){

        return 1; 
        
    }

    //return 0; 

    //intialize mpfr variables 
    mpfr_t phr, phO, chr, chO, r, O, temp, Pi2;  

    mpfr_rnd_t RoundingStyle = MPFR_RNDN; 

    mpfr_init2(phr, bits); 
    mpfr_init2(chr, bits); 
    mpfr_init2(r, bits); 
    mpfr_init2(temp, bits); 
    mpfr_init2(phO, bits); 
    mpfr_init2(chO, bits); 
    mpfr_init2(O, bits); 
    mpfr_init2(Pi2, bits); 

    mpfr_set_d(chr, hr, RoundingStyle); 
    mpfr_set_d(phO, hO, RoundingStyle);
    mpfr_set_d(chO, hO2, RoundingStyle);
    mpfr_set_d(r, rx, RoundingStyle); 
    mpfr_set_d(O, Ox, RoundingStyle);

    mpfr_const_pi (Pi2, RoundingStyle);
    mpfr_mul_d(Pi2, Pi2, 2, RoundingStyle); 

    while(true){

        //Calculate absolute value of next iteration using rh = e**(prh*rx*cos(pOh+Ox))
        mpfr_add(temp, phO, O, RoundingStyle);
        mpfr_remainder(temp, temp, Pi2, RoundingStyle);
        mpfr_cos(temp, temp, RoundingStyle);

        mpfr_mul(temp, temp, r, RoundingStyle);

        mpfr_mul(temp, temp, chr, RoundingStyle);

        mpfr_exp(chr, temp, RoundingStyle); 


        //Check if number is too big (rounding error causes it to be an integer and it is almost impossible for the result to be an integer)
        mpfr_floor(temp, chr); 

        mpfr_sub(temp, chr, temp, RoundingStyle); 

        if(mpfr_sgn(temp) == 0){
            l = 0; 
            break;
        }

        //Calculate phase of next iteration using Oh = rh*rx*(sin(pOh+Ox))

        

        mpfr_add(temp, chO, O, RoundingStyle);
        mpfr_remainder(temp, temp, Pi2, RoundingStyle);
        mpfr_sin(temp, temp, RoundingStyle);

        mpfr_mul(temp, temp, r, RoundingStyle);

        mpfr_swap(phO, chO); 

        mpfr_mul(chO, temp, chr, RoundingStyle);

        l++; 

        //Calculate whether angle is negative resulting in next rh = 0 using angle = cos(Oh + Ox)
        mpfr_add(temp, chO, O, RoundingStyle);
        mpfr_remainder(temp, temp, Pi2, RoundingStyle);

        mpfr_cos(temp, temp, RoundingStyle);

        sign = mpfr_sgn(temp); 

        if (sign < 0){
            l++;

            break; 
        }
        //sign is only 0 if cos(inf) 
        else if (sign == 0){
            
            l = 0; 

            break; 
        }

    }

    //clear the cache to free up memory 
    mpfr_clear(phr); 
    mpfr_clear(chr); 
    mpfr_clear(r); 
    mpfr_clear(temp); 
    mpfr_clear(phO); 
    mpfr_clear(chO); 
    mpfr_clear(O); 
    mpfr_clear(Pi2); 

    mpfr_free_cache ();

    //return period length 
    return l; 
}

//brent cycle algorithim with the deepFindCycle function (a = real part and b = imaginary part)
int FindCycle2(double a, double b, int bits){ 

    //return 0 if value not in image of -W(-xe^-x)=x 
    if ((b == 0) and (a > 1)){

        return 0; 

     }
    
    //return 0 if value not in image of -W(-xe^-x)=x 
    if (a >= b/tan(b)){

        return 0; 
    }

    //intialize variables 
    int l1 = 0, i = 0, l0 = 200, multiple = 1, rate = 0, l; 

    double error = 1;

    double const minErr = pow(10, -20);  

    int const maxCycleLen = l0;

    double real = exp(-a)*(a*cos(b)+b*sin(b));

    double imag = exp(-a)*(b*cos(b)-a*sin(b));

    double const Ox = atan2(imag, real); 

    double const rx = sqrt(pow(real, 2)+pow(imag, 2)); 

    double phr =0, hr=0, tr=0, phO=0, hO=0, tO=0, O, R; 

    //while error is too big do the follwoing 
    while (error > minErr) {

        //if the iteration takes too long return 0 
        if (i >= 20000){

            return 0; 
        }

        //If the cycle is equal to multiple then do the following 
        if ((l1 >= multiple) and (tr != hr) and (tO != hO)){

            tr = hr; 

            tO = hO;

            multiple *= 2; 

            l1 = 0; 

        }

        //Find the next iteration  

        O = Ox+phO;

        R = phr*rx; 

        hr = exp(R*cos(O));

        hO = R*sin(O);

        //if hr is infinity return maxCycleLen which will be represented as Overflow 
        if (isinf(hr) == 1){
            
            //use the deep brent algorihtim 
            l = deepFindCycle(phr, phO, rx, Ox, bits); 
            
            // if l is greater than 0 do the following, otherwise return maxcCycleLen which will be represented as Overflow 
            if (l > 0){
                
                hr = 0; 
                hO = 0; 

                l1+=l; 
 
            }
            else{
                return maxCycleLen; 
            }

        }

        //calculate absolute error percentage of previous and current iteration 
        error = pow(hr,2) + pow(tr, 2) - 2*hr*tr*cos(hO-tO);

        //error = abs((tr-hr)/(hr+pow(10, -10))) + abs((tO-hO)/(hO+pow(10, -10))); 

        //if error is not calculatable, set error to 1 
        if ((isinf(error) == 1) or (isnan(error) == 1)){

            error = 1; 
        }

        //reinatilize the variables 
        phr = hr; 

        phO = hO; 

        i++; 

        l1++; 

        //Find the lowest period length greater than 0 
        if(error < pow(10, (-5-rate))){

            if((l0 > l1) and (l1 != 0)){
                l0 = l1;
                rate++;

            }
        }
    }

    //return the period length 
    return l0; 

}

//A faster version but is using an improper error function and after making some assumptions on the image (but produces an almost identitical image)
int fastFindCycle2(double a, double b, int bits){

    //fast return rectangele of period 2 
    if ((abs(b) < 0.6) and (a < -1.3)){

        return 2; 
    }
    
    //fast return circle period 1 
    if (sqrt((pow(a, 2)+pow(b,2))) <= 1){

        return 1; 
    }
    
    //fast return 0 if value not in image of -W(-xe^-x)=x 
    if (a >= b/tan(b)){

        return 0; 
    }

    //intialize variables 
    int l1 = 0, i = 0, l0 = 200, multiple = 1, rate = 0, l; 

    double error = 1;

    double const minErr = pow(10, -30), doubPi2 = 2*M_PI;  

    int const maxCycleLen = l0;

    double real = exp(-a)*(a*cos(b)+b*sin(b));

    double imag = exp(-a)*(b*cos(b)-a*sin(b));

    double const Ox = atan2(imag, real); 

    double const rx = sqrt(pow(real, 2)+pow(imag, 2)); 

    double phr =0, hr=0, tr=0, phO=0, hO=0, tO=0, O, R; 

    //while error is too big do the follwoing 
    while (error > minErr) {

        //if the iteration takes too long return 0 
        if (i >= 20000){

            return 0; 
        }

        //If the cycle is equal to multiple then do the following 
        if (l1 >= multiple) {

            tr = hr; 

            tO = hO;

            multiple *= 2; 

            l1 = 0; 

        }

        //Find the next iteration  

        O = Ox+phO;

        R = phr*rx; 

        hr = exp(R*cos(O));

        hO = R*sin(O);

        //if hr is infinity return maxCycleLen which will be represented as Overflow 
        if (isinf(hr) == 1){
            
            //use the deep brent algorihtim 
            l = deepFindCycle(phr, phO, rx, Ox, bits); 
            
            // if l is greater than 0 do the following, otherwise return maxcCycleLen which will be represented as Overflow 
            if (l > 0){
                
                hr = 0; 
                hO = 0; 

                l1+=l; 
 
            }
            else{
                return maxCycleLen; 
            }

        }

        //calculate absolute error percentage of previous and current iteration 
        error = abs(tr - hr) + abs(hO-tO);

        //error = pow(tr, 2)+pow(hr, 2)-2*tr*hr*cos(tO-hO); 

        //if error is not calculatable, set error to 1 
        if ((isinf(error) == 1) or (isnan(error) == 1)){

            error = 1; 
        }

        //cout << hr << " e2 " << error << "\n"; 

        //reinatilize the variables 
        phr = hr; 

        phO = hO; 

        i++; 

        l1++; 

        //Find the lowest period length greater than 0 
        if(error < pow(10, (-5-rate))){

            if((l0 > l1) and (l1 != 0)){
                l0 = l1;
                rate++;

            }
        }
    }

    //return the period length 
    return l0; 

}

//This function will make the fractal image array using FindCycle algorithim 
int fractalMake(double rmin, double rmax, double imin, double imax, int qualityPerUnit, string output, int functionMode = 0, int bits = 1000){
    
    auto start = high_resolution_clock::now();

    std :: cout << "Program started\n"; 

    int quality = 2*qualityPerUnit; 

    double rquality = (rmax-rmin)*quality, iquality = (imax-imin)*quality; 

    double rspace = (rmax-rmin)/(rquality-1), ispace = (imax-imin)/(iquality-1);

    if (rquality*iquality > 50000000){

        std :: cout << "You are trying to generate data for an image with over 50 million pixels. This will take a lot of time and might result in a crash. Press 1 to continue.\n";

        cin >> quality; 

        if (quality != 1){
            return 0; 
        }

    }

    std :: cout << "\nYou are creating a " << rquality << " X " << iquality << " image data file.\n"; 

    ofstream out("Data/" + output + ".csv");

    switch (functionMode){

        case 0:

            for (int i = 0;i < iquality;i++){   

                std :: cout << "\r" << i+1 << " of " << iquality;

                for (int j = 0;j < rquality;j++){   

                    out << FindCycle(rmin+j*rspace, imax-i*ispace)<<",";
                }

                out << endl; 
            }

            break; 

        case 1: 

            for (int i = 0;i < iquality;i++){   

                std :: cout << "\r" << i+1 << " of " << iquality;

                for (int j = 0;j < rquality;j++){   

                    out << fastFindCycle(rmin+j*rspace, imax-i*ispace)<<",";
                }

                out << endl; 
            }
            break; 

        case 2: 

            for (int i = 0;i < iquality;i++){   

                std :: cout << "\r" << i+1 << " of " << iquality;

                for (int j = 0;j < rquality;j++){   

                    out << FindCycle2(rmin+j*rspace, imax-i*ispace, bits)<<",";
                }

                out << endl; 
            }
            break; 

        case 3: 

            for (int i = 0;i < iquality;i++){   

                std :: cout << "\r" << i+1 << " of " << iquality;

                for (int j = 0;j < rquality;j++){   

                    out << fastFindCycle2(rmin+j*rspace, imax-i*ispace, bits)<<",";
                }

                out << endl; 
            }
            break; 

        default: 

            std :: cout << " Improper function used. Values can only be integers in [0, 3]"; 
            return 0; 

    }

    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<seconds>(stop - start);

    std :: cout << "\nTime taken by function: "<< duration.count() << " seconds" << endl;

    std :: cout << "Efficiency: " << (rquality*iquality)/(int) duration.count() << " entries/second"<< endl;

    return 0;
}

#endif