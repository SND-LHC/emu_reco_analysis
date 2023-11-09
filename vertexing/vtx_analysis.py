import ROOT as r
import numpy as np
import fedrarootlogon

pos = [90000, 102000,  90000, 102000,  -75236, 0]

vertexfilepath = '/home/scanner/sndlhc/RUN1/b000021/trackfiles/57_1/rootfiles'
vertexFiles = [[],[],[],[]]
r.gStyle.SetOptStat(1)
#ntracks and flag
c1 = r.TCanvas('c1', 'c1', 1400, 600)
c1.Divide(2,1)
h_n = r.TH1D('h_n','ntracks;ntracks', 50, 0, 50)
h_flag = r.TH1D('h_flag','flag;flag', 6, 0, 6)

#ntracks after soft cut
cn = r.TCanvas('cn', 'ntracks', 800, 600)
h_ntracks = r.TH1D('h_ntracks','ntracks;ntracks', 50, 0, 50)

#vz
cz = r.TCanvas('cz', 'vtx_zposition', 800, 600)
h_vz = r.TH1D('h_vz','vz;vz[um]', 100, -35000, 5000)

#flag1 dot protuct
c_cos = r.TCanvas('c_cos', 'c_cos', 800, 600)
h_cos = r.TH1D('h_cos', 'cos between in/out tracks', 10000, -2, 2)

#flag1 angular difference
cf1 = r.TCanvas('cf1', 'flag 1 angular diff', 1800, 1000)
cf1.Divide(3,2)
h_tx = r.TH1D('h_tx', 'tx diff;tang#theta_{xz}', 100, -5, 5)
h_ty = r.TH1D('h_ty', 'ty diff;tang#theta_{yz}', 100, -5, 5)
h_txy = r.TH2D('h_txy', 'txy diff;tang#theta_{xz};tang#theta_{yz}', 100, -5, 5, 100, -5, 5)
h_theta = r.TH1D('h_theta', 'theta diff;#theta', 100, -3, 3)
h_phi = r.TH1D('h_phi', 'phi diff;#phi', 200, -10, 10)
h_tp = r.TH2D('h_thetaphi', 'theta phi diff;#theta;phi', 100, -3, 3, 200, -10, 10)

#flag0 stats
cf0 = r.TCanvas('cf0', 'flag 0 stats', 1200, 1000)
cf0.Divide(2, 2)
h_n0 = r.TH1D('h_n0', 'ntracks;ntracks', 50, 0, 50)
h_nseg = r.TH1D('h_nseg', 'nseg;nseg', 30, 0, 30)
h_npl = r.TH1D('h_npl', 'npl;npl', 30, 0, 30)
h_ff = r.TH1D('h_ff', 'fillfactor;FF', 20, 0, 2)

#flag0 avg stats
cf0avg = r.TCanvas('cf0avg', 'cf0avg', 1200, 1000)
cf0avg.Divide(2, 2)
h_nseg_avg = r.TH1D('h_nseg_avg', 'nseg_avg;nseg', 30, 0, 30)
h_npl_avg = r.TH1D('h_npl_avg', 'npl_avg;npl', 30, 0, 30)
h_ff_avg = r.TH1D('h_ff_avg', 'fillfactor;FF', 20, 0, 2)

#impact parameter
c_imp = r.TCanvas('c_imp', 'impact parameter', 800, 600)
h_imp = r.TH1D('h_imp', 'impact parameter;ip[um]', 200, -50, 150)

#vertex probability
c_prob = r.TCanvas('c_prob', 'vtx prob', 600, 600)
h_prob = r.TH2D('h_prob', 'vtx prob;ip[um];P', 200, -50, 150, 100, 0, 1)

#vertex position
c_xyz = r.TCanvas('c_xyz', 'vtx_position', 600, 1000)
c_xyz.Divide(1,2)
h_xy = r.TH2D('h_xy','vxy;vx[um];vy[um]',100, 0, 200000, 100, 0, 200000)
h_z = r.TH1D('h_z','vz;vz[um]', 100, -35000, 5000)

#save vertices in root file
outputFile = r.TFile(vertexfilepath+"/vertex_selection.root","RECREATE")	
outputTree = r.TNtuple("vtx","Tree of vertices","vID:flag:n:nin:nout:nseg:npl:ff")



