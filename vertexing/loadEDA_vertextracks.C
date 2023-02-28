//Loading tracks with EDA, displaying vertices and preparing analysis;
//To reconstruct other vertices, please use gEDA->GetTrackSet("TS")->SetDraw(kTRUE) before it;
//The other verticves are stored in gEDA->GetVertexSet()->eVerticesPrevious
//Used also to test shower reconstruction;
EdbVertexRec * vertexrec;
EdbEDA *gEDA;
namespace VERTEX_PAR
{
  float DZmax = 3000.;
  //float ProbMinV   = 0.0001;  // minimum acceptable probability for chi2-distance between tracks
  float ProbMinV   = 0.01;
  float ImpMax     = 15.;    // maximal acceptable impact parameter [microns] (for preliminary check)
  bool  UseMom     = false;  // use or not track momentum for vertex calculations
  bool  UseSegPar  = true;  // use only the nearest measured segments for vertex fit (as Neuchatel)
  int   QualityMode= 0;      // vertex quality estimation method (0:=Prob/(sigVX^2+sigVY^2); 1:= inverse average track-vertex distance)
}

void DrawLinkedVertices(int vID = -1){
 //vID == -1 means use selected vertex
 int nlinked = 0;
 EdbVertex *vertex; 
 if (vID >= 0) (EdbVertex*) gEDA->GetVertexSet()->eVerticesPrevious->At(vID); //provided vID, use that to get vertex
 else vertex = gEDA->GetSelectedVertex(); //look from selected vertex
 //looping over tracks belonging to vertex
 for (int itrack = 0; itrack < vertex->N(); itrack++){
  //vertex track association, then getting track
  EdbVTA *VTa = vertex->GetVTa(itrack);
  EdbTrackP *track = VTa->GetTrack();
  if (VTa->Zpos() == 1){ //track exits from vertex, look if ends in another vertex
   EdbVertex * endingvtx = track->VertexE();
   if (endingvtx){
    gEDA->GetVertexSet()->AddVertex(endingvtx);
    //adding all tracks from that vertex (same track will not be added twice, check already present in EdbEDATrackSet->AddTrack())
    for (int jtrack = 0; jtrack < endingvtx->N(); jtrack++){
     EdbTrackP *newtrack = endingvtx->GetTrack(jtrack);
     gEDA->GetTrackSet("SB")->AddTrack(newtrack);
    }//end  of tracks loop
    //increasing counter
    nlinked++;
   } //end if ending vtx
  }//end if vta
  if (VTa->Zpos() == 0){  //track ends from vertex, look if starts in another vertex
   EdbVertex * startingvtx = track->VertexS();

   if (startingvtx){
    gEDA->GetVertexSet()->AddVertex(startingvtx);
    //adding all tracks from that vertex (same track will not be added twice, check already present in EdbEDATrackSet->AddTrack())
    for (int jtrack = 0; jtrack < startingvtx->N(); jtrack++){
     EdbTrackP *newtrack = startingvtx->GetTrack(jtrack);
     gEDA->GetTrackSet("SB")->AddTrack(newtrack);
    }//end  of tracks loop
    //increasing counter
    nlinked++;
   } 

  }//end if vta
 }//end of loop over tracks
 gEDA->Redraw();
 cout<<"Disegnati altri vertici: "<<nlinked<<endl;
 
}

void DrawVertexList(int nvertices, int* vertexlist){
 gEDA->GetVertexSet()->StorePrevious();
 TObjArray *vertices_todraw = new TObjArray();
 //selecting my vertices to be drawn
 for (int i = 0; i < nvertices; i++){ //range for loop, C++11
  int vID = vertexlist[i];
  EdbVertex *vtx = gEDA->GetVertexSet()->GetVertex(vID);
  //eda->GetVertexSet()->DrawSingleVertex(vtx);
  vertices_todraw->Add(vtx);
 }
 gEDA->GetVertexSet()->ClearVertices(); //clearing vertices (previous are still recoverable)
 gEDA->GetVertexSet()->SetVertices(vertices_todraw);
 gEDA->Redraw();
}

