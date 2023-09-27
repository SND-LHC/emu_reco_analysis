void CheckFrames()
{
  TCut cut("cut","nsg>2000");
  TCanvas *c = new TCanvas("fra","fra",1000,800);
  c->Divide(2,2);
  c->cd(1)->SetGrid();
  FrameAlign->Draw("dx:z1", cut && "side==0","");
  ((TH2F*)gPad->GetPrimitive("htemp"))->GetYaxis()->SetLimits(-1,1);
  ((TH2F*)gPad->GetPrimitive("htemp"))->GetXaxis()->SetTitle("z1[#mum]");
  ((TH2F*)gPad->GetPrimitive("htemp"))->GetYaxis()->SetTitle("dx[#mum]");
  c->cd(2)->SetGrid();
  FrameAlign->Draw("dx:z1", cut &&"side==1","");
  ((TH2F*)gPad->GetPrimitive("htemp"))->GetYaxis()->SetLimits(-1,1);
  ((TH2F*)gPad->GetPrimitive("htemp"))->GetXaxis()->SetTitle("z1[#mum]");
  ((TH2F*)gPad->GetPrimitive("htemp"))->GetYaxis()->SetTitle("dx[#mum]");
  c->cd(3)->SetGrid();
  FrameAlign->Draw("dy:z1", cut && "side==0","");
  ((TH2F*)gPad->GetPrimitive("htemp"))->GetYaxis()->SetLimits(-1.5,1.5);
  ((TH2F*)gPad->GetPrimitive("htemp"))->GetXaxis()->SetTitle("z1[#mum]");
  ((TH2F*)gPad->GetPrimitive("htemp"))->GetYaxis()->SetTitle("dy[#mum]");
  c->cd(4)->SetGrid();
  FrameAlign->Draw("dy:z1", cut && "side==1","");
  ((TH2F*)gPad->GetPrimitive("htemp"))->GetYaxis()->SetLimits(-1.5,1.5);
  ((TH2F*)gPad->GetPrimitive("htemp"))->GetXaxis()->SetTitle("z1[#mum]");
  ((TH2F*)gPad->GetPrimitive("htemp"))->GetYaxis()->SetTitle("dy[#mum]");
}
