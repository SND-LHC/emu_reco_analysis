import ROOT as r
import numpy as np
import fedrarootlogon
pos = [[],[],[],[]]
#pos =         [xmin,   xmax,   ymin,   ymax,    zmin , zmax]
pos[0].append([0,      53000,  0,      53000,  -37223.46, 0])
pos[0].append([53000,  96000,  0,      53000,  -37223.46, 0])
pos[0].append([0,      53000,  53000,  96000,  -37223.46, 0])
pos[0].append([53000,  96000,  53000,  96000,  -37223.46, 0])
pos[1].append([96000,  138000, 0,      53000,  -37223.46, 0])
pos[1].append([138000, 200000, 0,      53000,  -37223.46, 0])
pos[1].append([96000,  138000, 53000,  96000,  -37223.46, 0])
pos[1].append([138000, 200000, 53000,  96000,  -37223.46, 0])
pos[2].append([0,      53000,  96000,  138000, -37223.46, 0])
pos[2].append([53000,  96000,  96000,  138000, -37223.46, 0])
pos[2].append([0,      53000,  138000, 200000, -37223.46, 0])
pos[2].append([53000,  96000,  138000, 200000, -37223.46, 0])
pos[3].append([96000,  138000, 96000,  138000, -37223.46, 0])
pos[3].append([138000, 200000, 96000,  138000, -37223.46, 0])
pos[3].append([96000,  138000, 138000, 200000, -37223.46, 0])
pos[3].append([138000, 200000, 138000, 200000, -37223.46, 0])
pos_sim = [-39450, 0]
pos_sim_x = [289, 299]
pos_sim_y = [85, 95]

path = 	'/home/scanner/sndlhc/RUN0/b000131/vertexfiles/upstream31'
paths = [path+'/first', path+'/second', path+'/third', path+'/fourth']
vertexFiles = [[],[],[],[]]

r.gROOT.SetBatch(False)
r.gStyle.SetOptStat(0)

#flag0 stats
cf0 = r.TCanvas('cf0', 'flag 0 stats', 1200, 1000)
cf0.Divide(2, 2)
h_n0 = r.TH1D('ntracks', 'RUN0 data;multiplicity', 48, 2.5, 50.5)
h_nseg = r.TH1D('nseg', 'RUN0 data;nseg', 32, 0, 32)
h_npl = r.TH1D('npl', 'RUN0 data;npl', 32, 0, 32)
h_ff = r.TH1D('ff', 'RUN0 data;FF', 22, -0.05, 1.05)
h_n0_sim = r.TH1D('ntracks_mc', 'Monte Carlo BKG;multiplicity', 48, 2.5, 50.5)
h_nseg_sim = r.TH1D('nseg_mc', 'Monte Carlo BKG;nseg', 32, 0, 32)
h_npl_sim = r.TH1D('npl_mc', 'Monte Carlo BKG;npl', 32, 0, 32)
h_ff_sim = r.TH1D('ff_mc', 'Monte Carlo BKG;FF', 22, -0.05, 1.05)

#flag0 avg stats
cf0avg = r.TCanvas('cf0avg', 'cf0avg', 1200, 1000)
cf0avg.Divide(2, 2)
h_nseg_avg = r.TH1D('nseg_avg', 'RUN0 data;nseg', 31, 0, 31)
h_npl_avg = r.TH1D('npl_avg', 'RUN0 data;npl', 31, 0, 31)
h_ff_avg = r.TH1D('ff_avg', 'RUN0 data;FF', 100, -0.01, 1.01)
h_nseg_avg_sim = r.TH1D('nseg_avg_mc', 'Monte Carlo;nseg', 31, 0, 31)
h_npl_avg_sim = r.TH1D('npl_avg_mc', 'Monte Carlo;npl', 31, 0, 31)
h_ff_avg_sim = r.TH1D('ff_avg_mc', 'Monte Carlo BKG;FF', 100, -0.01, 1.01)

#impact parameter
c_imp = r.TCanvas('c_imp', 'impact parameter', 1400, 600)
c_imp.Divide(2, 1)
h_imp = r.TH1D('ip', 'RUN0 data;ip[um]', 50, -0.5, 150)
h_imp_sim = r.TH1D('ip_mc', 'Monte Carlo BG;ip[um]', 50, 0, 150)
h_imp_avg = r.TH1D('ip_avg', 'RUN0 data;ip[um]', 50, 0, 150)
h_imp_avg_sim = r.TH1D('ip_avg_mc', 'Monte Carlo BKG;ip[um]', 30, 0, 150)

