//merging different cell trees
EdbCell2 * emulsioncell;
TList *selectedcoupleslist;
TFile * outputfile;

int couples_loop(TString cpfilename, int ix, int iy);

void merge_couplestrees(int platenumber){
 //histo file
 outputfile = new TFile(Form("/eos/experiment/sndlhc/emulsionData/2022/CERN/emu_reco/RUN1/b000044/p%0*i/44.%i.0.0_merged.cp.root",3,platenumber, platenumber),"RECREATE"); 
 //histograms to be stored
 TTimeStamp *tstamp = new TTimeStamp();

 selectedcoupleslist = new TList();

 //const int platenumber = 2; //now as input option of the script
 //same parameters used as input for linking map!
 const int nx = 19;
 const int ny = 19;
 const float xmin = 0.;
 const float xmax = 190000.;
 const float ymin = 0.;
 const float ymax = 190000.;
 emulsioncell = new EdbCell2();
 emulsioncell->InitCell(nx,xmin,xmax,ny,ymin,ymax,1);

 TString prepath(Form("root:://eospublic.cern.ch//eos/experiment/sndlhc/emulsionData/2022/CERN/emu_reco/RUN1/b000044/p%0*i/44.%i.",3,platenumber,platenumber));

for (int ix = 0; ix < nx; ix++){
 for (int iy = 0; iy < ny; iy++){

  //second cp file, looping over couples
  TString secondlinkcpfilename(prepath+TString(Form("%d.%d.cp.root",ix+1,iy+1))); //name starts 1 more now
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
 if (mytree->eTree->GetEntries()==0) return -1;

 TTree *outputtree = mytree->eTree->CopyTree(Form("eN1<=1&&eN2<=1&&s1.eFlag>=0&&s2.eFlag>=0&&TMath::Abs(s.eX-%.0f)<%.0f&&TMath::Abs(s.eY-%.0f)<%.0f"
  ,emulsioncell->X(ix),emulsioncell->Xbin()/2.,emulsioncell->Y(iy),emulsioncell->Ybin()/2.)); //removing cell overlap

 selectedcoupleslist->Add(outputtree);
 cout<<"Selected cell "<<ix<<iy<<" before "<<mytree->eTree->GetEntries()<<" now "<<outputtree->GetEntries()<<endl;
 mytree->Close();
 return 0;
}
