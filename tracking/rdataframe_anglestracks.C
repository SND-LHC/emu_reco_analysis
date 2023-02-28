//Drawing tracks simple distribution with RDataFrame (26 May)
//In case of quarters, remember to change both inputfiles, cut, and outputfile

void CopyNoCOV(EdbSegP&tr, EdbSegP*firstseg){
  //covariance matrix does get problem in destruction, for now we need to exclude it
  tr.SetPID(firstseg->PID());
  tr.Set(firstseg->ID(),firstseg->X(),firstseg->Y(),firstseg->TX(),firstseg->TY(),firstseg->W(),firstseg->Flag());
  tr.SetZ(firstseg->Z());
  //if(firstseg->CheckCOV()) tr.SetCOV(firstseg->COV());
  tr.SetSZ(firstseg->SZ());
  tr.SetVid(firstseg->Vid(0),firstseg->Vid(1));
  tr.SetAid(firstseg->Aid(0),firstseg->Aid(1),firstseg->Side());
  tr.SetProb(firstseg->Prob());
  tr.SetVolume(firstseg->Volume());
  tr.SetDZ(firstseg->DZ());
  tr.SetDZem(firstseg->DZem());
  tr.SetP(firstseg->P());
  tr.SetChi2(firstseg->Chi2());
  tr.SetTrack(firstseg->Track());
  tr.SetMC( firstseg->MCEvt(), firstseg->MCTrack() );
  tr.SetScanID(firstseg->ScanID());

}

double GetVX(TClonesArray sf){
  //Since covariance matrix cannot be copied in EdbSegP branch, leads to segfault at the end, access attributes like this
  EdbSegP* firstseg = (EdbSegP*) sf.At(0);
  return firstseg->COV()(0,0);
}

EdbSegP GetFirstSegment(TClonesArray sf){
  EdbSegP* firstseg = (EdbSegP*) sf.At(0);
  EdbSegP tr;
  //EdbSegP tr(*firstseg);

  
  CopyNoCOV(tr,firstseg);
  
  return tr;
}

EdbSegP GetLastSegment(TClonesArray sf){
  EdbSegP* lastseg = (EdbSegP*) sf.At(sf.GetEntries()-1);
  EdbSegP trlast;
  //EdbSegP tr(*firstseg);

  
  CopyNoCOV(trlast,lastseg);
  
  return trlast;
}



