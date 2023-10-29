#!/usr/bin/env python
# coding: utf-8

# In[1]:

#Import modules 
import time as time
import tracemalloc as mem
import pickle as P


#Function that uploads a python object asa pickle object 
def uploadObject(Object, filename):
    
    #Print the time it took to upload the object 
    t0 = time.time()
    i=1
    
    #Keep trying to upload the pickle object until you can load the pickle object 
    while 1==1:
        try:
            filehandler = open(filename, 'wb') 
            
            P.dump(Object, filehandler)
            loadObject(filename)
            print('----------------FINISHED SAVING----------------')
            break
        except EOFError:
            print(f'Attempt #{i} failed')
            i+=1
            if i == 11:
                print('Failed to Save')
                break
            
        
    t = time.time() - t0
    print(f'Time: {t} s\n')
    
#Function that loads a pickle object and returns the python object 
def loadObject(filename):
    t0 = time.time()
    infile = open(filename,'rb')
    Object = P.load(infile)
    infile.close()
    t1 = time.time()
    print(f'Time: {t1-t0} s\n')
    return Object

#Function that returns the time and memory of the given function and the function's parameters 
def funcStats(func, funcParams):
    t0 = time.time()
    mem.start()
    
    mem.reset_peak()
    results = func(**funcParams)
    size, peak = mem.get_traced_memory()
    mem.reset_peak()
    mem.stop()
    t = time.time() - t0
    print(f'Time: {t:.3f} s, Peak memory: {(peak/10**6):.3f} Mb')
    return results 


# In[ ]:




