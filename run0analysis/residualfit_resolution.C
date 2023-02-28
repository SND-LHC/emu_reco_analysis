//Fitting with ROOFIT file provided by Chris
void residualfit_resolution()
{
  TFile *inputfile = TFile::Open("checktrackslinearfits.root"); ///home/scanner/sndlhc/SNDCosmics/checktrackslinearfits.root
  TTree *inputtree = (TTree*) inputfile->Get("restree");

  RooRealVar dxr("dx", "dx[#mum]", -60, 60.);
  RooDataSet data_x("data_x","dataset with dxr",inputtree,dxr) ;

  //adding binning, try binned
  //dxr.setBins(30);
  //RooDataHist binnedData("binnedData", "matcheddata",
  //RooArgList(dxr), matcheddata);

  //define parameters and PDFs 
  RooRealVar mu_x("mu_x", "average", 0, -1000, 1000);
  RooRealVar sigma_x("sigma_x", "sigma",40, 0, 100);
  RooGaussian gauss_x("gauss_x","gaussian PDF", dxr, mu_x, sigma_x);

  RooRealVar mu2_x("mu2_x", "average of second gaussian", 0, -1000, 1000);
  RooRealVar sigma2_x("sigma2_x", "sigma of second gaussian", 20, 0, 100);
  RooGaussian gauss2_x("gauss2_x","Second gaussian PDF", dxr, mu2_x, sigma2_x);

  //polynomial, max 1 degree
  //RooRealVar slope("slope", "slope",20,-10,10);
  //RooPolynomial polynom("polynom","Pol1",dxr,RooArgList(slope));

  RooRealVar yield_x("yield_x", "signal yield", 1000, 0, 10000);
  RooRealVar yield2_x("yield2_x", "signal yield", 1000, 0, 10000);
  RooRealVar b_x("b_x", "background yield", 100, 0, 200);
  RooAddPdf sum_x("sum_x","Total distribution",RooArgList(gauss_x,gauss2_x),RooArgList(yield_x,yield2_x));

   RooFitResult *r_x = sum_x.fitTo(data_x, RooFit::Save());
  //RooFitResult *r = sum.fitTo(binnedData, RooFit::Save());
  //summary printing
  r_x->Print();
  //TH2* hcorr = r->correlationHist() ;
  //plot the result
  RooPlot * xFrame = dxr.frame() ;
  //binnedData.plotOn(xFrame) ;
  data_x.plotOn(xFrame,RooFit::Name("matcheddatax"));
  sum_x.plotOn(xFrame,RooFit::Name("totalx")) ;
  sum_x.plotOn(xFrame, RooFit::Components(gauss2_x), RooFit::LineStyle(kDashed), RooFit::Name("gauss2componentx")) ;
  sum_x.plotOn(xFrame, RooFit::Components(gauss_x), RooFit::LineStyle(kDotted), RooFit::Name("gausscomponentx")) ;

  sum_x.paramOn(xFrame,RooFit::Layout(0.55)) ;
  TCanvas c_x;
  xFrame->Draw();
  auto reducedchi2_x = xFrame->chiSquare("totalx","matcheddatax",6);

  //adding legend
  TLegend *leg_x = new TLegend(0.4,0.6,0.89,0.89);
  leg_x->AddEntry("totalx",Form("Total distribution with chi2/ndf = %f",reducedchi2_x));
  leg_x->AddEntry("gausscomponentx","First gaussian");
  leg_x->AddEntry("gauss2componentx","Second gaussian");
  leg_x->Draw();

  c_x.SaveAs("polfit_x.root");

  //redoing on y side

  RooRealVar dyr("dy", "dy[#mum]", -60, 60);
  RooDataSet data_y("data_y","dataset with dy",inputtree,dyr) ;

  //define parameters and PDFs 
  RooRealVar mu_y("mu_y", "average", 0, -1000, 1000);
  RooRealVar sigma_y("sigma_y", "sigma",40, 0, 100);
  RooGaussian gauss_y("gauss_y","gaussian PDF", dyr, mu_y, sigma_y);

  RooRealVar mu2_y("mu2_y", "average of second gaussian", 0, -1000, 1000);
  RooRealVar sigma2_y("sigma2_y", "sigma of second gaussian", 20, 0, 100);
  RooGaussian gauss2_y("gauss2_y","Second gaussian PDF", dyr, mu2_y, sigma2_y);

  RooRealVar yield_y("yield_y", "signal yield", 1000, 0, 10000);
  RooRealVar yield2_y("yield2_y", "signal yield", 1000, 0, 10000);
  RooRealVar b_y("b_y", "background yield", 100, 0, 200);

  RooAddPdf sum_y("sum_y","Total distribution",RooArgList(gauss_y,gauss2_y),RooArgList(yield_y,yield2_y));

  RooFitResult *r_y = sum_y.fitTo(data_y, RooFit::Save());
  //RooFitResult *r = sum.fitTo(binnedData, RooFit::Save());
  //summary printing
  r_y->Print();
  //TH2* hcorr = r->correlationHist() ;
  //plot the result
  RooPlot * yFrame = dyr.frame() ;
  //binnedData.plotOn(xFrame) ;
  data_y.plotOn(yFrame,RooFit::Name("matcheddatay"));
  sum_y.plotOn(yFrame,RooFit::Name("totaly")) ;
  sum_y.plotOn(yFrame, RooFit::Components(gauss2_y), RooFit::LineStyle(kDashed),RooFit::Name("gauss2componenty"));
  sum_y.plotOn(yFrame, RooFit::Components(gauss_y), RooFit::LineStyle(kDotted),RooFit::Name("gausscomponenty")) ;

  sum_y.paramOn(yFrame,RooFit::Layout(0.55)) ;
  TCanvas c_y;
  yFrame->Draw();
  auto reducedchi2_y = yFrame->chiSquare("totaly","matcheddatay",6);
  //adding legend
  TLegend *leg_y = new TLegend(0.4,0.6,0.89,0.89);
  leg_y->AddEntry("totaly",Form("Total distribution with chi2/ndf = %f",reducedchi2_y));
  leg_y->AddEntry("gausscomponenty","First gaussian");
  leg_y->AddEntry("gauss2componenty","Second gaussian");
  leg_y->Draw();

  c_y.SaveAs("polfit_y.root");
}

