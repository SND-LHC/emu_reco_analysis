//Saving linking and alignment reports (first implementation in a general way on 19 December 2018)
//in the b000001 folder, there should be already a folder called 
//plots with subfolders: thicknesses, link_reports, al_reports
//TString: class which allows path concatenation. Access the char* object with Data()
const int brick = 24; //3 with large angles
const int firstplate = 7;
const int lastplate = 7;

TString path = TString("/home/scanner/sndlhc/RUN1");


//TString run = "GSI5";
//TString brick = TString(run.Data()[3]); //syntax is GSI1,GSI2,GSI3,GSI4

void couplesweight(){
   for (int iplate = firstplate; iplate <= lastplate; iplate++){
        TFile *inputfile;
        TTree *couples;
        
        inputfile = TFile::Open(Form((path+"/b%06i/p%03d/%i.%i.0.0.cp.root").Data(),brick,iplate,brick,iplate));
        if (inputfile){
          if (inputfile->Get("couples")){
           couples = (TTree*) inputfile->Get("couples");
                
            TCanvas *c = new TCanvas();
            couples->Draw("s.eW");
            c->Print(Form((path+"/b%06i/plots/couples_weight/cp_p%i.png").Data(),brick,iplate),"png");

            }
        }

   } //end loop ocver plates
}

void couplesecography(){
    //check for quality of couples for alignment, as suggested by Dario
    TCanvas *c = new TCanvas("c","cxy couples quality",800,800);
    for (int iplate = firstplate; iplate <= lastplate; iplate++){
        
        TFile *inputfile;
        TTree *couples;
        
        inputfile = TFile::Open(Form((path+"/b%06i/p%03d/%i.%i.0.0.cp.root").Data(),brick,iplate,brick,iplate));
        
        
        if(inputfile){
            if (inputfile->Get("couples")){
                couples = (TTree*) inputfile->Get("couples");
                
                couples->Draw("s.eY:s.eX","eCHI2P<2.0&&s.eW>10&&eN1==1&&eN2==1");
                c->Print(Form((path+"/b%06i/plots/goodcouples/goodcp_p%i.png").Data(),brick,iplate),"png");

            }
        }
    }//end loop on files
}

void linkreports(){
    TFile *inputfile;
    TCanvas *c;
    for (int iplate = firstplate; iplate <= lastplate; iplate++){
        //opening the file with the reports
        inputfile = TFile::Open(Form((path+"/b%06i/p%03d/%i.%i.0.0.cp.root").Data(),brick,iplate,brick,iplate));
	if (inputfile){
 	 if (inputfile->Get("report")){
          c = (TCanvas*) inputfile->Get("report");
        
          /* //saving a full pdf report (very heavy)
          if (i==firstplate) c->Print((path+"/b%06i/plots/link_reports/linking.pdf(").Data(),"pdf");
          else if (i==lastplate) c->Print((path+"/b%06i/plots/link_reports/linking.pdf)").Data(),"pdf");
          else c->Print((path+"/b%06i/plots/link_reports/linking.pdf").Data(),"pdf");*/
        
          //saving many png images for quick view
          c->Draw();
          c->Print(Form((path+"/b%06i/plots/link_reports/link_p%i.png").Data(),brick,iplate),"png");
         }
         else cout<<"coul not open report for plate "<<iplate<<endl;        
         //close the file
         inputfile->Close();
	}
    }
}

void alreports(){
    TFile *inputfile;
    TCanvas *c;
    for (int i = firstplate; i <= lastplate; i++){
        if (i == firstplate) continue;
        //opening the file with the report
        inputfile = TFile::Open(Form((path+"/b%06i/AFF/%i.%i.0.0_%i.%i.0.0.al.root").Data(),brick,brick,i,brick,i-1));
        c = (TCanvas*) inputfile->Get("report_al");
        c->Draw();
        
        //saving many png images for quick view
        c->Print(Form((path+"/b%06d/plots/al_reports/alignment_p%i.png").Data(),brick,i,i-1));
        inputfile->Close();
    }
}

