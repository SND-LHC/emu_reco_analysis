import ROOT


from argparse import ArgumentParser
parser = ArgumentParser()
parser.add_argument("-n", dest="nSeg", help="nSeg", default="")
options = parser.parse_args()

def loadHists(histFile, query=None):
    f = ROOT.TFile.Open(histFile)
    histList = {}
    keyList = f.GetListOfKeys()
    for key in keyList:
        #if query is not None and key.GetName() not in query:
        #    continue
        hist = f.Get(key.GetName())
        #hist.SetDirectory(ROOT.gROOT)
        hist.SetName(key.GetName())
        histList[key.GetName()] = hist
    if len(histList) == 0: raise Exception('ERROR: histList is empty!')
    f.Close()
    return histList

nseg = options.nSeg
positionxFile = 'polfit_x'+str(nseg)+'.root'
positionyFile = 'polfit_y'+str(nseg)+'.root'
anglexFile = 'gausfit_angx'+str(nseg)+'.root'
angleyFile = 'gausfit_angy'+str(nseg)+'.root'

positionx = loadHists(positionxFile)
positiony = loadHists(positionyFile)
anglex = loadHists(anglexFile)
angley = loadHists(angleyFile)

c = ROOT.TCanvas()
positionx['c1'].Draw()
positionx['c1'].Print('pos_reso_x'+str(nseg)+'.png')

positiony['c1_n2'].Draw()
positiony['c1_n2'].Print('pos_reso_y'+str(nseg)+'.png')

anglex['c1'].Draw()
anglex['c1'].Print('ang_reso_x'+str(nseg)+'.png')

angley['c1_n2'].Draw()
angley['c1_n2'].Print('ang_reso_y'+str(nseg)+'.png')


