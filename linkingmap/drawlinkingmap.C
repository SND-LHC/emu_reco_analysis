void drawlinkingmap(){
 TFile *inputfile = TFile::Open("/home/scanner/sndlhc/RUN0/b000331/testmap.root");
 EdbCorrectionMap *map_bot = (EdbCorrectionMap*) inputfile->Get("map_bot");
 EdbCorrectionMap *map_top = (EdbCorrectionMap*) inputfile->Get("map_top");

 EdbCorrectionMapper *mapper = new EdbCorrectionMapper();
 const int nx =19, ny=19;
 for(int ix = 0; ix < nx;ix++){
  for (int iy = 0; iy < ny; iy++){
   //accessing layer from the map
   EdbLayer *bot_layer = map_bot->GetLayer(ix*10000. + 5000.,iy * 10000. + 5000.);
   EdbLayer *top_layer = map_top->GetLayer(ix*10000. + 5000.,iy * 10000. + 5000.);
   //modifications ONLY FOR THE DRAWING (do not use in real data);
   bot_layer->SetZcorr(bot_layer->Shr()); //then I insert into the map
   bot_layer->SetNcp(int(bot_layer->Shr()*1000.)); //just for test
   top_layer->SetZcorr(top_layer->Shr());
   top_layer->SetNcp(int(top_layer->Shr()*1000.)); //just for test
  }
 }

 gStyle->SetOptStat("t");
 
 mapper->DrawMap(*map_bot,"bot");
 TH2D* hmapDZ_bot = (TH2D*) gDirectory->FindObject("mapDZ_bot");
 hmapDZ_bot->GetZaxis()->SetRangeUser(0.6,0.9);
 hmapDZ_bot->SetTitle("Bottom Layer Map;x[#mum];y[#mum]");
 mapper->DrawMap(*map_top,"top");
 TH2D* hmapDZ_top = (TH2D*) gDirectory->FindObject("mapDZ_top");
 hmapDZ_top->GetZaxis()->SetRangeUser(0.6,0.9);
 hmapDZ_top->SetTitle("Top Layer Map;x[#mum];y[#mum]");
}
