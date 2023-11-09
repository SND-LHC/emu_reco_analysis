import ROOT as r
r.gStyle.SetOptStat(0)
r.gROOT.SetBatch(True)

def loadHists(histFile, query=None):
    f = r.TFile.Open(histFile)
    histList = {}
    keyList = f.GetListOfKeys()
    for key in keyList:
        # if query is not None and key.GetName() not in query:
        #    continue
        hist = f.Get(key.GetName())
        if key.GetName()=='cz':continue
        hist.SetDirectory(r.gROOT)
        hist.SetName(key.GetName())
        histList[key.GetName()] = hist
    if len(histList) == 0: raise Exception('ERROR: histList is empty!')
    f.Close()
    return histList


from argparse import ArgumentParser
parser = ArgumentParser()
parser.add_argument("-f", "--from", dest="fromPlate", help="from plate", type=int,required=True)
parser.add_argument("-t", "--to", dest="toPlate", help="to plate", type=int,required=True)

options = parser.parse_args()

h_dens = r.TH2D('h_dens','gc_density;plate;gcd',60,0,60,300,0,30000)
lumi = 8.643
for plate in range(options.toPlate, options.fromPlate+1):
	f = 'goodcoupleshistos_cells'+str(plate)+'.root'
	goodHistos = loadHists(f)
	c = r.TCanvas('c','c')
	goodHistos['hxy'].Draw('COLZ')
	c.Print('goodcouples_xy_'+str(plate)+'.png')
	goodcouples = goodHistos['hxy'].GetEntries()
	gc_density = goodcouples/10./6./lumi
	h_dens.Fill(plate, gc_density)

densityFile = r.TFile("density.root", "RECREATE")
h_dens.Write()
densityFile.Write()
densityFile.Close()


