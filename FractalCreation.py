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
    def __init__(self, rmin, rmax, imin, imax, qualityPerUnit):
        
        #intialize variables
        self.rmin = rmin
        self.rmax = rmax
        self.imin = imin
        self.imax = imax
        self.qualityPerUnit = qualityPerUnit
    
    #Get the unique number of cycles in the new matrix 
    def getUniqueSet(self):
        
        return self.UniSet
        
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

        rquality = (self.rmax-self.rmin)*2*self.qualityPerUnit
        iquality = (self.imax-self.imin)*2*self.qualityPerUnit
        
        #print(rquality, iquality)

        x1 = np.round(np.arange(self.rmin, self.rmax+rsep*0.1, rsep), 5)
        x0 = np.arange(0, rquality+0.1, int(rquality/(len(x1)-1)))
        
        y1 = np.round(np.arange(self.imax, self.imin-isep*0.1, -isep, ), 5)
        y0 = np.arange(0, iquality+0.1, int(iquality/(len(y1)-1)))
        
        #Get the subplot figure 
        fig, ax = plt.subplots(1, 1, figsize=(20, 20))
        
        #Load the colorObject or upload the colorObject as a pickle object 
        if loadColor: 
            ColorObjs = loadObject('ColorMaps')
        else:
            
            UniSet = np.unique(Matrix.reshape(1, rquality*iquality))

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

        ax.set_title('Iterated Exponential Tower Fractal')
        
        #plot the graph 
        plt.show()
        
        #Save the image as a heatmap 
        if name:
            fig.savefig(f'Images/{name}.png', dpi = DPI)

        


