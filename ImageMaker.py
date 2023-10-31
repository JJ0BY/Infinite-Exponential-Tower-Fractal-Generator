#!/usr/bin/env python
# coding: utf-8

#Import Local files
import sys
sys.path.append("..") 
from FractalCreation import FractalCreation
import csv
import numpy

#Specify the settings used here 

rmin = -2
rmax = 1
imin = 0
imax = 2

qualityPerUnit = 1000

FilePath = "Data/test_data3.csv"

ImageName = "ITF"

LoadColorValue = True 

#End settings

matrix2 = np.genfromtxt(FilePath, delimiter=",")

OBJ = FractalCreation(rmin, rmax, imin, imax, qualityPerUnit)

#Data Cleaning 
matrix2 = np.delete(matrix2, -1, 1) 
matrix2[matrix2 == 0] = np.nan

dpi_val = max(matrix2.shape[0], matrix2.shape[1])/20 

#Plot and save the image 
OBJ.PlotSave(Matrix=matrix2, name=f'{ImageName}_Quality_{qualityPerUnit}', DPI=dpi_val, loadColor=LoadColorValue)




