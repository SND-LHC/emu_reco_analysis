'''example on how to read a vertex file by using ReadVertexTree'''

import ROOT as r
import fedrarootlogon

r.gEDBDEBUGLEVEL = 0 #let us not care about FEDRA messages for now

fileprefix = ""
filepath = "/home/scanner/sndlhc/RUN0/b000031/"
#initialization
dproc = r.EdbDataProc()
gAli = dproc.PVR()
scancond = r.EdbScanCond()
gAli.SetScanCond(scancond)

#setting parameters as in vertexing.C
vertexrec = r.EdbVertexRec()
vertexrec.SetPVRec(gAli)
vertexrec.eDZmax=3000.
vertexrec.eProbMin=0.0001
vertexrec.eImpMax=15.
vertexrec.eUseMom=False
vertexrec.eUseSegPar=True
vertexrec.eQualityMode=0

proc = r.EdbDataProc()

#reading vertices, building EdbVertex objects
print("start first loop to prepare vertices")

dproc.ReadVertexTree(vertexrec, fileprefix+filepath+"vertextree.root", "1")

vertices = gAli.eVTX
print("prepared vertices: ",len(vertices),"starting loop ")
#now we can loop in vertices as usual in Python
for vtx in vertices:
 ntracks = vtx.N()
 #loop in tracks
 for itrack in range(ntracks):
  track = vtx.GetTrack(itrack)

print("end of loop")

