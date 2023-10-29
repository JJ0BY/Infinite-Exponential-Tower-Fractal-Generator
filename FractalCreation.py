#!/usr/bin/env python
# coding: utf-8

#Import modules 
import matplotlib.pyplot as plt
from matplotlib.colors import ListedColormap

import time as t
import random as r
from colorsys import hls_to_rgb

import numpy as np

import multiprocessing as mp
from Methods import loadObject, uploadObject

#Create Class for Fractal Creation 
class FractalCreation:
    
    #Init function to set variables of the class 
    def __init__(self, rmin, rmax, imin, imax, quality):
        
        #intialize variables
        self.rmin = rmin
        self.rmax = rmax
        self.imin = imin
        self.imax = imax
        self.quality = quality
        
        #Quality control to avoid floating point errors, for example round to 1 instead of 1.000000094
        fraction = min((rmax-rmin)/(quality-1), (imax-imin)/(quality-1)) 
        decimal_places = len(str(fraction).split('.')[1])+1
        
        
        #Get 4000 elements in [rmin, rmax] and [imin, imax]
        X = np.linspace(rmin, rmax, quality).astype(np.longdouble)
        Y = np.linspace(imax, imin, quality).astype(np.longdouble)*1j
        
        #Create and intialize matrix grid
        self.Matrix = np.outer(X, np.ones(X.shape)).T + np.outer(Y, np.ones(Y.shape))
        self.MatrixFlattened = np.round(self.Matrix.reshape(1, quality**2)[0], decimal_places)
        self.Matrix = self.MatrixFlattened.reshape(quality, quality)
        
        self.newMatrix = None 
    
    #getter functions 
    
    #get flattened matrix
    def getMatrixFlattened(self):
        
        return self.MatrixFlattened
    
    #get the new matrix recieved after applying the function on each value of the matrix 
    def getNewMatrix(self):
        
        return self.newMatrix
    
    #Get the unique number of cycles in the new matrix 
    def getUniqueSet(self):
        
        return self.UniSet
    
    #Save the new matrix as a pickle object 
    def saveNewMatrix(self, name=True):
        
        if name:
            name = f'data/FractalMatrix_R[{self.rmin},{self.rmax}]_I[{self.imin},{self.imax}]_Q[{self.quality}]'
        
        uploadObject(self.newMatrix, name)
    
    #Load a pickle object as a new matrix
    def loadNewMatrix(self, name=True):
        
        if name:
            name = f'data/FractalMatrix_R[{self.rmin},{self.rmax}]_I[{self.imin},{self.imax}]_Q[{self.quality}]'
        
        self.newMatrix = loadObject(name)    
        
        return self.newMatrix
    
    #Parallelize applying the function to all values in the matrix 
    def parallelize(self, arr, func, n_cores=4):
        pool = mp.Pool(n_cores)
        print('Running Pool')
        arr2 = pool.map(func, arr)
        pool.close()
        pool.join()
        print('Pool run ended')
        return arr2
    
    #Apply the function on all values in the matrix 
    def funcMatrix(self, func, n_cores=8):
        t0 = t.time()

        b = self.parallelize(self.MatrixFlattened, func, n_cores)
        
        t1 = t.time()

        #Print how long the program took to create the matrix and the number of unique elements in the matrix 
        print(f'{((t1-t0)/60):.3f} Minutes')

        self.newMatrix = np.array(b).reshape(self.quality, self.quality)
        
    #Get 'n' unique colors 
    def get_distinct_colors(self, n):

        colors = []

        for i in np.arange(0., 360., 360. / n):
            h = i / 360.
            l = (50 + np.random.rand() * 10) / 100.
            s = (90 + np.random.rand() * 10) / 100.
            colors.append(hls_to_rgb(h, l, s))

        r.shuffle(colors)
        
        return colors
    
    #Plot and save the matrix as a heatmap and save it as an image
    def PlotSave(self, Matrix='', name='', rsep=True, isep=True, loadColor=True, DPI=500):
        
        #Label the ticks for the x and y axis 
        if rsep:
            
            rsep = (self.rmax-self.rmin)/10
        
        if isep:
            
            isep = (self.imax-self.imin)/10
            
        if type(Matrix) != np.ndarray:
            
            Matrix = self.newMatrix
        
        x1 = np.round(np.arange(self.rmin, self.rmax+rsep*0.1, rsep), 5)
        x0 = np.arange(0, self.quality+0.1, int(self.quality/(len(x1)-1)))
        
        y1 = np.round(np.arange(self.imax, self.imin-isep*0.1, -isep, ), 5)
        y0 = np.arange(0, self.quality+0.1, int(self.quality/(len(y1)-1)))
        
        #Get the subplot figure 
        fig, ax = plt.subplots(1, 1, figsize=(20, 20))
        
        #Load the colorObject or upload the colorObject as a pickle object 
        if loadColor: 
            ColorObjs = loadObject('ColorMaps')
        else:
            
            UniSet = np.unique(Matrix.reshape(1, self.quality**2))

            ColorObjs = self.get_distinct_colors(len(UniSet))

            uploadObject(ColorObjs, 'ColorMaps')
        
        #Set overflow to black pixel 
        ColorObjs.insert(len(ColorObjs), (0,0,0))

        #choose the color scheme 
        cmap = ListedColormap(ColorObjs)
        
        #Set np.nan values (values that took too long to converge) to white 
        cmap.set_bad(color="white")

        #Graph settings
        #Plot heatmap of the matrix graph 
        ax.imshow(Matrix, interpolation='none', cmap=cmap)

        #Fix axes settings
        ax.xaxis.set_ticks(x0)
        ax.xaxis.set_ticklabels(x1)

        ax.yaxis.set_ticks(y0)
        ax.yaxis.set_ticklabels(y1)
        ax.set_xlabel('Real')
        ax.set_ylabel('Imag')

        ax.set_title('Cycles')
        
        #plot the graph 
        plt.show()
        
        #Save the image as a heatmap 
        if name:
            fig.savefig(f'Images/{name}.png', dpi = DPI)

        


