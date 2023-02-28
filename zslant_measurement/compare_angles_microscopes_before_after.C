void compare_angles_microscopes_before_after(){
 gStyle->SetOptStat(0);
 //opening file, get trees of couples
 TFile *mic3_file = TFile::Open("/home/scanner/sndlhc/RUN0/b000231/p808/231.808.0.0.cp.root");
 TFile *mic2_file = TFile::Open("/home/scanner/sndlhc/RUN0/b000231/p908/231.908.0.0.cp.root");

 TTree *mic3_couples = (TTree*) mic3_file->Get("couples");
 TTree *mic2_couples = (TTree*) mic2_file->Get("couples");
 //draw TX histograms
 TCanvas *c0 = new TCanvas(); 

 mic3_couples->Draw("s.eTX>>hmic3_TX(150,-0.15,0.15)","eN1==1&&eN2==1");
 mic2_couples->Draw("s.eTX>>hmic2_TX(150,-0.15,0.15)","eN1==1&&eN2==1");

 TH1D *hmic3_TX = (TH1D*) gDirectory->FindObject("hmic3_TX");
 TH1D *hmic2_TX = (TH1D*) gDirectory->FindObject("hmic2_TX");

 hmic3_TX->SetLineColor(kRed);
 hmic2_TX->Draw();
 hmic2_TX->SetTitle(";TX");
 hmic3_TX->Draw("same");

 //applying offset correction
 TCanvas *c1 = new TCanvas(); 

 mic3_couples->Draw("s.eTX-(0.00593301/2.)>>hmic3_TX_corrected(150,-0.15,0.15)","eN1==1&&eN2==1");
 mic2_couples->Draw("s.eTX- (0.0125672/2.)>>hmic2_TX_corrected(150,-0.15,0.15)","eN1==1&&eN2==1");

 TH1D *hmic3_TX_corrected = (TH1D*) gDirectory->FindObject("hmic3_TX_corrected");
 TH1D *hmic2_TX_corrected = (TH1D*) gDirectory->FindObject("hmic2_TX_corrected");

 hmic3_TX_corrected->SetLineColor(kRed);
 hmic2_TX_corrected->SetTitle(";TX");
 hmic2_TX_corrected->Draw();
 hmic3_TX_corrected->Draw("same");

 //TY plots
 TCanvas *c2 = new TCanvas(); 

 mic3_couples->Draw("s.eTY>>hmic3_TY(150,-0.15,0.15)","eN1==1&&eN2==1");
 mic2_couples->Draw("s.eTY>>hmic2_TY(150,-0.15,0.15)","eN1==1&&eN2==1");

 TH1D *hmic3_TY = (TH1D*) gDirectory->FindObject("hmic3_TY");
 TH1D *hmic2_TY = (TH1D*) gDirectory->FindObject("hmic2_TY");

 hmic3_TY->SetLineColor(kRed);
 hmic3_TY->SetTitle(";TY");
 hmic3_TY->Draw();
 hmic2_TY->Draw("same");

 //applying offset correction
 TCanvas *c3 = new TCanvas(); 

 mic3_couples->Draw("s.eTY-(0.0105279/2.)>>hmic3_TY_corrected(150,-0.15,0.15)","eN1==1&&eN2==1");
 mic2_couples->Draw("s.eTY- (0.0441632/2.)>>hmic2_TY_corrected(150,-0.15,0.15)","eN1==1&&eN2==1");

 TH1D *hmic3_TY_corrected = (TH1D*) gDirectory->FindObject("hmic3_TY_corrected");
 TH1D *hmic2_TY_corrected = (TH1D*) gDirectory->FindObject("hmic2_TY_corrected");

 hmic3_TY_corrected->SetLineColor(kRed);
 hmic3_TY_corrected->SetTitle(";TY");
 hmic3_TY_corrected->Draw();
 hmic2_TY_corrected->Draw("same"); 
}
