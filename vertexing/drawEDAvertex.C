//display for vertices, patterns are not drawn
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

int FindMostCommonEvent(EdbTrackP *track);
void drawEDAvertex(bool newversion = true, TString vertexfilename= "vertextree.root"){
 using namespace VERTEX_PAR;
 const int nvertices = 1;
 int vertexlist[nvertices] = {1866};
 //int vertexcolors[nvertices] = {kRed,kRed,kRed,kRed};
 //int vertexlist[nvertices] = {260, 273, 280, 393};
 int vertexcolors[nvertices] = {kRed};
 EdbDataProc *dproc = new EdbDataProc();

 TFile * inputfile = TFile::Open(vertexfilename.Data());
 EdbVertexRec *vertexrec; 

 TObjArray *drawnvertices = new TObjArray(100);
 TObjArray *drawntracks = new TObjArray(100);

// EdbTrackP *specialtrack = new EdbTrackP();

 EdbPVRec *ali = new EdbPVRec();
 EdbScanCond *scancond = new EdbScanCond();
 ali->SetScanCond(scancond);

 for (int i = 0; i < nvertices; i++){ //range for loop, C++11
  int vID = vertexlist[i];
  
  EdbVertex *vertex = 0;

  if (newversion){ 
    vertexrec = new EdbVertexRec();
    vertexrec->SetPVRec(ali);
    vertexrec->eDZmax=DZmax;
    vertexrec->eProbMin=ProbMinV;
    vertexrec->eImpMax=ImpMax;
    vertexrec->eUseMom=UseMom;
    vertexrec->eUseSegPar=UseSegPar;
    vertexrec->eQualityMode=QualityMode;

    vertex = dproc->GetVertexFromTree(*vertexrec,vertexfilename,vID);
  }
  else{
    vertexrec = (EdbVertexRec*) inputfile->Get("EdbVertexRec");
    vertex = (EdbVertex*) vertexrec->eVTX->At(vID);
  }

  vertex->SetXYZ(vertex->VX(),vertex->VY(),vertex->VZ()); //EDA USES X(), NOT VX(). Here I want the display to match my coordinates
  drawnvertices->Add(vertex); // assuming the array is filled with EdbVertex.
  for (int itrk = 0; itrk < vertex->N(); itrk++){
     EdbTrackP* track =  vertex->GetTrack(itrk);     
     for (int iseg = 0; iseg < track->N(); iseg++) track->GetSegment(iseg)->SetFlag(vertexcolors[i]); // to color them differently
 //    for (int iseg = 0; iseg < track->N(); iseg++) track->GetSegment(iseg)->SetFlag(vertexcolors[i]); // to color them differently
     drawntracks->Add(track);
//     specialtrack = track;
  }
 }

 ali->eTracks = drawntracks;
 ali->eVTX = drawnvertices;
 EdbEDA * eda = new EdbEDA(ali); // init DataSet but doesn't read linked_track.root
 eda->GetTrackSet("TS")->SetColorMode(kCOLOR_BY_ID);
/* eda->GetTrackSet("TS")->RemoveTrack(specialtrack); //charm colored differently
 eda->GetTrackSet("SB")->SetTrackAttribute(4);
 eda->GetTrackSet("SB")->AddTrack(specialtrack);*/
 eda->Run();
}

void drawEDAvertices(bool newversion = true, TString vertexfilename= "vertextree.root", bool applycut = false){
 using namespace VERTEX_PAR;
 TFile * inputfile = TFile::Open(vertexfilename.Data());
 TTree *vtxtree = (TTree*) inputfile->Get("vtx");
 EdbVertexRec *vertexrec;
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
 const int nvertices = vtxtree->GetEntries();
 cout<<"Reading number of vertices: "<<nvertices<<endl;
 map<int,EdbTrackP*>emptymap;

 if (newversion){
  dproc->ReadVertexTree(*vertexrec, "vertextree.root", "1",emptymap);
 }

 for (int i = 0; i < nvertices; i++){ //range for loop, C++11
  int vID = i;

  if (newversion){
    vertex = (EdbVertex*) ((TObjArray*)(ali->eVTX)) ->At(vID);
  } 
  else{ 
    vertexrec = (EdbVertexRec*) inputfile->Get("EdbVertexRec");
    vertex = (EdbVertex*) vertexrec->eVTX->At(vID);
  }
  if(applycut){
   if (vertex->N() > 10) drawnvertices->Add(vertex);
   for (int itrk = 0; itrk < vertex->N(); itrk++){
     EdbTrackP* track =  vertex->GetTrack(itrk);          
     if (vertex->N() > 10) drawntracks->Add(track);
   }
  }
  else{
  	drawnvertices->Add(vertex); // assuming the array is filled with EdbVertex.
  	for (int itrk = 0; itrk < vertex->N(); itrk++){
    	EdbTrackP* track =  vertex->GetTrack(itrk);          
 //    	for (int iseg = 0; iseg < track->N(); iseg++) track->GetSegment(iseg)->SetFlag(vertexcolors[i]); // to color them differently
     	drawntracks->Add(track);
//     	specialtrack = track;
  	}
  }
 }

 ali->eTracks = drawntracks;
 ali->eVTX = drawnvertices;
 EdbEDA * eda = new EdbEDA(ali); // init DataSet but doesn't read linked_track.root
 //eda->GetTrackSet("TS")->SetColorMode(kBLACKWHITE);
 eda->Run();
}


void find_vertices_event(int myevent, TString vertexfilename= "vertextree.root"){
 using namespace VERTEX_PAR;
 TFile * inputfile = TFile::Open(vertexfilename.Data());
 TTree *vtxtree = (TTree*) inputfile->Get("vtx");
 EdbVertexRec *vertexrec;
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

 map<int,EdbTrackP*>emptymap;
 const int nvertices = vtxtree->GetEntries();
 cout<<"Reading number of vertices: "<<nvertices<<endl;
 dproc->ReadVertexTree(*vertexrec, "vertextree.root", "1",emptymap);
 
 EdbVertex *vertex = 0;
 
 for (int i = 0; i < nvertices; i++){ //range for loop, C++11
  int vID = i;
  
  vertex = (EdbVertex*) ((TObjArray*)(ali->eVTX)) ->At(vID);
  
  bool oureventvertex = false;
  for (int itrk = 0; itrk < vertex->N(); itrk++){
     EdbTrackP* track =  vertex->GetTrack(itrk);          
     if (FindMostCommonEvent(track) == myevent){
      drawntracks->Add(track);
      oureventvertex = true;
      }
   }
   if(oureventvertex) drawnvertices->Add(vertex);
 }

 ali->eTracks = drawntracks;
 ali->eVTX = drawnvertices;
 EdbEDA * eda = new EdbEDA(ali); // init DataSet but doesn't read linked_track.root
 //eda->GetTrackSet("TS")->SetColorMode(kBLACKWHITE);
 eda->Run();
}

int FindMostCommonEvent(EdbTrackP *track){

  map <int, int> nseg_MCEvt;
  int mostcommonevent = -1;
  int maxnsegments = -1;
  //looping over track segments, counting them
  for (int iseg = 0; iseg < track->N(); iseg++){

    EdbSegP * myseg = track->GetSegment(iseg);
    nseg_MCEvt[myseg->MCEvt()]++; 
  }
  //which MCEvent has the most segments?

  for (auto && [MCEvent,nsegments]:nseg_MCEvt){
    //find maxnsegments
    if (nsegments > maxnsegments){
      maxnsegments = nsegments;
      mostcommonevent = MCEvent;
    } //end if condition

  } //end loop on map
  return mostcommonevent;
}