//draw all thickness
#include "thickness.C"
#include "check_raw.C"
void thickness();

void drawallthicknesses(){
    
    double meanthickness_base = 0.;
    double meanthickness_emu = 0.;
    
    for (int i = firstplate; i <= lastplate; i++){
        TFile *f = TFile::Open(Form((path+"/b%06d/p%03d/%i.%i.0.0.raw.root").Data(),brick,i,brick,i));
        if (!f) continue;
        if (!f->Get("Views")){ cout<<"Not found tree Views, raw file probably corrupted, skipping"<<endl; continue;}
        thickness();
        gROOT->GetSelectedPad()->GetCanvas()->SetName(Form("canvas%d",i));
        TCanvas *thickcanvas = (TCanvas*) gROOT->GetSelectedPad()->GetCanvas()->GetPrimitive("diff_3");
        //getting the three histograms
        TH1F *hup = (TH1F*) thickcanvas->GetPrimitive("up");
        TH1F *hdown = (TH1F*) thickcanvas->GetPrimitive("down");
        TH1F *hbase = (TH1F*) thickcanvas->GetPrimitive("base");
        
        hup->GetXaxis()->SetRange(10,300);
        hbase->GetXaxis()->SetRange(10,300);
        hdown->GetXaxis()->SetRange(10,300);
        
        cout<<"Plate number: "<<i<<endl;
        cout<<"Thickness top layer: "<<hup->GetMean()<<" with RMS "<<hup->GetRMS()<<endl;
        cout<<"Thickness plastic base: "<<hbase->GetMean()<<" with RMS "<<hbase->GetRMS()<<endl;
        cout<<"Thickness bottom layer: "<<hdown->GetMean()<<" with RMS "<<hdown->GetRMS()<<endl;
        
        meanthickness_emu += hup->GetMean();
        meanthickness_base += hbase->GetMean();
        meanthickness_emu += hdown->GetMean();
        
        TCanvas *canvas = (TCanvas*) gROOT->GetSelectedPad()->GetCanvas();
        canvas->Print(Form((path+"/b%06d/plots/thicknesses/thickness_plate%i.png").Data(),brick,i));
        
    }
    meanthickness_emu = meanthickness_emu/((lastplate - firstplate + 1)*2);
    meanthickness_base = meanthickness_base/(lastplate -firstplate +1);
    cout<<"Average thickness of emulsion: "<<meanthickness_emu<<endl;
    cout<<"Average thickness of plastic base: "<<meanthickness_base<<endl;
    //gROOT->GetSelectedPad()->GetCanvas()->Print((path+"/b%06i/plots/thicknesses/allthicknesses.pdf)").Data(),"pdf");
}

void drawallraws(){
  TCanvas *cz = NULL;
  TCanvas *cview = NULL;
  TCanvas *csurf = NULL;
  for (int i = firstplate; i <= lastplate; i++){
	TFile *f = TFile::Open(Form((path+"/b%06d/p%03d/%i.%i.0.0.raw.root").Data(),brick,i,brick,i));
        if (!f) continue;
        if (!f->Get("Views")){ cout<<"Not found tree Views, raw file probably corrupted, skipping"<<endl; continue;}
	check_raw();
	//getting canvases; printing them
        TCanvas *cz = (TCanvas*) gROOT->FindObject("cz");
	TCanvas *cview = (TCanvas*) gROOT->FindObject("view");
        TCanvas *csurf = (TCanvas*) gROOT->FindObject("csurf");

        cz->Print(Form((path+"/b%06d/plots/raws/checkz_plate%i.png").Data(),brick,i));
	cview->Print(Form((path+"/b%06d/plots/raws/checkview_plate%i.png").Data(),brick,i));
	csurf->Print(Form((path+"/b%06d/plots/raws/checksurf_plate%i.png").Data(),brick,i));

	//closing canvases
	cz->Close();
	cview->Close();
	csurf->Close();

  }
}