using namespace ROOT;
void rdataframe_anglestracks(){
  TFile *tracksfile = TFile::Open("/home/scanner/sndlhc/RUN1/b000021/trackfiles/39-15/b000021.0.0.0.trk.root");
  TTree *trackstree = (TTree*) tracksfile->Get("tracks");
  //FOR MIC2 (use these if last two plates are scanned from mic2)!
  const double txslant = 0.0125672;
  const double tyslant = 0.0441632;

  const int maxnseg = 25;
  const int minnseg = 18;
  //position map binning
  const int nbinsx = 6;
  const float xmin = 9.2;
  const float xmax = 9.8;
  const int nbinsy = 6;
  const float ymin = 9.2;
  const float ymax = 9.8;

  //angle bins
  const int nbinstx = 2000;
  const float txmin = -0.1;
  const float txmax = 0.1;
  const int nbinsty = 2000;
  const float tymin = -0.1;
  const float tymax  = 0.1; 


  RDataFrame df(*trackstree);

  //nseg before cut, to have all track segments
  auto hnseg = df.Fill<int>(TH1I("hnseg", "number of segments;nseg", maxnseg+1, 0, maxnseg+1), {"nseg"}); //integer filling is more convoluted

  auto dftr0 = df.Define("varx",GetVX,{"sf"}); //cov matrix need to be stored in another branch

  auto dftr = dftr0.Define("tr",GetFirstSegment,{"sf"}).Define("trlast",GetLastSegment,{"sf"});
  //definition of variables to fill histogram with (only difference, coordinates in cm instead of micron);
  auto dftr1 = dftr.Define("TX",Form("tr.TX() - (%f/2.) ",txslant))
                   .Define("TY",Form("tr.TY() - (%f/2.) ",tyslant))
                   .Define("Xcm","tr.X()*1e-4  ")
                   .Define("Ycm","tr.Y()*1e-4  ")
                   .Define("Xcmlast","trlast.X()*1e-4  ")
                   .Define("Ycmlast","trlast.Y()*1e-4  ");

  //3D angles
  auto dftr2 = dftr1.Define("tantheta","tr.Theta()").Define("theta","TMath::ATan(tantheta)");

  //selecting good tracks (aka long, in this case  
  //auto dfgoodtr = dftr2.Filter(Form("nseg>=%i",minnseg)).Filter("Xcm<9.6&&Ycm>9.6"); //cut in position
  auto dfgoodtr = dftr2.Filter(Form("nseg>=%i",minnseg)); //not cut in position

  auto hxy = dfgoodtr.Histo2D({"hxy","xy map;x[cm];y[cm]",nbinsx,xmin,xmax,nbinsy,ymin,ymax},"Xcm","Ycm");

  auto htx = dfgoodtr.Histo1D({"htx","TX of tracks;TX", nbinstx,txmin,txmax},"TX");
  auto htxmap = dfgoodtr.Profile2D({"htxmap","TX of tracks in xy map;x[cm];y[cm];TX",nbinsx,xmin,xmax,nbinsy,ymin,ymax, txmin,txmax},"Xcm","Ycm","TX");

  auto hty = dfgoodtr.Histo1D({"hty","TY of tracks;TY", nbinsty,tymin,tymax},"TY");
  auto htymap = dfgoodtr.Profile2D({"htymap","TY of tracks in xy map;x[cm];y[cm];TY",nbinsx,xmin,xmax,nbinsy,ymin,ymax, tymin,tymax},"Xcm","Ycm","TY");

  auto htxty = dfgoodtr.Histo2D({"htxty","TY and TX of tracks;TX;TY",nbinstx,txmin,txmax, nbinsty,tymin,tymax},"TX","TY");

  //Theta angle
  auto htheta = dfgoodtr.Histo1D({"htheta","Theta angle distribution;#theta",150,0,1.5},"theta");

  auto hsintheta = dfgoodtr.Define("sintheta","TMath::Sin(theta)")
                           .Histo1D({"hsintheta","SinTheta distribution;sin#theta",110,0,1.1},"sintheta");

  auto hcostheta = dfgoodtr.Define("costheta","TMath::Cos(theta)")
                           .Histo1D({"hcostheta","CosTheta distribution;cos#theta",110,0,1.1},"costheta");

  //just for testing COV
  auto hvarx = dfgoodtr.Histo1D("varx");

  TFile *canvasfile = new TFile("plots_25plates_1w2b1.root","RECREATE");
  canvasfile->cd();
  //Drawing plots
  TCanvas *ctx = new TCanvas("ctx","TX Canvas",1600,800);
  ctx->Divide(2,1);
  ctx->cd(1);
  htx->DrawClone();
  ctx->cd(2);
  htxmap->DrawClone("COLZ");

  htx->Write();
  htxmap->Write();
  //ctx->Write();

  //Drawing plots
  TCanvas *cty = new TCanvas("cty","TY Canvas",1600,800);
  cty->Divide(2,1);
  cty->cd(1);
  hty->DrawClone();
  cty->cd(2);
  htymap->DrawClone("COLZ");

  hty->Write();
  htymap->Write();
  //cty->Write();

  TCanvas *cangle = new TCanvas("cangle","Angular distributions",1600,800);
  cangle->Divide(2,1);
  cangle->cd(1);
  htheta->DrawClone();
  cangle->cd(2);
  htxty->DrawClone("COLZ");

  htheta->Write();
  htxty->Write();
  //cangle->Write();

  //averagine of histogram
  const int minbin = 1;
  const int maxbin = nbinsx;
  ROOT::RVec<int> nmuons_array;

  for (int ibinx = minbin+1; ibinx <=maxbin; ibinx++){ //bin 0 is actually underflow!
   for (int ibiny = minbin+1; ibiny <=maxbin; ibiny++){
   int nmuons = hxy->GetBinContent(ibinx,ibiny);
    nmuons_array.push_back(nmuons);
   }
  }

  cout<<"N values "<<nmuons_array.size()<<endl;
  cout<<"Valor Medio: "<<ROOT::VecOps::Mean(nmuons_array)<<" con errore "<<ROOT::VecOps::StdDev(nmuons_array)/TMath::Sqrt(nmuons_array.size())<<endl;

  TCanvas *cxy = new TCanvas("cxy","XY Canvas",800,800);
  hxy->DrawClone("COLZ && TEXT");

  TLine *lx0 = new TLine(minbin,minbin,minbin,maxbin);
  lx0->SetLineColor(kRed);
  lx0->SetLineWidth(3);

  TLine *lx1 = new TLine(maxbin,minbin,maxbin,maxbin);
  lx1->SetLineColor(kRed);
  lx1->SetLineWidth(3);
 
  TLine *ly0 = new TLine(minbin,minbin,maxbin,minbin);
  ly0->SetLineColor(kRed);
  ly0->SetLineWidth(3);

  TLine *ly1 = new TLine(minbin,maxbin,maxbin,maxbin);
  ly1->SetLineColor(kRed);
  ly1->SetLineWidth(3);

  lx0->Draw("SAME");
  lx1->Draw("SAME");
  ly0->Draw("SAME");
  ly1->Draw("SAME");

  hxy->Write();
  //cxy->Write();

  TCanvas *csincostheta = new TCanvas("csincostheta","Sin and Cos Theta");
  csincostheta->Divide(2,1);
  csincostheta->cd(1);
  hsintheta->DrawClone();
  csincostheta->cd(2);
  hcostheta->DrawClone();

  hsintheta->Write();
  hcostheta->Write();
  //csincostheta->Write();

  TCanvas *cnseg = new TCanvas("cnseg","Number of segments per track");
  hnseg->DrawClone();

  hnseg->Write();
  //cnseg->Write();

}

