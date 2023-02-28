//from the files, prepare a map (should be done online)
void preparecorrectionmap(){
 EdbScanProc *scanproc = new EdbScanProc();
 scanproc->eProcDirClient = ".."; //as usual
 EdbCorrectionMap *linkingmap_bot = new EdbCorrectionMap();
 EdbCorrectionMap *linkingmap_top = new EdbCorrectionMap();
 int nx = 19;
 int ny = 19;
 linkingmap_bot->Init(nx,0,190000,ny,0,190000);
 linkingmap_top->Init(nx,0,190000,ny,0,190000);
 //loop to fill map
 for(int ix = 0; ix < nx;ix++){
  for (int iy = 0; iy < ny; iy++){
   //adapted from ReadPiecePar
   EdbDataPiece piece;
   TString str;
   EdbID id(331,15,ix,iy);
   scanproc->MakeFileName(str,id,"par");
   piece.ReadPiecePar(str);
   for(int i=0; i<3; i++) { //loop top bottom
    EdbLayer *l=piece.GetLayer(i);
    if(l) { //do I have a layer?
     if(i==1) linkingmap_bot->GetLayer(ix*10000. + 5000.,iy * 10000. + 5000.)->ApplyCorrections(*l); //then I insert into the map    
     if(i==2) linkingmap_top->GetLayer(ix*10000. + 5000.,iy * 10000. + 5000.)->ApplyCorrections(*l); //then I insert into the map         
    }//end if
   }//end for top bottom
  } //end for y
 } //end for x
 TFile *outputfile = new TFile("testmap.root","RECREATE");
 linkingmap_bot->Write("map_bot");
 linkingmap_top->Write("map_top");
}//end program
