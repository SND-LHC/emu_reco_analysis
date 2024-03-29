#include <iostream>
#include <vector>

//Computing volume track probability from base-track probabilities
//Combination part derived from 
//https://stackoverflow.com/questions/12991758/creating-all-possible-k-combinations-of-n-items-in-c
//Launch with
//.L combination_basetracks_efficiencies.C
//main()
//global variables
using namespace std;

vector<int> plates;
vector<int> combination;

int countiterations = 0;


const int nfilms = 25; //number of films
const int firstfilm = 15; //starting from
const int lastfilm = 39; //ending into
//should we take into account holes?
const int min_nfilms = 12; //how many films we require

TString eff_filename("efficiency_plate_RUN1_onesquarecm_39_15.root");

double plateefficiency[nfilms];

void SetEfficiencyArray(){
    //TFile *inputfile = TFile::Open("efficiency_plate_requiredallfilms.root");
    TFile *inputfile = TFile::Open(eff_filename.Data());
    TEfficiency *heffplates = (TEfficiency*) inputfile->Get("hexpected_clone");
    cout<<"Starting code, getting efficiency from tgraph"<<endl;
    for (int ifilm = 0; ifilm < nfilms; ifilm++){
        int nfilm = ifilm + firstfilm;
        plateefficiency[ifilm] = heffplates->GetEfficiency(nfilm+1); //first bin is 0
        cout<<"Efficiency for film "<<nfilm<<": "<<plateefficiency[ifilm]<<endl;
    }
    cout<<"Average efficiency: "<<TMath::Mean(nfilms,plateefficiency)<<"dev st: "<<TMath::StdDev(nfilms,plateefficiency)<<" con errore "<<TMath::StdDev(nfilms,plateefficiency)/TMath::Sqrt(nfilms)<<endl;

}

double pevent(const vector<int>&combination){ 
 //probability of having a particular combination of base-tracks
 //if base-track in a plate is there, multiply for eff[film], otherwise multiply for 1-eff[film]   
 double prob = 1.;
 bool filmfound[nfilms]; //will tell us if film contains a basetrack
 std::fill_n(filmfound,nfilms,false); //should be already false by default, but let us initialize it for safety
 //looping over all films, storing the ones "found" in the combination
 for (int iplate:combination){
    filmfound[iplate] = true; 
 }
 for (int ifilm = 0; ifilm < nfilms; ifilm++){
    if (filmfound[ifilm]) prob = prob * plateefficiency[ifilm]; //plate was found, p;
    else prob = prob * (1-plateefficiency[ifilm]); //plate was not found, 1-p;
 }
 cout<<"prob "<<prob<<endl;
 return prob;
}

void pretty_print(const vector<int>& v) {
  //static int count = 0;
  cout << "combination no " << (++countiterations) << ": [ ";
  for (int i = 0; i < v.size(); ++i) { cout << v[i] + firstfilm << " "; }
  cout << "] ";
}
double totprob = 0.; //global variabile, reporting total probability

void go(int offset, int k) {

  if (k == 0) {
    //print the combination for check
    pretty_print(combination);
    //compute probability of this combination, add to total probability
    totprob += pevent(combination);
    return;
  }
  for (int i = offset; i <= plates.size() - k; ++i) {
    combination.push_back(plates[i]);
    go(i+1, k-1);
    combination.pop_back();
  }
}

int main() {
  int n = nfilms;
  int mink = min_nfilms;
  SetEfficiencyArray();
  double probiteration = 0;
  for (int i = 0; i < n; ++i) { plates.push_back(i); }
  for (int k = mink; k <= n; ++k) {
    cout<<"Starting combinations of "<<k<<" plates over "<<n<<endl;
    go(0, k);countiterations = 0;
    cout<<"Probability of having "<<k<<" plates over "<<n<<": "<<totprob - probiteration<<endl;
    probiteration = totprob;
    }
  cout<<totprob<<endl;
  
  return 0;
}

vector<double> eff_ClopperPears(int foundevents, int totalevents){
  //using Clopper Pearson confidence limits, more reliable and recommended by Lista and PDG
  vector<double> efficiency; //value, lower limit, upper limit

  efficiency.push_back((double) foundevents/totalevents);

  double conflevel = 0.683; //default, 1 sigma

  //computing lower and upper limits
  double efflow = TEfficiency::ClopperPearson(totalevents,foundevents,conflevel,false);
  efficiency.push_back(efflow);
  double effhigh = TEfficiency::ClopperPearson(totalevents,foundevents,conflevel,true);
  efficiency.push_back(effhigh);
  return efficiency;
}

void compute_efferrors(){
  const int ntracks_total = 658220 + 658568 + 610817 + 633040; //tracks with nseg>=18 in the RUN0 sample (sum all 4 quarters);

  const int nefficiencies = 9; //number of samples computed efficiency
  double eff_list[nefficiencies] = {0.09,0.45,1.90,6.25,15.60,27.72,31.22,16.74,99.98};

  cout<<"Efficiency for ntracks total "<<ntracks_total<<endl;
  //loopint over values
  for (int ieff = 0; ieff<nefficiencies;ieff++){
   vector<double> efficiency = eff_ClopperPears(ntracks_total * eff_list[ieff]/100.,ntracks_total);
   cout<<"value "<<efficiency[0]<<" limit: ["<<efficiency[1]<<","<<efficiency[2]<<"]"<<endl;
  }

}
