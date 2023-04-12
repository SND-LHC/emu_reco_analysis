//cut couples, using copytree
void cutcouples(){
 //read file
 TFile *inputfile = TFile::Open("/home/scanner/sndlhc/RUN1/b000023/p002/23.2.0.0.cp.root");
 TTree *inputtree = (TTree*) inputfile->Get("couples");

 cout<<"Selecting best couples "<<inputtree->GetEntries()<<endl;
 //selection file
 TFile *outputfile = new TFile("testcutcouples.root","RECREATE");
 TTree *cuttree = inputtree->CopyTree("eN1==1 && eN2==1");

 cout<<"End of best couples selection, now "<<cuttree->GetEntries()<<endl;

 outputfile->cd();
 cuttree->Write();

 outputfile->Close();
}
