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

path = 	'/home/scanner/sndlhc/RUN0/b000131/vertexfiles/upstream31'
paths = [path+'/first', path+'/second', path+'/third', path+'/fourth']
vertexFiles = [[],[],[],[]]

r.gROOT.SetBatch(False)
r.gStyle.SetOptStat(0)

#ntracks and flag
c1 = r.TCanvas('c1', 'c1', 1400, 600)
c1.Divide(2,1)
h_n = r.TH1D('h_n','multiplicity;multiplicity', 50, 0, 50)
h_flag = r.TH1D('h_flag','flag;flag', 6, 0, 6)

#ntracks after soft cut
cn = r.TCanvas('cn', 'ntracks', 800, 600)
h_ntracks = r.TH1D('h_ntracks','ntracks;ntracks', 47, 3, 50)
h_flag_cut = r.TH1D('h_flag_cut','flag;flag', 6, 0, 6)

#vz
cz = r.TCanvas('cz', 'vtx_zposition', 800, 600)
h_vz = r.TH1D('h_vz','vz;vz[um]', 200, -45000, 5000)

'''
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
'''
#flag0 stats
cf0 = r.TCanvas('cf0', 'flag 0 stats', 1200, 1000)
cf0.Divide(2, 2)
h_n0 = r.TH1D('h_n0', 'multiplicity;multiplicity', 47, 3, 50)
h_nseg = r.TH1D('h_nseg', 'nseg;nseg', 32, 0, 32)
h_npl = r.TH1D('h_npl', 'npl;npl', 32, 0, 32)
h_ff = r.TH1D('h_ff', 'fillfactor;FF', 22, 0, 1.1)

#flag0 avg stats
cf0avg = r.TCanvas('cf0avg', 'cf0avg', 1200, 1000)
cf0avg.Divide(2, 2)
h_nseg_avg = r.TH1D('h_nseg_avg', 'nseg_avg;nseg', 32, 0, 32)
h_npl_avg = r.TH1D('h_npl_avg', 'npl_avg;npl', 32, 0, 32)
h_ff_avg = r.TH1D('h_ff_avg', 'fillfactor;FF', 22, 0, 1.1)

#impact parameter
c_imp = r.TCanvas('c_imp', 'impact parameter', 1400, 600)
c_imp.Divide(2,1)
h_imp = r.TH1D('h_imp', 'impact parameter;ip[um]', 50, 0, 150)
h_imp_avg = r.TH1D('h_imp_sim', 'impact parameter;ip[um]', 50, 0, 150)

#vertex probability
c_prob = r.TCanvas('c_prob', 'vtx prob', 800, 600)
h_prob = r.TH1D('h_prob', 'vtx prob;P', 33, 0, 1.1)

#vertex position
c_xyz = r.TCanvas('c_xyz', 'vtx_position', 600, 1000)
c_xyz.Divide(1,2)
h_xy = r.TH2D('h_xy','vxy;vx[um];vy[um]',100, 0, 200000, 100, 0, 200000)
h_z = r.TH1D('h_z','vz;vz[um]', 35, -38000, 0)