dproc = r.EdbDataProc()
gAli = dproc.PVR()
scancond = r.EdbScanCond()
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
dproc.ReadVertexTree(vertexrec, vertexfilepath+"/vertextree0.root", "1")
vertices = gAli.eVTX
for vtx in vertices:
      ntracks = vtx.N()
      vx = vtx.VX()
      vy = vtx.VY()
      vz = vtx.VZ()
      flag = vtx.Flag()
      #cell cuts
      if vx < pos[0]: continue
      if vx > pos[1]: continue
      if vy < pos[2]: continue
      if vy > pos[3]: continue
      h_n.Fill(ntracks)
      h_vz.Fill(vz)
      #soft cuts
      if ntracks < 3: continue
      if vz < pos[4]: continue
      if vz > pos[5]: continue
      h_flag.Fill(flag)
      #flag 1 study
      if flag == 1 or flag == 4:
        h_ntracks.Fill(ntracks)
        for itrack in range(ntracks):
          if vtx.Zpos(itrack)==0:
            nout = 0
            track = vtx.GetTrack(itrack)
            seg = track.GetSegmentFLast() 
            trackDistList = []
            trackDistListX = []
            trackDistListY = []
            trackDistListTheta = []
            trackDistListPhi = []
            inTrackTX = seg.TX()
            inTrackTY = seg.TY()
            inTrackTheta = seg.Theta()
            inTrackPhi = seg.Phi()
            for jtrack in range(ntracks):
              if vtx.Zpos(jtrack)==1:
                nout+=1
                track = vtx.GetTrack(jtrack)
                temp1 = track.TX()*inTrackTX+track.TY()*inTrackTY+1
                temp2 = r.TMath.Sqrt(track.TX()**2+track.TY()**2+1)*r.TMath.Sqrt(inTrackTX**2+inTrackTY**2+1)
                trackDist = float(temp1)/float(temp2)
                # trackDist = r.TMath.Sqrt((track.TX()-inTrackTX)**2+(track.TY()-inTrackTY)**2) #prodotto scalare
                trackDistList.append(trackDist)
                trackDistListX.append(track.TX()-inTrackTX)
                trackDistListY.append(track.TY()-inTrackTY)
                trackDistListTheta.append(track.Theta()-inTrackTheta)
                trackDistListPhi.append(track.Phi()-inTrackPhi)
            nin = ntracks - nout
            outputTree.Fill(vtx.ID(), flag, ntracks, nin, nout, 0, 0, 0)
            if nout>nin:
              #if (nout-nin==1) and np.min(trackDistList)>0.92: continue
              h_cos.Fill(np.min(trackDistList))
              minIndex = np.where(trackDistList == np.min(trackDistList))[0]
              h_tx.Fill(trackDistListX[minIndex[0]])
              h_ty.Fill(trackDistListY[minIndex[0]])
              h_txy.Fill(trackDistListX[minIndex[0]],trackDistListY[minIndex[0]])
              h_theta.Fill(trackDistListTheta[minIndex[0]])
              h_phi.Fill(trackDistListPhi[minIndex[0]])
              h_tp.Fill(trackDistListTheta[minIndex[0]],trackDistListPhi[minIndex[0]])
      #flag 0 study
      elif flag == 0 or flag == 3:
        h_ntracks.Fill(ntracks)
        nplList = []
        nsegList = []
        ffList = []
        ipList = []
        for itrack in range(ntracks):
          track = vtx.GetTrack(itrack)
          h_imp.Fill(vtx.GetVTa(itrack).Imp())
          ipList.append(vtx.GetVTa(itrack).Imp())
          nseg = track.N()
          npl = track.Npl()
          nfirst = track.GetSegmentFirst().Plate()
          nava = 60 - nfirst + 1
          FF = float(nseg)/float(nava)
          nplList.append(npl)
          nsegList.append(nseg)
          ffList.append(FF)
          h_nseg.Fill(nseg)
          h_npl.Fill(npl)
          h_ff.Fill(FF)
        h_xy.Fill(vtx.VX(), vtx.VY())
        h_z.Fill(vtx.VZ())
        h_prob.Fill(np.mean(ipList), vtx.V().prob())
        h_n0.Fill(ntracks)
        h_nseg_avg.Fill(np.mean(nsegList))
        h_npl_avg.Fill(np.mean(nplList))
        h_ff_avg.Fill(np.mean(ffList))
        outputTree.Fill(vtx.ID(), flag, ntracks, 0, ntracks, np.mean(nsegList), np.mean(nplList), np.mean(ffList))

#ntracks and flag
c1.cd(1).SetLogy()
h_n.Draw()
c1.cd(2)
h_flag.Draw()
c1.Update()

#ntracks after soft cut
cn.cd().SetLogy()
h_ntracks.Draw()

#vz with cut lines
cz.cd()
h_vz.Draw()
cz.Update()
lz_min = r.TLine(-29627, cz.GetUymin(), -29627, cz.GetUymax())
lz_max = r.TLine(0 , cz.GetUymin(), 0, cz.GetUymax())
lz_min.SetLineColor(r.kRed)
lz_max.SetLineColor(r.kRed)
lz_min.SetLineWidth(2)
lz_max.SetLineWidth(2)
lz_min.Draw("same")
lz_max.Draw("same")
cz.Update()

#flag1 dot protuct
c_cos.cd()
h_cos.Draw()
c_cos.Update()
#c_cos.Print('cos_cut3.png')

#flag1 angular difference
cf1.cd(1)
h_tx.Draw()
cf1.cd(2)
h_ty.Draw()
cf1.cd(3)
h_txy.Draw("COLZ")
cf1.cd(4)
h_theta.Draw()
cf1.cd(5)
h_phi.Draw()
cf1.cd(6)
h_tp.Draw("COLZ")
cf1.Update()
#cf1.Print('angle_cut3.png')

#flag0 stats
cf0.cd(1).SetLogy()
h_n0.Draw()
cf0.cd(2).SetLogy()
h_nseg.Draw()
cf0.cd(3).SetLogy()
h_npl.Draw()
cf0.cd(4).SetLogy()
h_ff.Draw()
cf0.Update()

#flag0 avg stats
cf0avg.cd(1).SetLogy()
h_n0.Draw()
cf0avg.cd(2).SetLogy()
h_nseg_avg.Draw()
cf0avg.cd(3).SetLogy()
h_npl_avg.Draw()
cf0avg.cd(4).SetLogy()
h_ff_avg.Draw()
cf0avg.Update()

#impact parameter
c_imp.cd()
h_imp.Draw()

#vertex probability
c_prob.cd()
h_prob.Draw("COLZ")

#vertex position
c_xyz.cd(1)
h_xy.Draw('COLZ')
c_xyz.cd(2)
h_z.Draw()

#write output file
outputFile.cd()
outputTree.Write()
outputFile.Close()