#vertex probability
c_prob = r.TCanvas('c_prob', 'vtx prob', 800, 600)
h_prob = r.TH1D('vtx_prob', 'RUN0 data;prob', 30, 0, 1.1)
h_prob_sim = r.TH1D('vtx_prob_mc', 'Monte Carlo BKG;prob', 100, 0, 1)

#save vertices in root file
outputFile = r.TFile("vertex_selection_newsigma.root","RECREATE")	
outputTree = r.TNtuple("vtx","Tree of vertices","quadrant:subquadrant:vID:flag:n:nin:nout:nseg:npl:nfirst:ff:meanIP:prob")

for quad in range(4):
  for subquad in range(4):
    print('Quadrant {}/16'.format(quad*4+subquad+1))
    vertexFiles[quad].append(paths[quad]+'/vertextree'+str(subquad)+'.root')
    dproc = r.EdbDataProc()
    gAli = dproc.PVR()
    scancond = r.EdbScanCond()
    scancond.SetSigma0(4,4,0.005,0.005) #change sigma0
    scancond.SetDegrad(4) #change angular degradation
    gAli.SetScanCond(scancond)
    vertexrec = r.EdbVertexRec()
    vertexrec.SetPVRec(gAli)
    vertexrec.eDZmax=3000.
    vertexrec.eProbMin=0.0001
    vertexrec.eImpMax=15.
    vertexrec.eUseMom=False
    vertexrec.eUseSegPar=True
    vertexrec.eQualityMode=0
    proc = r.EdbDataProc()
    dproc.ReadVertexTree(vertexrec, vertexFiles[quad][subquad], "1")
    vertices = gAli.eVTX
    for vtx in vertices:
      ntracks = vtx.N()
      vx = vtx.VX()
      vy = vtx.VY()
      vz = vtx.VZ()
      flag = vtx.Flag()
      #cell cuts
      if vx < pos[quad][subquad][0]: continue
      if vx > pos[quad][subquad][1]: continue
      if vy < pos[quad][subquad][2]: continue
      if vy > pos[quad][subquad][3]: continue
      #soft cuts
      if ntracks < 3: continue
      if vz < pos[quad][subquad][4]: continue
      if vz > pos[quad][subquad][5]: continue
      #flag 0 study
      if flag == 0 or flag == 3:
        nplList = []
        nsegList = []
        ffList = []
        ipList = []
        angleList = []
        for itrack in range(ntracks):
          track = vtx.GetTrack(itrack)
          h_imp.Fill(vtx.GetVTa(itrack).Imp())         
          ipList.append(vtx.GetVTa(itrack).Imp())
          nseg = track.N()
          npl = track.Npl()
          nfirst = track.GetSegmentFirst().Plate()
          nava = 34 - nfirst + 1
          FF = float(nseg)/float(nava)
          nplList.append(npl)
          nsegList.append(nseg)
          ffList.append(FF)
          tx = track.TX()
          ty = track.TY()
          angle = r.TMath.Sqrt(tx*tx+ty*ty)
          angleList.append(angle)
          h_nseg.Fill(nseg)
          h_npl.Fill(npl)
          h_ff.Fill(FF)
        h_prob.Fill(vtx.V().prob())
        h_n0.Fill(ntracks)
        h_nseg_avg.Fill(np.mean(nsegList))
        h_npl_avg.Fill(np.mean(nplList))
        h_ff_avg.Fill(np.mean(ffList))
        h_imp_avg.Fill(np.mean(ipList))
        outputTree.Fill(quad, subquad, vtx.ID(), flag, ntracks, 0, ntracks, np.mean(nsegList), np.mean(nplList), nfirst, np.mean(ffList), np.mean(ipList), vtx.V().prob())

