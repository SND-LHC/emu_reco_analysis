import fedrarootlogon
import ROOT as r
import sys

emulsioncell = r.EdbCell2()
emulsioncell.InitCell(19,0,190000,19,0,190000,1)

jx = int(sys.argv[1])
jy = int(sys.argv[2])
#storing it to file
with open("linkheadercut{}_{}.rootrc".format(jx,jy),"w") as inputfile:
 inputfile.write("fedra.link.read.HeaderCut:               TMath::Abs(eXview-{:.0f}) < {:.0f} && TMath::Abs(eYview-{:.0f}) < {:.0f}\n".format(emulsioncell.X(jx),emulsioncell.Xbin()/2.,emulsioncell.Y(jy),emulsioncell.Ybin()/2.))


