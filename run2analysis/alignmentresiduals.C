//REDRAWING ALIGNMENT RESIDUALS (RE-CREATED 14 October 2022)

void alignmentresiduals(){
 //accessing file
 EdbCouplesTree *aligncouples = new EdbCouplesTree();
 aligncouples->InitCouplesTree("couples","/home/scanner/sndlhc/RUN2/test_offlinescans/b000024/AFF/24.8.0.0_24.7.0.0.al.root","READ");
 TFile *inputfile = TFile::Open("/home/scanner/sndlhc/RUN2/test_offlinescans/b000024/AFF/24.8.0.0_24.7.0.0.al.root","READ");

 EdbLayer *la1 = (EdbLayer*) inputfile->Get("corr_layer1"); //corrlayer2 is identical, but let us put it anyway
 EdbLayer *la2 = (EdbLayer*) inputfile->Get("corr_layer2");
 TH2D *hdxy = new TH2D("hdxy","Position residuals;dx[#mum];dy[#mum]",20,-10,10,20,-10,10);
 TH2D *hdxy_original = new TH2D("hdxy_original","Position residuals;dx[#mum];dy[#mum]",9,-120,120,9,-120,120);
 TH2D *hdtxty = new TH2D("hdtxty","Angular residuals;dtx[mrad];dty[mrad]",20,-0.01,0.01,20,-0.01,0.01);
 //with respect to coordinate
 //TH2D *hdx_x = new TH2D("hdx_x","Position residuals;x2[#mum];dx[#mum]",100,91000,101000,20,-10,10);
 //TH2D *hdx_y = new TH2D("hdx_y","Position residuals;y2[#mum];dx[#mum]",100,91000,101000,20,-10,10);
 //TH2D *hdy_x = new TH2D("hdy_x","Position residuals;x2[#mum];dy[#mum]",100,91000,101000,20,-10,10);
 //TH2D *hdy_y = new TH2D("hdy_y","Position residuals;y2[#mum];dy[#mum]",100,91000,101000,20,-10,10);

 TProfile *hdx_x = new TProfile("hdx_x","dx vs x;x2[#mum];dx[#mum]",100,91000,101000,-10,10);
 TProfile *hdx_y = new TProfile("hdx_y","dx vs y;y2[#mum];dx[#mum]",100,91000,101000,-10,10);
 TProfile *hdy_x = new TProfile("hdy_x","dy vs x;x2[#mum];dy[#mum]",100,91000,101000,-10,10);
 TProfile *hdy_y = new TProfile("hdy_y","dy vs y;y2[#mum];dy[#mum]",100,91000,101000,-10,10);

 //TH2D *hdtx_x = new TH2D("hdtx_x","dtx vs x;x2[#mum];dtx",100,91000,101000,20,-0.01,0.01);
 //TH2D *hdtx_y = new TH2D("hdtx_y","dtx vs y;y2[#mum];dtx",100,91000,101000,20,-0.01,0.01);
 //TH2D *hdty_x = new TH2D("hdty_x","dty vs x;x2[#mum];dty",100,91000,101000,20,-0.01,0.01);
 //TH2D *hdty_y = new TH2D("hdty_y","dty vs y;y2[#mum];dty",100,91000,101000,20,-0.01,0.01);

 TH2D *hdtx_tx = new TH2D("hdtx_tx","dtx vs tx;TX2;dtx",200,-0.1,0.1,20,-0.01,0.01);
 TH2D *hdtx_ty = new TH2D("hdtx_ty","dtx vs ty;TY2;dtx",200,-0.1,0.1,20,-0.01,0.01);
 TH2D *hdty_tx = new TH2D("hdty_tx","dty vs tx;TX2;dty",200,-0.1,0.1,20,-0.01,0.01);
 TH2D *hdty_ty = new TH2D("hdty_ty","dty vs ty;TY2;dty",200,-0.1,0.1,20,-0.01,0.01);

 TProfile *hdtx_x = new TProfile("hdtx_x","dtx vs x;x2[#mum];dtx",1000,0,190000,-1,1);
 TProfile *hdtx_y = new TProfile("hdtx_y","dtx vs y;y2[#mum];dtx",1000,0,190000,-1,1);
 TProfile *hdty_x = new TProfile("hdty_x","dty vs x;x2[#mum];dty",1000,0,190000,-1,1);
 TProfile *hdty_y = new TProfile("hdty_y","dty vs y;y2[#mum];dty",1000,0,190000,-1,1);
 //looping over couples
 int ncouples = aligncouples->eTree->GetEntries();
 for (int i= 0; i < ncouples; i++){
  aligncouples->GetEntry(i);
  //applying actual transformations as in reports
  EdbSegP *s1 = aligncouples->eS1; 
  EdbSegP *s2 = aligncouples->eS2; 
  float dx = la2->X(*s2)-la1->X(*s1); //X() and Y() functions apply both propagation and affine transformations
  float dy = la2->Y(*s2)-la1->Y(*s1);

  float dtx = la2->TX(*s2)-la1->TX(*s1); //X() and Y() functions apply both propagation and affine transformations
  float dty = la2->TY(*s2)-la1->TY(*s1);

  hdxy->Fill(dx,dy);
  hdxy_original->Fill(dx,dy);
  if (s1->Chi2()<0.4 && s2->Chi2()<0.4)
  {
    hdtxty->Fill(dtx,dty);
  }
  //filling with positions
  hdx_x->Fill(la2->X(*s2),dx);
  hdx_y->Fill(la2->Y(*s2),dx);
  hdy_x->Fill(la2->X(*s2),dy);
  hdy_y->Fill(la2->Y(*s2),dy);

  //filling with angles
  hdtx_x->Fill(la2->X(*s2),dtx);
  hdtx_y->Fill(la2->Y(*s2),dtx);
  hdty_x->Fill(la2->X(*s2),dty);
  hdty_y->Fill(la2->Y(*s2),dty);

  //angular residual, with angles
  hdtx_tx->Fill(la2->TX(*s2),dtx);
  hdtx_ty->Fill(la2->TY(*s2),dtx);
  hdty_tx->Fill(la2->TX(*s2),dty);
  hdty_ty->Fill(la2->TY(*s2),dty);
  
 } 
 //drawing histogram
 TCanvas *cxy = new TCanvas("cxy","xy residual",800,800);
 hdxy->Draw("COLZ"); 
 TCanvas *ctxty = new TCanvas("ctxty","txty residual",800,800);
 hdtxty->Draw("COLZ");

 TCanvas *cxy1D = new TCanvas("cxy1D","1D distributions");

 TF1 *fdx = new TF1("fdx","pol0(0)+gaus(1)",-30,10);
 fdx->SetParameter(0,7000);
 fdx->SetParameter(1,3000);
 fdx->SetParameter(2,10);
 fdx->SetParameter(3,20);
 fdx->SetParNames("p0","N","mean","sigma");

 cxy1D->Divide(1,2);
 cxy1D->cd(1);
 TH1D *hdx = hdxy->ProjectionX();
 hdx->Draw();
 hdx->Fit("gaus");

 TF1 *fdy = new TF1("fdy","pol0(0)+gaus(1)",-20,20);
 fdy->SetParameter(0,7000);
 fdy->SetParameter(1,3000);
 fdy->SetParameter(2,0);
 fdy->SetParameter(3,20);
 fdy->SetParNames("p0","N","mean","sigma");

 cxy1D->cd(2);
 TH1D *hdy = hdxy->ProjectionY();
 hdy->Draw();
 hdy->Fit("gaus");

 TCanvas *cdxymaps = new TCanvas();
 cdxymaps->Divide(2,2);
 cdxymaps->cd(1);
 hdx_x->GetYaxis()->SetRangeUser(-4,4);
 hdx_x->Draw();
 cdxymaps->cd(2);
 hdx_y->GetYaxis()->SetRangeUser(-4,4);
 hdx_y->Draw();
 cdxymaps->cd(4);
 hdy_x->GetYaxis()->SetRangeUser(-4,4);
 hdy_x->Draw(); 
 cdxymaps->cd(3);
 hdy_y->GetYaxis()->SetRangeUser(-4,4);
 hdy_y->Draw();

 TCanvas *cdtxtymaps = new TCanvas();
 cdtxtymaps->Divide(2,2);
 cdtxtymaps->cd(1);
 hdtx_x->GetYaxis()->SetRangeUser(-0.01,0.01);
 hdtx_x->Draw();
 cdtxtymaps->cd(2);
 hdtx_y->GetYaxis()->SetRangeUser(-0.01,0.01);
 hdtx_y->Draw();
 cdtxtymaps->cd(4);
 hdty_x->GetYaxis()->SetRangeUser(-0.01,0.01);
 hdty_x->Draw(); 
 cdtxtymaps->cd(3);
 hdty_y->GetYaxis()->SetRangeUser(-0.01,0.01);
 hdty_y->Draw();

 TCanvas *cxy_original = new TCanvas("cxy_original","xy residual",800,800);
 //hdxy_original->SetMinimum(10290); //values from the actual plot
 //hdxy_original->SetMaximum(30583);
 hdxy_original->Draw("COLZ");

 TCanvas *cdtxty_angularmaps = new TCanvas();
 cdtxty_angularmaps->Divide(2,2);
 cdtxty_angularmaps->cd(1);
 hdtx_tx->Draw("COLZ");
 cdtxty_angularmaps->cd(2);
 hdtx_ty->Draw("COLZ");
 cdtxty_angularmaps->cd(4);
 hdty_tx->Draw("COLZ");
 cdtxty_angularmaps->cd(3);
 hdty_ty->Draw("COLZ");
}
