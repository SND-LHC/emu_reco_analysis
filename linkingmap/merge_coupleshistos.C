//merging different cells

TH2D *htxty, *htxty_small, *hxy;
TH1D *htx, *hty, *htheta, *htx_small, *hty_small, *htheta_small;
EdbCell2 * emulsioncell;

int couples_loop(TString cpfilename, int ix, int iy);

void merge_coupleshistos(int brickID, int platenumber){
 //histo file
 TFile *histofile = new TFile(Form("/eos/user/s/snd2cern/emu_reco_plots/RUN1/b%0*i/goodcouples/goodcoupleshistos_cells%i.root",6, brickID, platenumber),"RECREATE");  
 //histograms to be stored
 TTimeStamp *tstamp = new TTimeStamp();

 htxty = new TH2D("htxty","2D angular distribution;TX;TY",300,-1.5,1.5,300,-1.5,1.5);
 htxty_small = new TH2D("htxty_small","2D angular distribution in small range;TX;TY",300,-0.15,0.15,300,-0.15,0.15);

 htx = new TH1D("htx","1D angular distribution;TX",300,-1.5,1.5);
 hty = new TH1D("hty","1D angular distribution;TY",300,-1.5,1.5);
 htheta = new TH1D("htheta","1D Theta distribution#Theta[rad]",150,0.,1.5);

 htx_small = new TH1D("htx_small","1D angular distribution in small range;TX",300,-0.15,0.15);
 hty_small = new TH1D("hty_small","1D angular distribution in small range;TY",300,-0.15,0.15);
 htheta_small = new TH1D("htheta_small","1D Theta distribution in small range;#Theta[rad]",150,0.,0.15);

 hxy = new TH2D("hxy","2D position distribution;x[mm];y[mm]",190,0,190,190,0,190);

 //same parameters used as input for linking map!
 const int nx = 18;
 const int ny = 18;
 const float xmin = 5000.;
 const float xmax = 185000.;
 const float ymin = 5000.;
 const float ymax = 185000.;
 emulsioncell = new EdbCell2();
 emulsioncell->InitCell(nx,xmin,xmax,ny,ymin,ymax,1);

 TH1F *hshr1 = new TH1F("hshr1","Shrinkage corrections all cells bot;shr1",80,0.7,1.5);
 TH1F *hshr2 = new TH1F("hshr2","Shrinkage corrections all cells top;shr2",80,0.7,1.5);

 TString prepath(Form("/eos/experiment/sndlhc/emulsionData/2022/CERN/emu_reco/RUN1/b%0*i/p%0*i/%i.%i.",6,brickID,3,platenumber,brickID, platenumber));

for (int ix = 0; ix < nx; ix++){
 for (int iy = 0; iy < ny; iy++){

  //first cp check
  TString firstlinkcpfilename(prepath+TString(Form("%d.%d.firstlinkcp.root",ix+1,iy+1)));
  TFile *firstcpfile = TFile::Open(firstlinkcpfilename.Data()); 
  if(firstcpfile){
   EdbSegCorr *corr1 = (EdbSegCorr*) firstcpfile->Get("corr1");
   EdbSegCorr *corr2 = (EdbSegCorr*) firstcpfile->Get("corr2");
   
   if(!(corr1) || !(corr2)){
    cout<<"missiong corrections, skipping cell  "<<ix<<iy<<endl;
    continue;
   }
   
   hshr1->Fill(corr1->V(5));
   hshr2->Fill(corr2->V(5));

   firstcpfile->Close();
   }//close if
  else{
   cout<<"missing first cp file, skipping cell "<<ix<<iy<<endl;
   continue;
   }
  //second cp file, looping over couples
  TString secondlinkcpfilename(prepath+TString(Form("%d.%d.cp.root",ix+1,iy+1)));
  if (gSystem->AccessPathName(secondlinkcpfilename.Data())){//returns False if file exists, True if it does not (yes, I find it weird too)
    cout<<"File does not exist:"<<secondlinkcpfilename.Data()<<" moving to next cell"<<endl;
    continue;
   }
  int loop_return = couples_loop(secondlinkcpfilename, ix, iy);
  }//end loop y
 }//end loop x
 //drawing plots

 //Drawing histograms, saving them to output file
 histofile->cd();

 TCanvas *cshr = new TCanvas();
 cshr->Divide(1,2);
 cshr->cd(1);
 hshr1->Draw("hist");
 hshr1->Write();
 cshr->cd(2);
 hshr2->Draw("hist");
 hshr2->Write();

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

int couples_loop(TString cpfilename, int ix, int iy){

 EdbCouplesTree *mytree = new EdbCouplesTree();
 mytree->InitCouplesTree("couples",cpfilename.Data(),"READ");

 mytree->eCut = Form("eCHI2P<2.0&&s.eW>10&&eN1<=1&&eN2<=1&&s1.eFlag>=0&&s2.eFlag>=0&&TMath::Abs(s.eX-%.0f)<%.0f&&TMath::Abs(s.eY-%.0f)<%.0f"
  ,emulsioncell->X(ix),emulsioncell->Xbin()/2.,emulsioncell->Y(iy),emulsioncell->Ybin()/2.); //best rank couples
 //how many entries above the cut?
 if (!(mytree->eTree)) return -1; //no TTree in file
 TEventList *lst = mytree->InitCutList();
 if (!lst){ 
  cout<<"We have no entries, quitting!"<<endl;
  return -1;}

 int nlst =lst->GetN();
 cout<<"We have "<<nlst<<" good couples with selection"<< mytree->eCut<<endl;

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

 mytree->Close();
 return 0;
}