void anglefit(){
  
  gROOT->SetStyle("Plain");
  //define variables and importing data
  TFile *inputfile = TFile::Open("checktrackslinearfits.root"); ///home/scanner/sndlhc/SNDCosmics/checktrackslinearfits.root
  TTree *inputtree = (TTree*) inputfile->Get("restree");

  RooRealVar dtxr("dtx", "dtx", -0.06, 0.06);
  RooDataSet data_x("matcheddata_x","dataset with match_dtxr",inputtree,dtxr) ;

  //adding binning, try binned
  //dtxr.setBins(30);
  //RooDataHist binnedData("binnedData", "matcheddata",
  //RooArgList(dtxr), matcheddata);

  //define parameters and PDFs 
  RooRealVar mu_x("mu_x", "average", 0, -1, 1);
  RooRealVar sigma_x("sigma_x", "sigma", 0.007, 0, 1);
  RooGaussian gauss_x("gauss_x","gaussian PDF", dtxr, mu_x, sigma_x);

  //second gaussian
  RooRealVar mu2_x("mu2_x", "average of second gaussian", 0, -1, 1);
  RooRealVar sigma2_x("sigma2_x", "sigma of second gaussian", 0.009, 0, 1);
  RooGaussian gauss2_x("gauss2_x","Second gaussian PDF", dtxr, mu2_x, sigma2_x);

  RooRealVar yield_signal_x("yield_signal_x", "signal yield", 1000, 0, 100000);
  RooRealVar yield_background_x("yield_background_x", "background yield", 1000, 0, 100000);
  RooAddPdf sum_x("sum_x","Total distribution",RooArgList(gauss_x,gauss2_x),RooArgList(yield_signal_x,yield_background_x));

  RooFitResult *r_x = sum_x.fitTo(data_x, RooFit::Save());
  //RooFitResult *r = sum.fitTo(binnedData, RooFit::Save());
  //summary printing
  r_x->Print();
  //TH2* hcorr = r->correlationHist() ;
  //plot the result
  RooPlot * xFrame = dtxr.frame() ;
  //binnedData.plotOn(xFrame) ;
  data_x.plotOn(xFrame,RooFit::Name("data_x"));
  sum_x.plotOn(xFrame,RooFit::Name("total_x")) ;
  sum_x.plotOn(xFrame, RooFit::Components(gauss_x), RooFit::LineStyle(kDashed),RooFit::Name("gaussx")) ;
  sum_x.plotOn(xFrame, RooFit::Components(gauss2_x), RooFit::LineStyle(kDotted),RooFit::Name("gauss2x")) ;
  sum_x.paramOn(xFrame,RooFit::Layout(0.55)) ;
  TCanvas c_x;
  xFrame->Draw();
  auto reducedchi2_x = xFrame->chiSquare("total_x","data_x",6);
  //c.SaveAs("polfit.pdf");
  //adding legend
  TLegend *leg_x = new TLegend(0.4,0.6,0.89,0.89);
  leg_x->AddEntry("total_x",Form("Total distribution with chi2/ndf = %f",reducedchi2_x));
  leg_x->AddEntry("gaussx","First Gaussian");
  leg_x->AddEntry("gauss2x","Second Gaussian");
  leg_x->Draw();

  c_x.SaveAs("gausfit_angx.root");
  //TCanvas cfitresults;
  //hcorr->Draw("colz");

  //***STARTING SECTION OF ANGLE Y********

  RooRealVar dtyr("dty", "dty", -0.06, 0.06);
  RooDataSet data_y("matcheddata_y","dataset with match_dtyr",inputtree,dtyr) ;

  //define parameters and PDFs 
  RooRealVar mu_y("mu_y", "average", 0, -1, 1);
  RooRealVar sigma_y("sigma_y", "sigma", 0.007, 0, 1);
  RooGaussian gauss_y("gauss_y","gaussian PDF", dtyr, mu_y, sigma_y);

   //second gaussian
  RooRealVar mu2_y("mu2_y", "average of second gaussian", 0, -1, 1);
  RooRealVar sigma2_y("sigma2_y", "sigma of second gaussian", 0.009, 0, 1);
  RooGaussian gauss2_y("gauss2_y","Second gaussian PDF", dtyr, mu2_y, sigma2_y);

  RooRealVar yield_signal_y("yield_signal_y", "signal yield", 1000, 0, 100000);
  RooRealVar yield_background_y("yield_background_y", "background yield", 1000, 0, 100000);
  RooAddPdf sum_y("sum_y","Total distribution",RooArgList(gauss_y,gauss2_y),RooArgList(yield_signal_y,yield_background_y));

  RooFitResult *r_y = sum_y.fitTo(data_y, RooFit::Save());
  //RooFitResult *r = sum.fitTo(binnedData, RooFit::Save());
  //summary printing
  r_y->Print();
  //TH2* hcorr = r->correlationHist() ;
  //plot the result
  RooPlot * yFrame = dtyr.frame() ;
  //binnedData.plotOn(xFrame) ;
  data_y.plotOn(yFrame,RooFit::Name("data_y"));
  sum_y.plotOn(yFrame,RooFit::Name("total_y")) ;
  sum_y.plotOn(yFrame, RooFit::Components(gauss_y), RooFit::LineStyle(kDashed),RooFit::Name("gaussy")) ;
  sum_y.plotOn(yFrame, RooFit::Components(gauss2_y), RooFit::LineStyle(kDotted),RooFit::Name("gauss2y")) ;
  sum_y.paramOn(yFrame,RooFit::Layout(0.55)) ;
  TCanvas c_y;
  yFrame->Draw();
  auto reducedchi2_y = yFrame->chiSquare("total_y","data_y",6);
  //c.SaveAs("polfit.pdf");
  //adding legend
  TLegend *leg_y = new TLegend(0.4,0.6,0.89,0.89);
  leg_y->AddEntry("total_y",Form("Total distribution with chi2/ndf = %f",reducedchi2_y));
  leg_y->AddEntry("gaussy","First Gaussian");
  leg_y->AddEntry("gauss2y","Second Gaussian");
  leg_y->Draw();

  c_y.SaveAs("gausfit_angy.root");
}

