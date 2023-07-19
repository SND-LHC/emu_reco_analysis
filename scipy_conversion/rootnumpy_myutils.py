import ROOT as r
import numpy as np
import root_numpy


def combinevectors(arrx,arry):
 arrx = arrx[:, np.newaxis]
 arry = arry[:, np.newaxis]
 #concatenate along columns
 arrxy = np.concatenate([arrx,arry],axis=1)
 return arrxy
    
def fillgraph2D(graph, arrx,arry):
 arrxy = combinevectors(arrx,arry)
 root_numpy.fill_graph(graph, arrxy)

def fillgraph1D(graph,arrx):
 root_numpy.fill_graph(graph,arrx)

def fillprofile2D(profile, arrx,arry):
 arrxy = combinevectors(arrx,arry)
 root_numpy.fill_profile(profile, arrxy)

def fillprofile1D(profile,arrx):
 root_numpy.fill_profile(profile,arrx)

def fillhist2D(hist, arrx, arry):
 arrxy = combinevectors(arrx,arry)
 root_numpy.fill_hist(hist, arrxy) 

def fillhist1D(hist,arrx):
 root_numpy.fill_hist(hist,arrx)
