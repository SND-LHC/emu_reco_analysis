import ROOT as r
import fedrarootlogon


run0path = "/home/scanner/sndlhc/TEST_POSD/b000001/"

couples = r.EdbCouplesTree()
couples.InitCouplesTree("couples", run0path+"p001/1.1.0.0.cp.root","READ")
couples.eCut = "eCHI2P<2.4&&s.eW>20&&eN1<=1&&eN2<=1&&s1.eFlag>=0&&s2.eFlag>=0"

lst = couples.InitCutList()
nlst = lst.GetN()

print("{} coppie".format(nlst))

#couplesfile = r.TFile.Open(run0path+"p001/1.1.0.0.cp.root")
#couples = couplesfile.Get("couples")
#couples.GetEntries()

#TX = tangente all'angolo (theta)xz = px/pz (=> angolo tra la proiezione del vettore nel piano xz e l'asse x?)

#TY vs TX
H_tyx = r.TH2F("H_tyx", "TY vs TX", 100, -10, 10, 100, -10, 10)

#TX
H_tx = r.TH1F("H_tx", "TX", 100, -6, 6)

#TY
H_ty = r.TH1F("H_ty", "TY", 100, -6, 6)

#POSIZIONE IN XY
H_xy = r.TH2F("H_xy", "coppie nel piano xy", 100, 0, 200, 100, 0, 200)

for ilst in range(nlst):
	entr = lst.GetEntry(ilst)
	couples.GetEntry(entr)

	seg = couples.eS
	#print(seg.X(), seg.Y(), seg.TX(), seg.TY())
	
	#TY vs TX
	tx = seg.TX()
	ty = seg.TY()
	
	H_tyx.Fill(ty, tx)	

	#TX
	tx = seg.TX()
	
	H_tx.Fill(tx)	
	
	#TY
	ty = seg.TY()
	
	H_ty.Fill(ty)	
	
	#XY
	x = seg.X()/1000
	y = seg.Y()/1000

	H_xy.Fill(x, y)


#TY vs TX
H_tyx.GetXaxis().SetTitle("TY")
H_tyx.GetYaxis().SetTitle("TX")
c1 = r.TCanvas("c1", "TY vs TX", 800, 800)
H_tyx.Draw()

#TX
H_tx.GetXaxis().SetTitle("TX")
c2 = r.TCanvas("c2", "TX", 800, 800)
H_tx.Draw()

#TY
H_ty.GetXaxis().SetTitle("TY")
c3 = r.TCanvas("c3", "TY", 800, 800)
H_ty.Draw()

#XY
H_xy.GetXaxis().SetTitle("#mu m")
H_xy.GetYaxis().SetTitle("#mu m")
c4 = r.TCanvas("c4", "coppie piano xy", 800, 800)
H_xy.Draw()
		