void loadEDA_vertextracks(bool newversion = true, TString vertexfilename= "vertextree.root", bool applycut = false){
 //selected vertices to be drawn
 const int nvertices = 1;
 int vertexlist[nvertices] = {1968}; //1968
 //EDA tracks loading and initialization
 gEDA = new EdbEDA("b000431.0.0.0.trk.root",-1,"1",kFALSE);
 gEDA->GetTrackSet("TS")->SetDraw(kFALSE);
 //SETTING PIDs
 EdbPVRec * pvrec_linkedtracks = gEDA->GetTrackSet("TS")->GetPVRec();
 int npatterns = pvrec_linkedtracks->Npatterns();
 for (int ipat = 0; ipat < npatterns; ipat++){
  EdbPattern * pat = pvrec_linkedtracks->GetPattern(ipat);
  pat->SetPID(pat->GetSegment(0)->PID());
 }
 
 using namespace VERTEX_PAR;
 TFile * inputfile = TFile::Open(vertexfilename.Data());
 TTree *vtxtree = (TTree*) inputfile->Get("vtx");
 EdbDataProc *dproc = new EdbDataProc();


 TObjArray *drawnvertices = new TObjArray(1000000);
 TObjArray *drawntracks = new TObjArray(1000000);

// EdbTrackP *specialtrack = new EdbTrackP();

 EdbPVRec *ali = new EdbPVRec();
 EdbScanCond *scancond = new EdbScanCond();
 ali->SetScanCond(scancond);
 
 vertexrec = new EdbVertexRec();
 vertexrec->SetPVRec(ali);
 vertexrec->eDZmax=DZmax;
 vertexrec->eProbMin=ProbMinV;
 vertexrec->eImpMax=ImpMax;
 vertexrec->eUseMom=UseMom;
 vertexrec->eUseSegPar=UseSegPar;
 vertexrec->eQualityMode=QualityMode;

 EdbVertex *vertex = 0;
 map<int,EdbTrackP*>emptymap;

 dproc->ReadVertexTree(*vertexrec, "vertextree.root", "1",emptymap);

 vertexrec->eEdbTracks = pvrec_linkedtracks->eTracks; //adding tracks info?

 cout<<"Reading number of vertices: "<<ali->eVTX->GetEntries()<<endl;

 for (int i = 0; i < ali->eVTX->GetEntries(); i++){ //range for loop, C++11
  int vID = i;
  vertex = (EdbVertex*) ((TObjArray*)(ali->eVTX)) ->At(vID);
  drawnvertices->Add(vertex); // assuming the array is filled with EdbVertex.
  
 }//end loop on vertices

 ali->eVTX = drawnvertices; 

 //adding all vertices to
 for (int ivtx = 0; ivtx < ali->eVTX->GetEntries(); ivtx++){
  EdbVertex* vtx = (EdbVertex*) ali->eVTX->At(ivtx);
  gEDA->GetVertexSet()->AddVertex(vtx);
 }

 gEDA->Run();
 for (int i = 0; i < nvertices; i++){ //range for loop, C++11
  int vID = vertexlist[i];
  EdbVertex *vtx = gEDA->GetVertexSet()->GetVertex(vID);
  //eda->GetVertexSet()->DrawSingleVertex(vtx);
  //adding tracks
  for (int itrack = 0; itrack < vtx->N();itrack++){
   EdbTrackP *track = vtx->GetTrack(itrack);
   gEDA->GetTrackSet("SB")->AddTrack(track);
  //eda->GetTrackSet("TS")->RemoveTrack(track); //if we want to remove the track from other dataset
  }//end loop on tracks
 }//end loop on vertices
 DrawVertexList(nvertices, vertexlist);
}//end code
