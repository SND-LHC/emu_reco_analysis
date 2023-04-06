//merging different cell trees
EdbCell2 * emulsioncell;
TList *selectedcoupleslist;
TFile * outputfile;

int couples_loop(TString cpfilename, int ix, int iy);

void merge_couplestrees(){
 //histo file
 outputfile = new TFile("44.2.0.0.cp.root","RECREATE"); 
 //histograms to be stored
 TTimeStamp *tstamp = new TTimeStamp();

 selectedcoupleslist = new TList();

 const int platenumber = 2;
 //same parameters used as input for linking map!
 const int nx = 19;
 const int ny = 19;
 const float xmin = 0.;
 const float xmax = 190000.;
 const float ymin = 0.;
 const float ymax = 190000.;
 emulsioncell = new EdbCell2();
 emulsioncell->InitCell(nx,xmin,xmax,ny,ymin,ymax,1);

 TString prepath(Form("root:://eospublic.cern.ch//eos/experiment/sndlhc/emulsionData/2022/CERN/emu_reco/RUN1/b000044/p00%i/44.%i.",platenumber,platenumber));

for (int ix = 1; ix < nx; ix++){
 for (int iy = 1; iy < ny; iy++){

  //second cp file, looping over couples
  TString secondlinkcpfilename(prepath+TString(Form("%d.%d.cp.root",ix,iy)));
  int loop_return = couples_loop(secondlinkcpfilename, ix, iy);
  }//end loop y
 }//end loop x
 //drawing plots

 TTree *mergedcouples = TTree::MergeTrees(selectedcoupleslist);
 //Drawing histograms, saving them to output file
 outputfile->cd();
 mergedcouples->Write();

 cout<<"Start time: "<<tstamp->GetTime()<<endl;
 tstamp->Set();
 cout<<"End time: "<<tstamp->GetTime()<<endl;

}

int couples_loop(TString cpfilename, int ix, int iy){

 EdbCouplesTree *mytree = new EdbCouplesTree();
 mytree->InitCouplesTree("couples",cpfilename.Data(),"READ");
 cout<<"Selecting cell "<<ix<<iy<<endl;

 outputfile->cd();
 if (mytree->eTree->GetEntries()==0) return -1;

 TTree *outputtree = mytree->eTree->CopyTree(Form("TMath::Abs(s.eX-%.0f)<%.0f&&TMath::Abs(s.eY-%.0f)<%.0f"
  ,emulsioncell->X(ix),emulsioncell->Xbin()/2.,emulsioncell->Y(iy),emulsioncell->Ybin()/2.)); //removing cell overlap

 selectedcoupleslist->Add(outputtree);
 cout<<"Selected cell "<<ix<<iy<<" before "<<mytree->eTree->GetEntries()<<" now "<<outputtree->GetEntries()<<endl;
 mytree->Close();
 return 0;
}
