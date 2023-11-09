void couples_loop(){

 const int brickID = 21;
 const int plateID = 9;

 TTimeStamp *tstamp = new TTimeStamp();

 TH2D *htxty = new TH2D("htxty","2D angular distribution;TX;TY",300,-1.5,1.5,300,-1.5,1.5);
 TH2D *htxty_small = new TH2D("htxty_small","2D angular distribution in small range;TX;TY",300,-0.15,0.15,300,-0.15,0.15);

 TH1D *htx = new TH1D("htx","1D angular distribution;TX",300,-1.5,1.5);
 TH1D *hty = new TH1D("hty","1D angular distribution;TY",300,-1.5,1.5);
 TH1D *htheta = new TH1D("htheta","1D Theta distribution#Theta[rad]",150,0.,1.5);
 
 TH1D *htx_small = new TH1D("htx_small","1D angular distribution in small range;TX",300,-0.15,0.15);
 TH1D *hty_small = new TH1D("hty_small","1D angular distribution in small range;TY",300,-0.15,0.15);
 TH1D *htheta_small = new TH1D("htheta_small","1D Theta distribution in small range;#Theta[rad]",150,0.,0.15);

 //TH2D *hxy = new TH2D("hxy","2D position distribution;x[mm];y[mm]",50,70,120,50,70,120);
 TH2D *hxy = new TH2D("hxy","2D position distribution;x[mm];y[mm]",190,0,190,190,0,190);

 EdbCouplesTree *mytree = new EdbCouplesTree();
 mytree->InitCouplesTree("couples",Form("/home/scanner/sndlhc/RUN1/b%06i/p%03i/%i.%i.0.0.cp.root",brickID,plateID,brickID,plateID),"READ");
 mytree->eCut = "eCHI2P<2.0&&s.eW>10&&eN1<=1&&eN2<=1&&s1.eFlag>=0&&s2.eFlag>=0"; //best rank couples

 TFile *histofile = new TFile(Form("/home/scanner/sndlhc/RUN1/b%06i/plots/goodcoupleshistos_%i.root",brickID,plateID),"RECREATE");
 //how many entries above the cut?
 TEventList *lst = mytree->InitCutList();
 int nlst =lst->GetN();
 cout<<"We have "<<nlst<<" good couples"<<endl;

 int entr = 0;
 for(int i=0; i<nlst; i++ ) {
  entr = lst->GetEntry(i);
  mytree->GetEntry(entr);

  EdbSegP *myseg = mytree->eS;
  
  //filling histograms
  hxy->Fill(myseg->X()/1000., myseg->Y()/1000.);
  htxty->Fill(myseg->TX(),myseg->TY());
  htx->Fill(myseg->TX());
  hty->Fill(myseg->TY());
  htheta->Fill(TMath::ATan(myseg->Theta()));

  htxty_small->Fill(myseg->TX(),myseg->TY());
  htx_small->Fill(myseg->TX());
  hty_small->Fill(myseg->TY());
  htheta_small->Fill(TMath::ATan(myseg->Theta()));

 }

 //Drawing histograms, saving them to output file
 histofile->cd();

 TCanvas *ctxty = new TCanvas("ctxty","txty angles of good segments",800,800);
 htxty->Draw("COLZ");
 htxty->Write();

 TCanvas *ctheta = new TCanvas("ctheta","theta angle of good segments");
 htheta->Draw();
 htheta->Write();
 TCanvas *cxy = new TCanvas("cxy","xy position of good segments",800,800);
 hxy->Draw("COLZ");
 hxy->Write();

 TCanvas *c1Dtxty = new TCanvas("c1Dtxty","angular 1D");
 c1Dtxty->Divide(1,2);
 c1Dtxty->cd(1);
 htx->Draw();
 htx->Write();
 c1Dtxty->cd(2);
 hty->Draw();
 hty->Write();

 TCanvas *ctxty_small = new TCanvas("ctxty_small","txty angles with small range of good segments",800,800);
 htxty_small->Draw("COLZ");
 htxty_small->Write();

 TCanvas *ctheta_small = new TCanvas("ctheta_small","theta angle of good segments");
 htheta_small->Draw();
 htheta_small->Write();

 TCanvas *c1Dtxty_small = new TCanvas("c1Dtxty_small","angular 1D");
 c1Dtxty_small->Divide(1,2);
 c1Dtxty_small->cd(1);
 htx_small->Draw();
 htx_small->Write();
 c1Dtxty_small->cd(2);
 hty_small->Draw();
 hty_small->Write();

 cout<<"Start time: "<<tstamp->GetTime()<<endl;
 tstamp->Set();
 cout<<"End time: "<<tstamp->GetTime()<<endl;
}