#montecarlo
dproc = r.EdbDataProc()
gAli = dproc.PVR()
scancond = r.EdbScanCond()
scancond.SetSigma0(4,4,0.005,0.005) #change sigma0
scancond.SetDegrad(4) #change angular degradation
gAli.SetScanCond(scancond)
vertexrec = r.EdbVertexRec()
vertexrec.SetPVRec(gAli)
vertexrec.eDZmax=3000.
vertexrec.eProbMin=0.0001
vertexrec.eImpMax=15.
vertexrec.eUseMom=False
vertexrec.eUseSegPar=True
vertexrec.eQualityMode=0
proc = r.EdbDataProc()
dproc.ReadVertexTree(vertexrec, "vertextree_sim.root", "1")
vertices = gAli.eVTX
for vtx in vertices:
  ntracks = vtx.N()
  vx = vtx.VX()
  vy = vtx.VY()
  vz = vtx.VZ()
  flag = vtx.Flag()
  #soft cuts
  if ntracks < 3: continue
  if vz < pos_sim[0]: continue
  if vz > pos_sim[1]: continue
  #flag 0 study
  if flag == 0 or flag == 3:
    nplList = []
    nsegList = []
    ffList = []
    ipList = []
    angleList = []
    for itrack in range(ntracks):
      track = vtx.GetTrack(itrack)
      h_imp_sim.Fill(vtx.GetVTa(itrack).Imp())         
      ipList.append(vtx.GetVTa(itrack).Imp())
      nseg = track.N()
      npl = track.Npl()
      nfirst = track.GetSegmentFirst().Plate()
      nava = 31 - nfirst + 1
      FF = float(nseg)/float(nava)
      nplList.append(npl)
      nsegList.append(nseg)
      ffList.append(FF)
      h_nseg_sim.Fill(nseg)
      h_npl_sim.Fill(npl)
      h_ff_sim.Fill(FF)
      tx = track.TX()
      ty = track.TY()
      angle = r.TMath.Sqrt(tx*tx+ty*ty)
      angleList.append(angle)
    h_prob_sim.Fill(vtx.V().prob())
    h_n0_sim.Fill(ntracks)
    h_nseg_avg_sim.Fill(np.mean(nsegList))
    h_npl_avg_sim.Fill(np.mean(nplList))
    h_ff_avg_sim.Fill(np.mean(ffList))
    h_imp_avg_sim.Fill(np.mean(ipList))

#flag0 stats
cf0.cd(1).SetLogy()
h_n0_sim.SetLineWidth(2)
h_n0_sim.SetLineColor(r.kRed)
h_n0_sim.SetFillStyle(3005)
h_n0_sim.SetFillColor(r.kRed)
h_n0_sim.DrawNormalized()
#r.gPad.Update()
#statsbox = r.gPad.GetPrimitive("stats")
#y1 = statsbox.GetY1NDC() # (lower) y start position of stats box
#y2 = statsbox.GetY2NDC() # (upper) y start position of stats box
#newy1 = 2 * y1 - y2   # new (lower) y start position of stats box
#newy2 = y1            # new (upper) y start position of stats box
#statsbox.SetY1NDC(newy1) #    //set new y start position
#statsbox.SetY2NDC(newy2) #   //set new y end position
h_n0.SetLineWidth(2)
h_n0.SetLineColor(1)
h_n0.DrawNormalized("sames")
h_n0.GetYaxis().SetRangeUser(10E-6, 2)
cf0.cd(1).BuildLegend(0.3,0.72,0.62,0.9)
cf0.cd(2).SetLogy()
h_nseg_sim.SetLineWidth(2)
h_nseg_sim.SetLineColor(r.kRed)
h_nseg_sim.SetFillStyle(3005)
h_nseg_sim.SetFillColor(r.kRed)
h_nseg_sim.DrawNormalized()
#r.gPad.Update()
#statsbox = r.gPad.GetPrimitive("stats")
#y1 = statsbox.GetY1NDC() # (lower) y start position of stats box
#y2 = statsbox.GetY2NDC() # (upper) y start position of stats box
#newy1 = 2 * y1 - y2   # new (lower) y start position of stats box
#newy2 = y1            # new (upper) y start position of stats box
#statsbox.SetY1NDC(newy1) #    //set new y start position
#statsbox.SetY2NDC(newy2) #   //set new y end position
h_nseg.SetLineWidth(2)
h_nseg.SetLineColor(1)
h_nseg.DrawNormalized("sames")
cf0.cd(2).BuildLegend(0.3,0.72,0.62,0.9)
cf0.cd(3).SetLogy()
h_npl_sim.SetLineWidth(2)
h_npl_sim.SetLineColor(r.kRed)
h_npl_sim.SetFillStyle(3005)
h_npl_sim.SetFillColor(r.kRed)
h_npl_sim.DrawNormalized()
#r.gPad.Update()
#statsbox = r.gPad.GetPrimitive("stats")
#y1 = statsbox.GetY1NDC() # (lower) y start position of stats box
#y2 = statsbox.GetY2NDC() # (upper) y start position of stats box
#newy1 = 2 * y1 - y2   # new (lower) y start position of stats box
#newy2 = y1            # new (upper) y start position of stats box
#statsbox.SetY1NDC(newy1) #    //set new y start position
#statsbox.SetY2NDC(newy2) #   //set new y end position
h_npl.SetLineWidth(2)
h_npl.SetLineColor(1)
h_npl.DrawNormalized("sames")
cf0.cd(3).BuildLegend(0.3,0.72,0.62,0.9)
cf0.cd(4).SetLogy()
h_ff_sim.SetLineWidth(2)
h_ff_sim.SetLineColor(r.kRed)
h_ff_sim.SetFillStyle(3005)
h_ff_sim.SetFillColor(r.kRed)
h_ff_sim.DrawNormalized()
#r.gPad.Update()
#statsbox = r.gPad.GetPrimitive("stats")
#y1 = statsbox.GetY1NDC() # (lower) y start position of stats box
#y2 = statsbox.GetY2NDC() # (upper) y start position of stats box
#newy1 = 2 * y1 - y2   # new (lower) y start position of stats box
#newy2 = y1            # new (upper) y start position of stats box
#statsbox.SetY1NDC(newy1) #    //set new y start position
#statsbox.SetY2NDC(newy2) #   //set new y end position
h_ff.SetLineWidth(2)
h_ff.SetLineColor(1)
h_ff.DrawNormalized("sames")
cf0.cd(4).BuildLegend(0.3,0.72,0.62,0.9)
cf0.Update()
#cf0.Print('flag0_mc.png')

