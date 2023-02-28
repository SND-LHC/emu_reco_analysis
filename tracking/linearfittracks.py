'''reading tracks, estimating resolution from a linear fit'''
import ROOT as r
import fedrarootlogon
import progressbar #for bars in loops

minnseg = 18 # if 10 plates
store_every_ntracks = 1
r.gStyle.SetOptFit(111)

#tracksfile = r.TFile.Open("b000001.0.0.0_first5plates.trk.root") #first 5 plates of W1 B1
tracksfile = r.TFile.Open("b000431.0.0.0.trk.root") # last 10 plates of W1 B1
#tracksfile = r.TFile.Open("b000011.0.0.0.trk.root") #last 10 plates of W1 B1 (1 cm2)
trackstree = tracksfile.Get("tracks")
#defining graphs
gzx = r.TGraph()
gzy = r.TGraph()

fzx = r.TF1("fzx","pol1")
fzy = r.TF1("fzy","pol1")

#defining resolution histograms
hrestx = r.TH1D("hrestx","Angular resolution;TX",60,-0.06,0.06)
hresty = r.TH1D("hresty","Angular resolution;TY",60,-0.06,0.06)
hresx = r.TH1D("hresx","Position resolution;X[#mum]",60,-60,60)
hresy = r.TH1D("hresy","Position resolution;Y[#mum]",60,-60,60)

#minnseg = 4 #if 5 plates
czx = r.TCanvas()
czy = r.TCanvas()
#starting loop over tracks
#ntracks = 100000
ntracks = trackstree.GetEntries()
bar = progressbar.ProgressBar(maxval=ntracks, \
    widgets=[progressbar.Bar('=', '[', ']'), ' ', progressbar.Percentage()])
bar.start()

outputfile = r.TFile("checktrackslinearfits.root","RECREATE")
restree = r.TNtuple("restree","Tree of residuals","itrack:dx:dy:dtx:dty")
for itrack in range(ntracks):
 trackstree.GetEntry(itrack)
 #condition over track length
 if (trackstree.nseg < minnseg):
  bar.update(itrack+1)
  continue
 segments = trackstree.s
 #looping over track segments
 ipoint = 0
 for seg in segments:

  varx = seg.COV()[0,0]
  vary = seg.COV()[1,1]

  gzx.SetPoint(ipoint,seg.Z(), seg.X())
  gzy.SetPoint(ipoint,seg.Z(), seg.Y())
  #if i want to add errors, still error on z is not so clear
  #gzx.SetPointError(ipoint,300,r.TMath.Sqrt(varx))
  #gzy.SetPointError(ipoint,300,r.TMath.Sqrt(vary))

  ipoint = ipoint + 1

 #(eventually) drawing, making fit
 #czx.cd()
 #gzx.Draw("AP*")
 gzx.Fit(fzx,"Q")

 #czy.cd()
 #gzy.Draw("AP*")
 gzy.Fit(fzy,"Q")


 if (itrack%store_every_ntracks == 0):
  gzx.Write("zx_{}".format(itrack))
  gzy.Write("zy_{}".format(itrack))

 #comparing fitted and found angles/position
 tx_fitted = fzx.GetParameter(1)
 ty_fitted = fzy.GetParameter(1)
 ipoint = 0
 for seg in segments:
  #comparing angles
  restx = tx_fitted - seg.TX()
  resty = ty_fitted - seg.TY()

  hrestx.Fill(restx)
  hresty.Fill(resty)

  #find y value for given z, then comparing positions
  x_fitted = fzx.Eval(seg.Z())
  y_fitted = fzy.Eval(seg.Z())

  resx = x_fitted - seg.X()
  resy = y_fitted - seg.Y()
  hresx.Fill(resx)
  hresy.Fill(resy)
  
  #filling entries for later analysis
  restree.Fill(itrack,resx,resy,restx,resty)
  #empty graphs
  gzx.RemovePoint(ipoint)
  gzy.RemovePoint(ipoint)

  ipoint = ipoint + 1
 #end of track, updating progress bar and moving to next
 bar.update(itrack+1)
#end of track loop, draw resolution histograms and gaussian fits
bar.finish()
cresang = r.TCanvas()
cresang.Divide(2,1)
cresang.cd(1)
hrestx.Draw()
hrestx.Fit("gaus","","",-0.015,0.015)
cresang.cd(2)
hresty.Draw()
hresty.Fit("gaus","","",-0.015,0.015)

crespos = r.TCanvas()
crespos.Divide(2,1)
crespos.cd(1)
hresx.Draw()
hresx.Fit("gaus","","",-15,15)
crespos.cd(2)
hresy.Draw()
hresy.Fit("gaus","","",-15,15)

outputfile.cd()
restree.Write()
outputfile.Close()
