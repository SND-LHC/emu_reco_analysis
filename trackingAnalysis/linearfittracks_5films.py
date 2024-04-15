import ROOT
import fedrarootlogon
import progressbar #for bars in loops
import numpy as np

nfilms = 57 #total number of films
minnseg = 35
minplate = 55
ROOT.gStyle.SetOptFit(111)

inputfile = "linked_tracks.root"
tracksfile = ROOT.TFile.Open(inputfile)
trackstree = tracksfile.Get("tracks")

#defining resolution histograms
hrestx = []
hresty = []
hresx = []
hresy = []
#defining resolution histograms
for ifilm in range(1,nfilms+1):
    hresx.append(ROOT.TH1D("hresx_{}".format(ifilm),"Position resolution;dx[#mum]",100,-3,3))
    hresy.append(ROOT.TH1D("hresy_{}".format(ifilm),"Position resolution;dy[#mum]",100,-3,3))
    hresty.append(ROOT.TH1D("hresty_{}".format(ifilm),"Angular resolution;dTY",100,-0.01,0.01))
    hrestx.append(ROOT.TH1D("hrestx_{}".format(ifilm),"Angular resolution;dTX",100,-0.01,0.01))
# hrestx = ROOT.TH1D("hrestx","Angular resolution;TX",200,-0.005,0.005)
# hresty = ROOT.TH1D("hresty","Angular resolution;TY",200,-0.005,0.005)
# hresx = ROOT.TH1D("hresx","Position resolution;X[#mum]",200,-5,5)
# hresy = ROOT.TH1D("hresy","Position resolution;Y[#mum]",200,-5,5)

ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetOptFit(1)
outHist = ROOT.TFile.Open("graphs.root","RECREATE")

czx = ROOT.TCanvas()
czy = ROOT.TCanvas()
#starting loop over tracks
ntracks = trackstree.GetEntries()
bar = progressbar.ProgressBar(maxval=ntracks, \
    widgets=[progressbar.Bar('=', '[', ']'), ' ', progressbar.Percentage()])
bar.start()
outputfile = ROOT.TFile("checktrackslinearfits.root","RECREATE")
restree = ROOT.TNtuple("restree","Tree of residuals","itrack:sample:dx:dy:dtx:dty:chi2fzx:chi2fzy")

for itrack in range(ntracks):
    trackstree.GetEntry(itrack)
    #condition over track length
    if (trackstree.nseg < minnseg) or (trackstree.npl < minplate):
        bar.update(itrack+1)
        continue

    FilledFilms = np.zeros(nfilms,dtype=bool)
    segPlate = np.zeros(nfilms)
    segListX = np.zeros(nfilms)
    segListY = np.zeros(nfilms)
    segListZ = np.zeros(nfilms)
    segListTX = np.zeros(nfilms)
    segListTY = np.zeros(nfilms)
    segments = trackstree.s
    segmentsf = trackstree.sf
    #looping over track segments
    for seg, segf in zip(segments, segmentsf):
        #if segp.Plate() < 50: continue
        FilledFilms[seg.Plate()-1] = True
        segPlate[seg.Plate()-1] = seg.Plate()-1
        segListX[seg.Plate()-1] = seg.X()
        segListY[seg.Plate()-1] = seg.Y()
        segListZ[seg.Plate()-1] = seg.Z()
        segListTX[seg.Plate()-1] = seg.TX()
        segListTY[seg.Plate()-1] = seg.TY()
    #if len(segListX) < minnseg: continue

    seg5Plate = [segPlate[i:i+5] for i in range(nfilms - 4)]
    seg5ListsX = [segListX[i:i+5] for i in range(nfilms - 4)]
    seg5ListsY = [segListY[i:i+5] for i in range(nfilms - 4)]
    seg5ListsZ = [segListZ[i:i+5] for i in range(nfilms - 4)]
    seg5ListsTX = [segListTX[i:i+5] for i in range(nfilms - 4)]
    seg5ListsTY = [segListTY[i:i+5] for i in range(nfilms - 4)]

    seg5ListspX = [np.delete(sublist,2) for sublist in seg5ListsX]
    seg5ListspY = [np.delete(sublist,2) for sublist in seg5ListsY]
    seg5ListspZ = [np.delete(sublist,2) for sublist in seg5ListsZ]

    for sample in range(len(seg5ListspX)):
        if(np.all(FilledFilms[sample:sample+5])):
            
            gzx = ROOT.TGraph(4, seg5ListspZ[sample], seg5ListspX[sample])
            gzy = ROOT.TGraph(4, seg5ListspZ[sample], seg5ListspY[sample])
            fzx = ROOT.TF1("fzx","pol1")
            fzy = ROOT.TF1("fzy","pol1")
            gzx.Fit(fzx,"Q")
            gzy.Fit(fzy,"Q") 

            x_fitted = fzx.Eval(seg5ListsZ[sample][2])
            y_fitted = fzy.Eval(seg5ListsZ[sample][2])
            resx = x_fitted - seg5ListsX[sample][2]
            resy = y_fitted - seg5ListsY[sample][2]

            tx_fitted = fzx.GetParameter(1)
            ty_fitted = fzy.GetParameter(1)
            restx = tx_fitted - seg5ListsTX[sample][2]
            resty = ty_fitted - seg5ListsTY[sample][2]

            ifilm = int(seg5Plate[sample][2])
            hresx[ifilm].Fill(resx)
            hresy[ifilm].Fill(resy)
            hrestx[ifilm].Fill(restx)
            hresty[ifilm].Fill(resty)

            restree.Fill(itrack,sample,resx,resy,restx,resty,fzx.GetChisquare(), fzy.GetChisquare())

            # gzx5 = ROOT.TGraph(5, seg5ListspZ[sample], seg5ListspX[sample])
            # gzy5 = ROOT.TGraph(5, seg5ListspZ[sample], seg5ListspY[sample])
            # cfit = ROOT.TCanvas("cfit_{}_{}".format(itrack, sample), "cfit_{}_{}".format(itrack, sample), 1000, 600)
            # cfit.Divide(2,1)
            # cfit.cd(1)
            # gzx.Draw("AP*")
            # gzx5.Draw("P*&&same")
            # cfit.cd(2)
            # gzy.Draw("AP*")
            # gzy5.Draw("P*&&same")
            # outHist.cd()
            # cfit.Write()
    #end of track, updating progress bar and moving to next
    bar.update(itrack+1)
