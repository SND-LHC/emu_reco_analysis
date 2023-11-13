//merging different cell trees
EdbCell2 * emulsioncell;
TList *selectedcoupleslist;
TFile * outputfile;

int couples_loop(TString cpfilename, int ix, int iy);

void merge_couplestrees(int brickID, int platenumber){
 //histo file
 outputfile = new TFile(Form("/eos/experiment/sndlhc/emulsionData/2022/CERN/emu_reco/RUN1/b%0*i/p%0*i/%i.%i.0.0_merged.cp.root", 6, brickID, 3, platenumber, brickID, platenumber),"RECREATE"); 
 //histograms to be stored
 TTimeStamp *tstamp = new TTimeStamp();

 selectedcoupleslist = new TList();

 //const int platenumber = 2; //now as input option of the script
 //same parameters used as input for linking map!
 const int nx = 18;
 const int ny = 18;
 const float xmin = 5000.;
 const float xmax = 185000.;
 const float ymin = 5000.;
 const float ymax = 185000.;
 emulsioncell = new EdbCell2();
 emulsioncell->InitCell(nx,xmin,xmax,ny,ymin,ymax,1);

 TString prepath(Form("/eos/experiment/sndlhc/emulsionData/2022/CERN/emu_reco/RUN1/b%0*i/p%0*i/41.%i.",6, brickID, 3 , platenumber, platenumber));

for (int ix = 8; ix < 10; ix++){
 for (int iy = 8; iy < 10; iy++){

  //second cp file, looping over couples
  TString secondlinkcpfilename(prepath+TString(Form("%d.%d.cp.root",ix+1,iy+1))); //name starts 1 more now
  if (gSystem->AccessPathName(secondlinkcpfilename.Data())){//returns False if file exists, True if it does not (yes, I find it weird too)
    cout<<"File does not exist:"<<secondlinkcpfilename.Data()<<" moving to next cell"<<endl;
    continue;
   }
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
 cout<<"Selecting cell "<<ix<<iy<<" from file "<<cpfilename.Data()<<endl;

 outputfile->cd();
 if (!(mytree->eTree)) return -1; //no TTree in file
 if (mytree->eTree->GetEntries()==0) return -1; //there is Tree in file, but it is empty

 TTree *outputtree = mytree->eTree->CopyTree(Form("eN1<=1&&eN2<=1&&s1.eFlag>=0&&s2.eFlag>=0&&TMath::Abs(s.eX-%.0f)<%.0f&&TMath::Abs(s.eY-%.0f)<%.0f"
  ,emulsioncell->X(ix),emulsioncell->Xbin()/2.,emulsioncell->Y(iy),emulsioncell->Ybin()/2.)); //removing cell overlap

 selectedcoupleslist->Add(outputtree);
 cout<<"Selected cell "<<ix<<iy<<" before "<<mytree->eTree->GetEntries()<<" now "<<outputtree->GetEntries()<<endl;
 mytree->Close();
 return 0;
}
