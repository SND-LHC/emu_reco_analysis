import ROOT as r

cpu = r.TFile.Open("/home/scanner/sndlhc/RUN1/b000224/plots/goodcoupleshistos_106.root","read")
gpulow = r.TFile.Open("/home/scanner/sndlhc/RUN1/b000024/plots/goodcoupleshistos_306.root","read")
gpuminlen20 = r.TFile.Open("/home/scanner/sndlhc/RUN1/b000124/plots/goodcoupleshistos_6.root","read")
#gpuminlen25 = r.TFile.Open("/home/scanner/sndlhc/RUN1/b000124/plots/goodcoupleshistos_306.root","read")
#gpuminlen30 = r.TFile.Open("/home/scanner/sndlhc/RUN1/b000124/plots/goodcoupleshistos_106.root","read")

#times in ms
cputime = 25516701.894
gpulowtime = 7667334.026
gpuminlen20time = 3863351.777
#gpuminlen25time = 3361761.550
#gpuminlen30time = 2818286.411

integralangles = []
integralangles_small = []
graphtime_eff = r.TGraph()
graphtime_smalleff = r.TGraph()

ctxty1D = r.TCanvas("ctxty1D","1D histograms of tx and ty")
ctxty1D.Divide(1,2)

ctxty1D_small = r.TCanvas("ctxty1D_small","1D histograms of tx and ty smaller range")
ctxty1D_small.Divide(1,2)

ctheta = r.TCanvas("ctheta","Theta histogram")
ctheta_small = r.TCanvas("ctheta_small","Theta histogram at smaller range")
def plot(histfile, title, color = False):
 ctxty1D.cd(1)
 htx = histfile.Get("htx")
 htx.SetTitle(title+": {:d} couples".format(int(htx.Integral())))
 if color:
  htx.SetLineColor(color)
  htx.Draw("SAME")
 else:
  htx.Draw()
 ctxty1D.cd(2)
 hty = histfile.Get("hty")
 if color:
  hty.SetLineColor(color)
  hty.Draw("SAME")
 else:
  hty.Draw()
 hty.SetTitle(title+": {:d} couples".format(int(hty.Integral())))

 ctheta.cd()
 htheta = histfile.Get("htheta")
 integralangles.append(htheta.Integral())
 htheta.GetXaxis().SetRange(1,78);
 htheta.SetTitle(title+": {:d} couples".format(int(htheta.Integral())))
 if color:
  htheta.SetLineColor(color)
  htheta.Draw("SAME")
 else:
  htheta.Draw()

 ctxty1D_small.cd(1)
 htx_small = histfile.Get("htx_small")
 htx_small.SetTitle(title+": {:d} couples".format(int(htx_small.Integral())))
 if color:
  htx_small.SetLineColor(color)
  htx_small.Draw("SAME")
 else:
  htx_small.Draw()
 ctxty1D_small.cd(2)
 hty_small = histfile.Get("hty_small")
 if color:
  hty_small.SetLineColor(color)
  hty_small.Draw("SAME")
 else:
  hty_small.Draw()
 hty_small.SetTitle(title+": {:d} couples".format(int(hty_small.Integral())))

 ctheta_small.cd()
 htheta_small = histfile.Get("htheta_small")
 integralangles_small.append(htheta_small.Integral())
 htheta_small.SetTitle(title+": {:d} couples".format(int(htheta_small.Integral())))
 if color:
  htheta_small.SetLineColor(color)
  htheta_small.Draw("SAME")
 else:
  htheta_small.Draw()
 

plot(cpu,"CPU")
plot(gpulow,"GPU lowthresh",r.kGreen)
plot(gpuminlen20, "GPU fast0",r.kRed)
#plot(gpuminlen25, "GPU fast1",r.kMagenta)
#plot(gpuminlen30, "GPU fast2",r.kBlack)


ctxty1D.GetPad(1).BuildLegend()
ctxty1D.GetPad(2).BuildLegend()

ctxty1D_small.GetPad(1).BuildLegend()
ctxty1D_small.GetPad(2).BuildLegend()

ctheta.BuildLegend()
ctheta_small.BuildLegend()



#graph ncouples with respect to option
graphtime_eff.AddPoint(cputime, integralangles[0]/integralangles[0])
graphtime_eff.AddPoint(gpulowtime, integralangles[1]/integralangles[0])
graphtime_eff.AddPoint(gpuminlen20time, integralangles[2]/integralangles[0])
#graphtime_eff.AddPoint(gpuminlen25time, integralangles[3]/integralangles[0])
#graphtime_eff.AddPoint(gpuminlen30time, integralangles[4]/integralangles[0])

graphtime_smalleff.AddPoint(cputime, integralangles_small[0]/integralangles_small[0])
graphtime_smalleff.AddPoint(gpulowtime, integralangles_small[1]/integralangles_small[0])
graphtime_smalleff.AddPoint(gpuminlen20time, integralangles_small[2]/integralangles_small[0])
#graphtime_eff.AddPoint(gpuminlen25time, integralangles[3]/integralangles[0])
#graphtime_eff.AddPoint(gpuminlen30time, integralangles[4]/integralangles[0])

ccomparison = r.TCanvas()
graphtime_eff.SetTitle("Processing comparison;Time[ms];IntegralAllAngles")
graphtime_eff.Draw("AP*")

ccomparison_small = r.TCanvas()
graphtime_smalleff.SetTitle("Processing comparison;Time[ms];IntegralLowAngles")
graphtime_smalleff.Draw("AP*")