#end of track loop, draw resolution histograms and gaussian fits
bar.finish()
hresx_tot = ROOT.TH2D("hresx_tot","Position resolution;plate;dx[#mum]",60,0,60,100,0,3)
hresy_tot = ROOT.TH2D("hresy_tot","Position resolution;plate;dy[#mum]",60,0,60,100,0,3)
hrestx_tot = ROOT.TH2D("hrestx_tot","Angular resolution;plate;dTX",60,0,60,100,0,0.005)
hresty_tot = ROOT.TH2D("hresty_tot","Angular resolution;plate;dTY",60,0,60,100,0,0.005)
hresx_err = ROOT.TH2D("hresx_err","Position resolution;plate;X[#mum]",60,0,60,100,0,3)
hresy_err = ROOT.TH2D("hresy_err","Position resolution;plate;Y[#mum]",60,0,60,100,0,3)
hrestx_err = ROOT.TH2D("hrestx_err","Angular resolution;plate;TX",60,0,60,100,0,0.005)
hresty_err = ROOT.TH2D("hresty_err","Angular resolution;plate;TY",60,0,60,100,0,0.005)
for ifilm in range(2,nfilms-2):
    hresx[ifilm].Draw()
    fitx = hresx[ifilm].Fit("gaus","SQR","",-1.5,1.5)
    hresy[ifilm].Draw()
    fity = hresy[ifilm].Fit("gaus","SQR","",-1.5,1.5)
    hrestx[ifilm].Draw()
    fittx = hrestx[ifilm].Fit("gaus","SQR","",-0.003,0.003)
    hresty[ifilm].Draw()
    fitty = hresty[ifilm].Fit("gaus","SQR","",-0.003,0.003)
    if int(fitx)==0:
        hresx_tot.Fill(ifilm+1, fitx.Parameter(2))
        hresx_err.Fill(ifilm+1, fitx.ParError(2))
    if int(fity)==0:
        hresy_tot.Fill(ifilm+1, fity.Parameter(2))
        hresy_err.Fill(ifilm+1, fity.ParError(2))
    if int(fittx)==0:
        hrestx_tot.Fill(ifilm+1, fittx.Parameter(2))
        hrestx_err.Fill(ifilm+1, fittx.ParError(2))
    if int(fitty)==0:
        hresty_tot.Fill(ifilm+1, fitty.Parameter(2))
        hresty_err.Fill(ifilm+1, fitty.ParError(2))
# cresang = ROOT.TCanvas("cresang", "cresang", 1000, 600)
# cresang.Divide(2,1)
# cresang.cd(1)
# hrestx.Draw()
# hrestx.Fit("gaus","","",-0.015,0.015)
# cresang.cd(2)
# hresty.Draw()
# hresty.Fit("gaus","","",-0.015,0.015)

# crespos = ROOT.TCanvas("crespos", "crespos", 1000, 600)
# crespos.Divide(2,1)
# crespos.cd(1)
# hresx.Draw()
# hresx.Fit("gaus","","",-15,15)
# crespos.cd(2)
# hresy.Draw()
# hresy.Fit("gaus","","",-15,15)
outHist.cd()
hresx_tot.Write()
hresy_tot.Write()
hrestx_tot.Write()
hresty_tot.Write()
for ifilm in range(nfilms):
    hresx[ifilm].Write()
    hresy[ifilm].Write()
    hresty[ifilm].Write()
    hrestx[ifilm].Write()
outHist.Close()

outputfile.cd()
restree.Write()
outputfile.Close()
