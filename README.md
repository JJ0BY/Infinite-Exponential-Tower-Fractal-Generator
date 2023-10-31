# Infinite-Exponential-Tower-Fractal-Generator
<h1> Welcome to a Western University USRI project! Here are the steps to create your own fractal picture similar to the picture found on https://rcorless.github.io/Two-cycles.html </h1>

- &nbsp; Go to MAIN.cpp, change the settings to what you want and build the MAIN.exe file using the following command "g++  MAIN.cpp -o MAIN -lmpfr -lgmp"
- &nbsp; Run the MAIN.exe file, this will take some time depending on your computer and the settings you have picked. 
- &nbsp; Once MAIN.exe file is finished running, go to CompletedTest2.ipynb and set the same settings as you have in MAIN.cpp and run everything.
- &nbsp; You can see the image on jupyter but if your picture has a quality greater than 2000, it is better to directly see it by downloading the picture you made located in Images/ and seeing it on your computer. 

## Iterated Exponential Fractal

The iteration <img src="https://rawgit.com/steventhornton/IteratedExponential/master/svgs/fe28dcd3c561e677c1867ff53257e2ac.svg" align=middle width=78.29226pt height=23.784120000000012pt/> where <img src="https://rawgit.com/steventhornton/IteratedExponential/master/svgs/8aaad2aa848320318ffb11a51bdfe36f.svg" align=middle width=50.2794pt height=23.026680000000013pt/> can either diverge or converge to a cycle. This repository provides the code for plotting these cycle lengths in the complex plane. Different colors are used to represent the different cycle lengths. Black typically indicates the divergence, overflow or underflow in the iteration while white represents that the program took too long too find the cycle length of that given point.

#### C++ Requirements
- The mpfr and gmp modules must be installed.
- The brent.h file must be in the same folder for it to properly work except for LICENSE and README.md. 

#### Python Requirements
- The sys, csv, numpy, tracemalloc, time, pickle, matplotlib, random, colorsys and multiprocessing modules must be installed.
- All the python and ipython files in this repository must be in the same folder, for it to properly work except for LICENSE and README.md. 

# Using the C++ program 

There are multiple main functions used for producing the images in MAIN.cpp 
It is called as
```matlab
iteratedExponential(workingDir, options);
```
The `Images` folder is the location where images will be saved while the data for these images are located in the `Data` folder. 

BE VERY CAREFUL OF THE DATA BEING OVERWRITTEN. 

The colors are randomly selected, however if you wish to change the color scheme, go to CompletedTest2.ipynb and set `loadColor = False`. This will make a random new color scheme and replace the old color scheme on the file ColorMaps. I recommend saving the ColorMaps you like on a different folder and feel free to share any good color schemes on the Issues tab. 

## Options

| Option Name | Default | Details |
| ----------- | ------- | ------- |
| `rmin` | -3 | The real boundary on the left. |
| `rmax` | 1 | The real boundary on the right. It is recommended for this value to be 1 or less than 1 otherwise you will just get white pixels due to the image of the function. |
| `imin` | -2 | The imaginary boundary at the bottom. |
| `imax` | 2 | The imaginary boundary at the top. |
| `bits` | 100 | The number of bits associated to the complex numbers within the program once they have an overflow issue. The bigger the number, the slower the program and I recommend that the bits value should be less than 10000.|
| `halfquality` | 2000. | Half the number of pixels in the image. The bigger the number, the slower the program and I recommend that the halfquality value should be less than 10000.|

# Example
```C++

int halfquality = 2000; 
int bits = 100; 
double rmin = -2; 
double rmax = 1; 
double imax = 3; 
double imin = 0;

fastFractalMake3(rmin, rmax, imin, imax, halfquality, "test_data4.csv", bits);

cout << "\n\n press any button to quit the program."; 

cin.get(); 

return 0; 
```

Output image:

<p align="center">
    <img alt="Iterated Exponential Fractal." src="[https://s3.amazonaws.com/stevenethornton.github/IterExpFractal_800.png](https://github.com/JJ0BY/Infinite-Exponential-Tower-Fractal-Generator/blob/main/Images/ImagesCompact/imageTesting_NoOverflow_C++_Quality_3000_Bits_20000.png?raw=true)https://github.com/JJ0BY/Infinite-Exponential-Tower-Fractal-Generator/blob/main/Images/ImagesCompact/imageTesting_NoOverflow_C++_Quality_3000_Bits_20000.png?raw=true"/>
</p>