h_ff_avg_sim.SetLineWidth(2)
h_ff_avg_sim.SetLineColor(r.kRed)
h_ff_avg_sim.SetFillStyle(3005)
h_ff_avg_sim.SetFillColor(r.kRed)
'''
#flag0 avg stats
cf0avg.cd(1).SetLogy()
h_n0_sim.SetLineColor(r.kRed)
h_n0_sim.DrawNormalized()
r.gPad.Update()
h_n0_sim.GetYaxis().SetRangeUser(10E-6, 2)
statsbox = r.gPad.GetPrimitive("stats")
y1 = statsbox.GetY1NDC() # (lower) y start position of stats box
y2 = statsbox.GetY2NDC() # (upper) y start position of stats box
newy1 = 2 * y1 - y2   # new (lower) y start position of stats box
newy2 = y1            # new (upper) y start position of stats box
statsbox.SetY1NDC(newy1) #    //set new y start position
statsbox.SetY2NDC(newy2) #   //set new y end position
h_n0.DrawNormalized("sames")
cf0avg.cd(1).BuildLegend(0.3,0.72,0.62,0.9)
cf0avg.cd(2).SetLogy()
h_nseg_avg_sim.SetLineColor(r.kRed)
h_nseg_avg_sim.DrawNormalized()
r.gPad.Update()
statsbox = r.gPad.GetPrimitive("stats")
y1 = statsbox.GetY1NDC() # (lower) y start position of stats box
y2 = statsbox.GetY2NDC() # (upper) y start position of stats box
newy1 = 2 * y1 - y2   # new (lower) y start position of stats box
newy2 = y1            # new (upper) y start position of stats box
statsbox.SetY1NDC(newy1) #    //set new y start position
statsbox.SetY2NDC(newy2) #   //set new y end position
h_nseg_avg.DrawNormalized("sames")
cf0avg.cd(2).BuildLegend(0.3,0.72,0.62,0.9)
cf0avg.cd(3).SetLogy()
h_npl_avg_sim.SetLineColor(r.kRed)
h_npl_avg_sim.DrawNormalized()
r.gPad.Update()
statsbox = r.gPad.GetPrimitive("stats")
y1 = statsbox.GetY1NDC() # (lower) y start position of stats box
y2 = statsbox.GetY2NDC() # (upper) y start position of stats box
newy1 = 2 * y1 - y2   # new (lower) y start position of stats box
newy2 = y1            # new (upper) y start position of stats box
statsbox.SetY1NDC(newy1) #    //set new y start position
statsbox.SetY2NDC(newy2) #   //set new y end position
h_npl_avg.DrawNormalized("sames")
cf0avg.cd(3).BuildLegend(0.3,0.7,0.6,0.9)
cf0avg.cd(4).SetLogy()
h_ff_avg_sim.SetLineColor(r.kRed)
h_ff_avg_sim.DrawNormalized()
r.gPad.Update()
statsbox = r.gPad.GetPrimitive("stats")
y1 = statsbox.GetY1NDC() # (lower) y start position of stats box
y2 = statsbox.GetY2NDC() # (upper) y start position of stats box
newy1 = 2 * y1 - y2   # new (lower) y start position of stats box
newy2 = y1            # new (upper) y start position of stats box
statsbox.SetY1NDC(newy1) #    //set new y start position
statsbox.SetY2NDC(newy2) #   //set new y end position
h_ff_avg.DrawNormalized("sames")
cf0avg.cd(4).BuildLegend(0.3,0.7,0.6,0.9)
h_ff_avg_sim.GetYaxis().SetRangeUser(3*10E-3, 0.4)
cf0avg.Update()
cf0avg.Print('flag0_avg_mc.png')
'''
#vertex probability
c_prob.cd()
h_prob_sim.SetLineWidth(2)
h_prob_sim.SetLineColor(r.kRed)
h_prob_sim.SetFillStyle(3005)
h_prob_sim.SetFillColor(r.kRed)
h_prob_sim.DrawNormalized()
#r.gPad.Update()
#statsbox = r.gPad.GetPrimitive("stats")
#y1 = statsbox.GetY1NDC() # (lower) y start position of stats box
#y2 = statsbox.GetY2NDC() # (upper) y start position of stats box
#newy1 = 2 * y1 - y2   # new (lower) y start position of stats box
#newy2 = y1            # new (upper) y start position of stats box
#statsbox.SetY1NDC(newy1) #    //set new y start position
#statsbox.SetY2NDC(newy2) #   //set new y end position
h_prob.SetLineWidth(2)
h_prob.SetLineColor(1)
h_prob.DrawNormalized("sames")
c_prob.BuildLegend(0.3,0.72,0.62,0.9)
h_prob_sim.GetYaxis().SetRangeUser(0, 0.35)
c_prob.Update()
#c_prob.Print('prob_mc.png')


