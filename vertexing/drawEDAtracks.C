void drawEDAtracks(){
 //which tracks do you want to draw?

 //TCut firstcut("nseg>=6&& TMath::Abs(s[0].eX-90000)<5000&& TMath::Abs(s[0].eY-90000)<5000&&TMath::Abs(s[nseg-1].eX-90000)<5000&& TMath::Abs(s[nseg-1].eY-90000)<5000"); //first general cut
 TCut firstcut("nseg>=4"); //first general cut
 //TCut firstcut("nseg==57");
//getting tracks set from file
 EdbEDA* gEDA = new EdbEDA("/home/scanner/sndlhc/RUN0/b000431/trackfiles/31_7/b000431.0.0.0.trk.root",-1,firstcut,kFALSE);
 EdbEDATrackSet *alltrackset = gEDA->GetTrackSet("TS"); 
 EdbEDATrackSet *selectedtrackset = gEDA->GetTrackSet("SF");

 int ntracks = alltrackset->N();

 for (int itrack = 0; itrack < ntracks; itrack++){
  EdbTrackP *track = alltrackset->GetTrack(itrack);
  int nseg = track->N();
  if (nseg >= 18 && TMath::Abs(track->X() - 96000) < 500 && TMath::Abs(track->Y() - 96000) < 500)
   selectedtrackset->AddTrack(track);
    
 }
 selectedtrackset->SetTrackAttribute(-1);
 selectedtrackset->SetColorMode(kCOLOR_BY_PLATE);
 alltrackset->SetDraw(kFALSE);
 gEDA->Redraw();
 gEDA->Run();

} 

