//fitting two gaussian peaks for comparison with electronic detectors (Antonio, 24 November 2022)
void fitangles(){
 //getting file and histograms
 TFile *inputfile = TFile::Open("plots_localtracking.root");
 TH1D *htx = (TH1D*) inputfile->Get("htx");
 
 const int myrebin = 5;
 htx->Rebin(myrebin);

 TCanvas *ctx = new TCanvas();
 htx->Draw();
 htx->GetXaxis()->SetRangeUser(0,0.04);

 //fitting to functions
 //htx->Fit("gaus","","",0,0.01);

// htx->Fit("gaus","+","",0.01,0.015);

 TF1 *total = new TF1("total","gaus(0)+gaus(3)",0.,0.015);
 //first gauss params
 total->SetParameter(0, 2.55531e+02);
 total->SetParameter(1, 5.64731e-03);
 total->SetParameter(2, 3.02244e-03 );

 //second gauss params
 total->SetParameter(3, 9.95001e+01);
 total->SetParameter(4, 1.1e-02);
 total->SetParameter(5, 4.09966e-03);

 htx->Fit(total,"RB");

 
 //now adding ty
 TCanvas *cty = new TCanvas();
 TH1D *hty = (TH1D*) inputfile->Get("hty");
 hty->Rebin(myrebin);
 hty->Draw();
 hty->GetXaxis()->SetRangeUser(-0.01,0.03);

 hty->Fit("gaus","","",0.005,0.015);


}