#impact parameter
c_imp.cd(1)
h_imp_sim.SetLineWidth(2)
h_imp_sim.SetLineColor(r.kRed)
h_imp_sim.SetFillStyle(3005)
h_imp_sim.SetFillColor(r.kRed)
h_imp_sim.DrawNormalized()
#r.gPad.Update()
#statsbox = r.gPad.GetPrimitive("stats")
#y1 = statsbox.GetY1NDC() # (lower) y start position of stats box
#y2 = statsbox.GetY2NDC() # (upper) y start position of stats box
#newy1 = 2 * y1 - y2   # new (lower) y start position of stats box
#newy2 = y1            # new (upper) y start position of stats box
#statsbox.SetY1NDC(newy1) #    //set new y start position
#statsbox.SetY2NDC(newy2) #   //set new y end position
h_imp.SetLineWidth(2)
h_imp.SetLineColor(1)
h_imp.DrawNormalized("sames")
c_imp.cd(1).BuildLegend(0.3,0.72,0.62,0.9)
c_imp.Update()
c_imp.cd(2)
h_imp_avg_sim.SetLineWidth(2)
h_imp_avg_sim.SetLineColor(r.kRed)
h_imp_avg_sim.SetFillStyle(3005)
h_imp_avg_sim.SetFillColor(r.kRed)
h_imp_avg_sim.DrawNormalized()
#r.gPad.Update()
#statsbox = r.gPad.GetPrimitive("stats")
#y1 = statsbox.GetY1NDC() # (lower) y start position of stats box
#y2 = statsbox.GetY2NDC() # (upper) y start position of stats box
#newy1 = 2 * y1 - y2   # new (lower) y start position of stats box
#newy2 = y1            # new (upper) y start position of stats box
#statsbox.SetY1NDC(newy1) #    //set new y start position
#statsbox.SetY2NDC(newy2) #   //set new y end position
h_imp_avg.DrawNormalized("sames")
c_imp.cd(2).BuildLegend(0.3,0.72,0.62,0.9)
c_imp.Update()
#c_imp.Print('ip_mc.png')

#write output file
outputFile.cd()
outputTree.Write()
outputFile.Close()

histoFile = r.TFile("histo.root", "RECREATE")
h_n0_sim.Write()
h_nseg_sim.Write()
h_npl_sim.Write()
h_ff_sim.Write()
h_imp_sim.Write()
h_ff_avg_sim.Write()
h_imp_avg_sim.Write()
h_prob_sim.Write()
h_n0.Write()
h_nseg.Write()
h_npl.Write()
h_ff.Write()
h_imp.Write()
h_prob.Write()
histoFile.Write()
histoFile.Close()
