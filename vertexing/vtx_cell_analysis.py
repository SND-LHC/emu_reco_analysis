import ROOT as r
import numpy as np
import fedrarootlogon
from time import time
import os.path

brickID = 21
from_plate = 57
to_plate = 1
ncells = 19
ncellmin = 0
ncellmax= 190000
cellsize= (ncellmax-ncellmin)/ncells
overlap_fraction = 0.25
zmin = -75262

path = '/eos/experiment/sndlhc/emulsionData/2022/emureco_Napoli/RUN1/b{:06d}/trackfiles/rootfiles/{}_{}'.format(brickID, from_plate, to_plate)
out_dir = '/eos/experiment/sndlhc/emulsionData/2022/emureco_Napoli/RUN1/b{:06d}/vertexfiles/{}_{}'.format(brickID, from_plate, to_plate)

#histo setup
h_n = r.TH1D('n','multiplicity;multiplicity', 50, 0, 50)
h_flag = r.TH1D('flag','flag;flag', 6, 0, 6)
h_vz = r.TH1D('vz','vz;vz[um]', 400, -80000, 0)
h_vxy = r.TH2D('vxvy', 'vxvy;vx[um];vy[um]', 190, 0, 190000, 190, 190000)
h_n0 = r.TH1D('ntracks', 'multiplicity;multiplicity', 48, 2.5, 50.5)
h_nseg = r.TH1D('nseg', 'nseg;nseg', 32, 0, 32)
h_npl = r.TH1D('npl', 'npl;npl', 32, 0, 32)
h_ff = r.TH1D('ff', 'fill factor;FF', 22, -0.05, 1.05)
h_imp = r.TH1D('ip', 'impact parameter;ip[um]', 50, -0.5, 150)
h_prob = r.TH1D('vtx_prob', 'probability;prob', 30, 0, 1.1)
h_maxape = r.TH1D('max_aperure', 'max aperture;max_ape', 200, 0, 200)

#save vertices in root file
outputFile = r.TFile(out_dir+"/vertex_selection.root","RECREATE")	
# outputTree = r.TNtuple("vtx","Tree of vertices","cell:vID:vx:vy:vz:flag:n:nseg:npl:nfirst:ff:meanIP:prob:maxape")
outputTree = r.TNtuple("vtx","Tree of vertices","cell:vID:n:ff:meanIP:prob:maxape")

for icell in range(362):
    start_time = time()
    xbin=((icell // ncells)) + 1
    ybin=((icell % ncells)) + 1
    cell_path = path+'/cell_{}'.format(ybin)
    #reading vertices
    vtx_file = cell_path+'/vertextree_{}_{}.root'.format(xbin, ybin)
    if not os.path.isfile(vtx_file): 
        #print("file does not exist ",vtx_file)
        continue
    print("opening file: ",vtx_file)
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
    dproc.ReadVertexTree(vertexrec, vtx_file, "1")
    vertices = gAli.eVTX
    for vtx in vertices:
        vx = vtx.VX()
        vy = vtx.VY()
        vz = vtx.VZ()
        flag = vtx.Flag()
        ntracks = vtx.N()
        #cell cuts
        if vx < (xbin-1-overlap_fraction/2)*cellsize: continue
        if vx > (xbin+overlap_fraction/2)*cellsize: continue
        if vy < (ybin-1-overlap_fraction/2)*cellsize: continue
        if vy > (ybin+overlap_fraction/2)*cellsize: continue
        h_n.Fill(ntracks)
        h_flag.Fill(flag)
        h_vz.Fill(vz)
        #soft cuts
        if ntracks < 3: continue
        if vz < zmin: continue
        if vz > 0: continue
        h_vxy.Fill(vx, vy)
        #flag 0 study
        if flag == 0 or flag == 3:
            h_n0.Fill(ntracks)
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
                nava = from_plate - to_plate - nfirst + 2
                FF = float(nseg)/float(nava)
                nsegList.append(nseg)
                nplList.append(npl)
                ffList.append(FF)
                h_nseg.Fill(nseg)
                h_npl.Fill(npl)
                h_ff.Fill(FF)
            h_prob.Fill(vtx.V().prob())
            h_maxape.Fill(vtx.MaxAperture())
            # outputTree.Fill(icell, vtx.ID(), vx, vy, vz, flag, ntracks, np.mean(nsegList), np.mean(nplList), nfirst, np.mean(ffList), np.mean(ipList), vtx.V().prob(), vtx.MaxAperture())
            outputTree.Fill(icell, vtx.ID(), ntracks, np.mean(ffList), np.mean(ipList), vtx.V().prob(), vtx.MaxAperture())
    del gAli
    print(time()-start_time)


#write output files
outputFile.cd()
outputTree.Write()
outputFile.Close()

histoFile = r.TFile(out_dir+"/hist_out.root", "RECREATE")
h_n.Write()
h_flag.Write()
h_vz.Write()
h_n0.Write()
h_nseg.Write()
h_npl.Write()
h_ff.Write()
h_imp.Write()
h_prob.Write()
h_maxape.Write()
histoFile.Write()
histoFile.Close()

end_time = time()-start_time
print("TOTAL ELAPSED TIME ", end_time)
