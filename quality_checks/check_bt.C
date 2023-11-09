//script to check for angular distribution with respect to position in couples quality
void check_bt()
{
    TCut beam("beam","abs(s.eTY-0.02)<0.025&&abs(s.eTX-0.03)<0.025&&abs(s.eX-95000)<5000&&abs(s.eY-95000)<5000"); //RUN2_Mic5na
  //TCut beam("beam","abs(s.eTY-0.015)<0.025&&abs(s.eTX-0.0)<0.025"); //RUN3_Mic3na

  TCanvas *c = new TCanvas("c","c", 1000,1000);
  c->Divide(2,3);
  c->cd(1);
  couples->Draw("s.eTY:s.eTX",beam,"colz");
  c->cd(2);
  couples->Draw("s.eY:s.eX",beam,"colz");

  c->cd(3);
  couples->Draw("s.eTX:s.eX>>h(500)", beam,"colz");
  c->cd(4);
  couples->Draw("s.eTY:s.eY", beam,"colz");
  c->cd(5);
  couples->Draw("s.eTX:s.eY", beam,"colz");
  c->cd(6);
  couples->Draw("s.eTY:s.eX", beam,"colz");
}
