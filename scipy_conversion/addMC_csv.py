import ROOT as r
import fedra2scipy_utils
import pandas as pd
import sys

nbrick = int(sys.argv[1])

inputfile = r.TFile.Open("../inECC_sndLHC.Genie-TGeant4.root")
df = pd.read_csv("brick{}.csv".format(nbrick))

inputfilebkg = r.TFile.Open("../sndLHC.Ntuple-TGeant4-1E3cm2.root")

dfwithMC = fedra2scipy_utils.addtrueMCinfo(df,inputfile,inputfilebkg) 
#storing dataframe
dfwithMC.to_csv("brick{}_withMC.csv".format(nbrick),index = False)
