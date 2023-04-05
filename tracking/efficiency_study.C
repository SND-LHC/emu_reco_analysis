EdbPVRec     *gAli=0;
using namespace ROOT;
void efficiency_study(){ //efficiency estimation, as used in OPERA paper

 const int nsegmin = 12; //18 in RUN0
 cout<<"requring at least "<<nsegmin<< " segments "<<endl;
 //TCut trcut = "t.eFlag>=0  &&t.eProb>0.01";
 TCut trcut = Form("t.eFlag>=0 &&t.eProb>0.01&&nseg >= %d",nsegmin);
 const int nfilms = 25;
 const int firstfilm = 15;
 const int lastfilm = 39; 

 cout<<" over nfilms "<<nfilms<<" from "<<firstfilm<<" to "<<lastfilm<<endl;

 TEfficiency* heff = NULL;

 TH1D *hexpected = new TH1D("hexpected", "Tracks expected to be found in each plate", 58,0,58);
 TH1D *hfound = new TH1D("hfound", "Tracks with an associated segment in each plate", 58,0,58); 

 TH2D *hxy = new TH2D("hxy", "2D position of tracks",190,0,190,190,0,190);

 //reading tracks from linked_tracks.root file
 EdbDataProc *dproc = new EdbDataProc();
 dproc->InitVolume(100, trcut); //100 is the code for track reading (as from fedra/src/libEIO/EdbDataSet.cxx)
 gAli = dproc->PVR();
 const int ntracks = gAli->eTracks->GetEntries();
 printf("ntracks after cut = %d\n", ntracks);

 //begin track loop
 EdbTrackP *trk = NULL;
 EdbSegP *seg = NULL;
 EdbSegP *firstseg = NULL;
 EdbSegP *lastseg = NULL;

 int nplate;
 for (int itrk = 0; itrk <ntracks; itrk++){
  trk = (EdbTrackP*)(gAli->eTracks->At(itrk)); //accessing track object
  //expected to found the track from first to last segment
  firstseg = trk->GetSegmentFirst();
  lastseg = trk->GetSegmentLast();
  for (int i = firstfilm; i <= lastfilm;i++){
  //for (int i = firstseg->Plate(); i <= lastseg->Plate();i++){
   hexpected->Fill(i);
  }
  hexpected->Fill(0);//for having y range set from 0 to 1 I add a bin with null efficiency for Plate 0
  hexpected->Fill(1);//for having y range set from 0 to 1 I add a bin with null efficiency for Plate 1
  hexpected->Fill(2);//for having y range set from 0 to 1 I add a bin with null efficiency for Plate 2
  hexpected->Fill(3);//for having y range set from 0 to 1 I add a bin with null efficiency for Plate 3 
  
  for (int iseg = 0; iseg < trk->N();iseg++){ //loop on associated segments
    seg = (EdbSegP*) trk->GetSegment(iseg);
    nplate = seg->Plate();
    //if(nplate!=15)
    hfound->Fill(nplate);
  }
  
  hxy->Fill(trk->X()/1000., trk->Y()/1000.);
 } //end of track loop

 //Getting efficiency for all plates
 TCanvas *c1 = new TCanvas();
 if (TEfficiency::CheckConsistency(*hfound,*hexpected)){
  heff = new TEfficiency(*hfound, *hexpected);
  heff->Draw();
  heff->SetTitle(";Plate;#epsilon");
  //estimating global efficiency and its error
  RVec<double> effplate; //efficiency in each plate
  double mean = 0;
  double sd = 0; //standard deviation
  double sd_mean = 0; //standard deviation of the mean (standard error)
  
  for (int ibin=firstfilm; ibin <= lastfilm; ibin++){ 
   effplate.push_back(heff->GetEfficiency(ibin));//counting only plates used, witout first and last   
   cout<<ibin<<" "<<heff->GetEfficiency(ibin)<<endl;
  }
  cout<<"Mean efficiency: "<<ROOT::VecOps::Mean(effplate)<<"with error: "<<ROOT::VecOps::StdDev(effplate)/TMath::Sqrt(nfilms)<<endl;
 }

 TCanvas *c2 = new TCanvas();
 hxy->Draw("COLZ");
 //storing into outputfile
 TFile *outputfile = new TFile("efficiency_plate.root","RECREATE");
 heff->Write();
}