//computing density, from one or more files
void compute_density(){
 TFile * tracks1 = TFile::Open("/home/scanner/sndlhc/RUN1/b000021/trackfiles/39-15/plots_25plates_1w2b1.root");

 //TH2D *hxy_effcorrected_1 = (TH2D*) tracks1->Get("hxy_effcorrected");
 //TH2D *hxy_effcorrected_2 = (TH2D*) tracks2->Get("hxy_effcorrected");
 //TH2D *hxy_effcorrected_3 = (TH2D*) tracks3->Get("hxy_effcorrected");
 //TH2D *hxy_effcorrected_4 = (TH2D*) tracks4->Get("hxy_effcorrected");

 TH2D *hxy_effcorrected_1 = (TH2D*) tracks1->Get("hxy");

 const double volume_track_eff = 0.832754;

 //efficiency correction

 hxy_effcorrected_1->Scale(1./volume_track_eff);

 //making the average value
 const int nbinsx = 6;
 const int nbinsy = 6;
 const int minbinx = 2;
 const int maxbinx = nbinsx;
 const int minbiny = 2;
 const int maxbiny = nbinsy;
 
 ROOT::RVec<double> nmuons_array;

 for (int ibinx = 1; ibinx <=nbinsx; ibinx++){ //bin 0 is actually underflow!
   for (int ibiny = 1; ibiny <=nbinsx; ibiny++){
    double nmuons = hxy_effcorrected_1->GetBinContent(ibinx,ibiny);
    if (ibinx >= minbinx && ibinx <= maxbinx && ibiny >= minbiny && ibiny <= maxbiny) nmuons_array.push_back(nmuons);
    //only for visualization, to closest integer
    hxy_effcorrected_1->SetBinContent(ibinx,ibiny,TMath::Nint(nmuons));
   }
 }
 gStyle->SetOptStat("t"); //no stats box
 //drawing plot and line
 TCanvas *cxy = new TCanvas("cxy","XY Canvas",800,800);

 hxy_effcorrected_1->Draw("COLZ && TEXT");

 TLine *lx0 = new TLine(minbinx-1,minbiny-1,minbinx-1,maxbiny);
 lx0->SetLineColor(kRed);
 lx0->SetLineWidth(3);

 TLine *lx1 = new TLine(maxbinx,minbiny-1,maxbinx,maxbiny);
 lx1->SetLineColor(kRed);
 lx1->SetLineWidth(3);
 
 TLine *ly0 = new TLine(minbinx-1,minbiny-1,maxbinx,minbiny-1);
 ly0->SetLineColor(kRed);
 ly0->SetLineWidth(3);

 TLine *ly1 = new TLine(minbinx-1,maxbiny,maxbinx,maxbiny);
 ly1->SetLineColor(kRed);
 ly1->SetLineWidth(3);

 lx0->Draw("SAME");
 lx1->Draw("SAME");
 ly0->Draw("SAME");
 ly1->Draw("SAME");

 cout<<"N values "<<nmuons_array.size()<<endl; 
 cout<<"List of array value "<<nmuons_array<<endl;
 cout<<"Valor Medio: "<<ROOT::VecOps::Mean(nmuons_array)<<"dev standard "<< ROOT::VecOps::StdDev(nmuons_array)<<" errore standard "<<ROOT::VecOps::StdDev(nmuons_array)/TMath::Sqrt(nmuons_array.size())<<endl;

 
}
