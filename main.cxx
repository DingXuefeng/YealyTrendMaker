#include "TrendData.h"
#include "TrendMaker.h"
#include <iostream>
#include "Factory.h"
#include "TString.h"
#include <cassert>
#include <fstream>
class TGraphErrors;
void config_POI();  // paramters of interests
void load_data(TrendMaker *maker,char *argv[]);
int main(int argc,char *argv[]) {
  config_POI();

    
  assert(argc==6);
  // argv[1]: var. say, period
  // argv[2]: name of the project, affecting name of the output tex file
  // argv[3]: output folder
  // argv[4]: inputlist
  // argv[5]: skip p-value?
  TrendMaker *maker = Factory<TrendMaker>::get()->create(argv[1]);
  maker->set_output_path(argv[2]/*name of the project*/,argv[3]/*output path*/,true/*make latex*/);
  load_data(maker,argv);
  maker->make_plots(atoi(argv[5]));

  return 0;
}
void config_POI() { // paramters of interests
  TrendDataImpl::reg("likelihood_p_value","p-val",-0.1,1.1);
  //TrendDataImpl::reg("chi2_ndf","#chi^{2} / NDF",0.5,2);
  TrendDataImpl::reg("beta_ly","LY (p.e./MeV)",460,560);
  TrendDataImpl::reg("nu_Be7_rate","#nu ^{7}Be (cpd/100t)",35,60);
//  TrendDataImpl::reg("nu_pp_rate","#nu pp (cpd/100t)");
  TrendDataImpl::reg("Bi210_rate","^{210}Bi (cpd/100t)",0,40);
  TrendDataImpl::reg("Kr85_rate","^{85}Kr (cpd/100t)",0,30);
  TrendDataImpl::reg("Po210_rate","^{210}Po (cpd/100t)");
  TrendDataImpl::reg("Po210_quenching","^{210}Po qch (cpd/100t)",0.07,0.085);
//  TrendDataImpl::reg("C14_rate","^{14}C (cpd/100t)");
//  TrendDataImpl::reg("synthpup_rate","synthpup (cpd/100t)");
  TrendDataImpl::reg("C11_rate","^{11}C (cpd/100t)",0,50);
  TrendDataImpl::reg("C11_2_rate","^{11}C (cpd/100t)",50,70);
  TrendDataImpl::reg("Ext_Tl208_rate","Ext ^{208}Tl (cpd/100t)",0,10);
  TrendDataImpl::reg("Ext_Bi214_rate","Ext ^{214}Bi (cpd/100t)",0,10);
  TrendDataImpl::reg("Ext_K40_rate","Ext ^{40}K (cpd/100t)",0,10);
  TrendDataImpl::reg("C11_quenching","^{11}C qch (cpd/100t)",0.8,1);
  //TrendDataImpl::regArray("beta_resolution","g1","g2","g3");
  //TrendDataImpl::regArray_min("beta_resolution",1,0,0);
  //TrendDataImpl::regArray_max("beta_resolution",2,6,3);
  TrendDataImpl::regCorrelation("Kr85_rate","Bi210_rate");
  TrendDataImpl::regCorrelation("C11_quenching","Bi210_rate");
  TrendDataImpl::regCorrelation("Po210_quenching","Bi210_rate");
  TrendDataImpl::regCorrelation("nu_Be7_rate","Bi210_rate");
  TrendDataImpl::regCorrelation("beta_ly","Bi210_rate");
  TrendDataImpl::regCorrelation("Kr85_rate","nu_Be7_rate");
  TrendDataImpl::regCorrelation("C11_quenching","beta_ly");
}
void load_data(TrendMaker *maker,char *argv[]) {
  std::ifstream inputlists;
  inputlists.open(argv[4]);
  while(true) {
    std::string filename,project;
    inputlists >> filename >> project;
    if(inputlists.fail()) break;
    TrendData *data = Factory<TrendData>::get()->create(project);
    std::ifstream inputlist;
    inputlist.open(filename);
    while(true) {
      int year;
      std::string file;
      inputlist>>year>>file;
      if(inputlist.fail()) 
        break;
      data->addFiles(year,"data/"+(project+"/")+file);
    }
    inputlist.close();
    maker->add_data(data);
  }
  inputlists.close();
}