#save vertices in root file
#outputFile = r.TFile("vertex_selection_newsigma.root","RECREATE")	
'''
outputTree = r.TTree('vtx', 'Tree of selected vertices')
quad = array('i', [0])
subquad = array('i', [0])
vID = array('i', [0])
flag = array('i', [0])
n = array('i', [0])
nin = array('i', [0])
nout = array('i', [0])
nseg = array('f', [0])
npl = array('f', [0])
nfirst = array('i', [0])
ff = array('f', [0])
meanIP = array('f', [0])
prob = array('f', [0])
outputTree.Branch('quad', quad, 'quad/I')
outputTree.Branch('subquad', subquad, 'subquad/I')
outputTree.Branch('vID', vID, 'vID/I')
outputTree.Branch('flag', flag, 'flag/I')
outputTree.Branch('n', n, 'n/I')
outputTree.Branch('nin', nin, 'nin/I')
outputTree.Branch('nout', nout, 'nout/I')
outputTree.Branch('nseg', nseg, 'nseg/F')
outputTree.Branch('npl', npl, 'npl/F')
outputTree.Branch('nfirst', nfirst, 'nfirst/I')
outputTree.Branch('ff', ff, 'ff/F')
outputTree.Branch('meanIP', meanIP, 'meanIP/F')
outputTree.Branch('prob', prob, 'prob/F')
'''
#outputTree = r.TNtuple("vtx","Tree of vertices","quadrant:subquadrant:vID:flag:n:nin:nout:nseg:npl:nfirst:ff:meanIP:prob")

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
      #if not (vx > pos_sim_x[0] and vx < pos_sim_x[1] and vy > pos_sim_y[0] and vy < pos_sim_y[1]): continue
      h_n.Fill(ntracks)
      h_vz.Fill(vz)
      h_flag.Fill(flag)
      #soft cuts
      if vz < pos[quad][subquad][4]: continue
      if vz > pos[quad][subquad][5]: continue
      h_flag_cut.Fill(flag)
      if flag == 0 or flag == 3:
        h_ntracks.Fill(ntracks)
      if ntracks < 3: continue
      #flag 1 study
      '''
      if flag == 1 or flag == 4:
        #h_ntracks.Fill(ntracks)
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
            nplList = []
            nsegList = []
            ffList = []
            ipList = []
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
                ipList.append(vtx.GetVTa(itrack).Imp())
                nseg = track.N()
                npl = track.Npl()
                nfirst = track.GetSegmentFirst().Plate()
                nava = 34 - nfirst + 1
                FF = float(nseg)/float(nava)
                nplList.append(npl)
                nsegList.append(nseg)
                ffList.append(FF)
            nin = ntracks - nout
            if nout>nin:
              if (nout-nin==1) and np.min(trackDistList)>0.92: continue
              #outputTree.Fill(quad, subquad, vtx.ID(), flag, ntracks, nin, nout, np.mean(nsegList), np.mean(nplList), nfirst, np.mean(ffList), np.mean(ipList), vtx.V().prob())
              h_cos.Fill(np.min(trackDistList))
              minIndex = np.where(trackDistList == np.min(trackDistList))[0]
              h_tx.Fill(trackDistListX[minIndex[0]])
              h_ty.Fill(trackDistListY[minIndex[0]])
              h_txy.Fill(trackDistListX[minIndex[0]],trackDistListY[minIndex[0]])
              h_theta.Fill(trackDistListTheta[minIndex[0]])
              h_phi.Fill(trackDistListPhi[minIndex[0]])
              h_tp.Fill(trackDistListTheta[minIndex[0]],trackDistListPhi[minIndex[0]])
        if nin==1:
          if (nout-nin==1) and np.min(trackDistList)>0.92: continue
          #outputTree.Fill(quad, subquad, vtx.ID(), flag, ntracks, nin, nout, np.mean(nsegList), np.mean(nplList), nfirst, np.mean(ffList), np.mean(ipList), vtx.V().prob())
      '''
      #flag 0 study
      if flag == 0 or flag == 3:
        nplList = []
        nsegList = []
        ffList = []
        ipList = []
        for itrack in range(ntracks):
          track = vtx.GetTrack(itrack)
          #h_imp.Fill(vtx.CheckImp(track)) #redo imp check, instead of using vta one
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
          h_nseg.Fill(nseg)
          h_npl.Fill(npl)
          h_ff.Fill(FF)
        h_xy.Fill(vtx.VX(), vtx.VY())
        h_z.Fill(vtx.VZ())
        h_prob.Fill(vtx.V().prob())
        h_n0.Fill(ntracks)
        h_nseg_avg.Fill(np.mean(nsegList))
        h_npl_avg.Fill(np.mean(nplList))
        h_ff_avg.Fill(np.mean(ffList))
        h_imp_avg.Fill(np.mean(ipList))
        #outputTree.Fill(quad, subquad, vtx.ID(), flag, ntracks, 0, ntracks, np.mean(nsegList), np.mean(nplList), nfirst, np.mean(ffList), np.mean(ipList), vtx.V().prob())

print("tot ", h_n.GetEntries())
print("vz ", h_flag_cut.GetEntries())
print("flag", h_ntracks.GetEntries())
print("mult", h_n0.GetEntries())
#ntracks and flag
c1.cd(1).SetLogy()
h_n.Draw()
c1.cd(2)
h_flag.Draw()
c1.cd(1).Update()
ln_min = r.TLine(3, c1.cd(1).GetUymin(), 3, c1.cd(1).GetUymax())
ln_min.SetLineColor(r.kRed)
ln_min.SetLineWidth(2)
ln_min.Draw("same")
c1.Update()
c1.Print('n_flag.png')
'''
#ntracks after soft cut
cn.cd().SetLogy()
h_ntracks.Draw()
cn.Update()
cn.Print('n_soft_mc.png')
'''
#vz with cut lines
cz.cd()
h_vz.Draw()
cz.Update()
lz_min = r.TLine(-37223.46, cz.GetUymin(), -37223.46, cz.GetUymax())
lz_max = r.TLine(0 , cz.GetUymin(), 0, cz.GetUymax())
lz_min.SetLineColor(r.kRed)
lz_max.SetLineColor(r.kRed)
lz_min.SetLineWidth(2)
lz_max.SetLineWidth(2)
lz_min.Draw("same")
lz_max.Draw("same")
cz.Update()
cz.Print('vz.png')
'''
#flag1 dot protuct
c_cos.cd()
h_cos.Draw()
c_cos.Update()
c_cos.Print('cos_cut.png')

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
cf1.Print('angle_cut.png')
'''
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
cf0.Print('flag0.png')

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
cf0avg.Print('flag0_avg.png')

#vertex probability
c_prob.cd()
h_prob.Draw()
c_prob.Update()
c_prob.Print('prob.png')

#impact parameter
c_imp.cd(1)
h_imp.Draw()
c_imp.cd(2)
h_imp_avg.Draw()
c_imp.Update()
c_imp.Print('ip.png')
'''
#vertex position
c_xyz.cd(1)
h_xy.Draw('COLZ')
c_xyz.cd(2)
h_z.Draw()
c_xyz.Update()
c_xyz.Print('vtx_xyz.png')

#write output file
outputFile.cd()
outputTree.Write()
outputFile.Close()
'''
