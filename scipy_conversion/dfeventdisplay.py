'''displaying showers from csv dataframe (created on 10 July 2020) '''

import matplotlib.pyplot as plt
from matplotlib.collections import LineCollection

def computelines(df):
 '''compute lines for angles'''
 dz = 300 #approximative width of emulsion film
 #first, we need to compute start points and end points to draw lines
 startx = (df["x"]-df["TX"]*dz/2.).to_numpy()
 endx =  (df["x"]+ df["TX"]*dz/2.).to_numpy()

 starty = (df["y"] - df["TY"]*dz/2.).to_numpy()
 endy = (df["y"]+ df["TY"]*dz/2.).to_numpy()
  
 startz = (df["z"]-dz/2.).to_numpy()
 endz = (df["z"]+dz/2.).to_numpy()
 #converting from pandas to numpy arrays and building the lines
 newlinexz = []
 newlineyz = []
 for i in range(len(df)):
  newlinexz.append([(startz[i],startx[i]),(endz[i],endx[i])])
  newlineyz.append([(startz[i],starty[i]),(endz[i],endy[i])])

 lcxz = LineCollection(newlinexz)
 lcyz = LineCollection(newlineyz)
 #returning the lines to be drawn
 return lcxz, lcyz


def displayevent(df, whichevent, mycolor, axiszy, axiszx):
 '''displays with segments'''
 dfevent = df.query("MCEvent=={}".format(whichevent))
 (lcxz,lcyz) = computelines(dfevent)
 axiszy.plot(dfevent['z'], dfevent['y'],"o",fillstyle="none",color = mycolor)
 axiszx.plot(dfevent['z'], dfevent['x'],"o",fillstyle="none",color = mycolor)

 axiszy.set_title("ZY distribution")
 axiszy.set_xlabel("z[$\mu m$]")
 axiszy.set_ylabel("y[$\mu m$]")

 axiszx.set_title("ZX distribution")
 axiszx.set_xlabel("z[$\mu m$]")
 axiszx.set_ylabel("x[$\mu m$]")

 axiszx.add_collection(lcxz)
 axiszy.add_collection(lcyz)

def drawallevents(df,nevents,axes):
 '''draw nevents from df with axes representing the three projections: in this order
    1) xy
    2) zy
    3) zx
 '''
 import numpy as np

 for ishower in range(nevents):
  dfevent = df.query("MCEvent == {}".format(ishower))
  #generate a random color

  r = np.random.random()
  g = np.random.random()
  b = np.random.random()
  color = (r, g, b)

  axes[0].plot(dfevent["x"],dfevent["y"],"o",fillstyle="none", c=color)
 
  axes[1].plot(dfevent["z"],dfevent["y"],"o",fillstyle="none",c=color)

  axes[2].plot(dfevent["z"],dfevent["x"],"o",fillstyle="none",c=color)


 axes[0].set_title("XY distribution of all surface")
 axes[0].set_xlabel("x[$\mu m$]")
 axes[0].set_ylabel("y[$\mu m$]")

 axes[1].set_title("ZY distribution of all surface")
 axes[1].set_xlabel("z[$\mu m$]")
 axes[1].set_ylabel("y[$\mu m$]")

 axes[2].set_title("ZX distribution of all surface")
 axes[2].set_xlabel("z[$\mu m$]")
 axes[2].set_ylabel("x[$\mu m$]")