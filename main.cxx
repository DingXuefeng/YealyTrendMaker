#include "TrendData.h"
#include "TrendMaker.h"
#include <iostream>
#include "Factory.h"
#include "TString.h"
#include <cassert>
#include <fstream>
class TGraphErrors;
void config_POI();  // paramters of interests
void load_data(TrendMaker *maker,const std::string &input) ;
int main(int argc,char *argv[]) {
  config_POI();

    
  assert(argc==2);
  // argv[1]: var. say, period
  // argv[2]: name of the project, affecting name of the output tex file
  // argv[3]: output folder
  // argv[4]: inputlist
  // argv[5]: skip p-value?
  std::ifstream task_list;
  task_list.open(argv[1]);
  std::string var,project,output,input,skip,title,comments;
  while(true) {
    task_list>>var>>project>>output>>input>>skip;
    std::getline(task_list,title);
    std::getline(task_list,title);
    std::getline(task_list,comments);
    if(task_list.fail()) break;
    std::cout<<"./main "<<var<<" "<<project<<" "<<output<<" "<<input<<" "<<skip<<" <"<<title<<"> ["<<comments<<"]"<<std::endl;
    TrendMaker *maker = Factory<TrendMaker>::get()->create(var);
    maker->config_latex(title,comments);
    maker->set_output_path(project/*name of the project*/,output/*output path*/,true/*make latex*/);
    load_data(maker,input);
    maker->make_plots(atoi(skip.c_str()));
  }
  task_list.close();

  return 0;
}
void config_POI() { // paramters of interests
  TrendDataImpl::reg("nu_Be7","#nu ^{7}Be (cpd/100t)",35,60);
//  TrendDataImpl::reg("nu_pp","#nu pp (cpd/100t)");
//  TrendDataImpl::reg("likelihood_p_value","p-val",-0.1,1.1);
//  TrendDataImpl::reg("chi2_ndf","#chi^{2} / NDF",0.5,2);
  //TrendDataImpl::reg("beta_ly","LY (p.e./MeV)",490,515);
//  TrendDataImpl::reg("beta_ly","LY (p.e./MeV)",460,560);
  //TrendDataImpl::reg("beta_ly","LY (p.e./MeV)",530,590); // npmt
//  TrendDataImpl::reg("nu_CNO","#nu CNO (cpd/100t)",0,40);
  TrendDataImpl::reg("Bi210","^{210}Bi (cpd/100t)",0,50);
  TrendDataImpl::reg("Kr85","^{85}Kr (cpd/100t)",0,50);
  TrendDataImpl::reg("Po210","^{210}Po (cpd/100t)",0,800);
  //TrendDataImpl::reg("Po210_quenching","^{210}Po qch (cpd/100t)",0.07,0.085);
//  TrendDataImpl::reg("Po210_quenching","^{210}Po qch (cpd/100t)",0.065,0.08); // npmt
//  TrendDataImpl::reg("C14","^{14}C (cpd/100t)");
//  TrendDataImpl::reg("synthpup","synthpup (cpd/100t)");
  TrendDataImpl::reg("C11","^{11}C (cpd/100t)",0,50);
//  TrendDataImpl::reg("C11","^{11}C (cpd/100t)",0,5);
  TrendDataImpl::reg("2*C11_2","^{11}C (cpd/100t)",0,120);
  TrendDataImpl::reg("MCshiftC11","^{11}C shift (nhit)",-10,10);
  TrendDataImpl::reg("MCscaleC11","^{11}C scale",0.95,1.05);
  TrendDataImpl::reg("MCshiftPo210","^{210}Po shift (nhit)",-5,5);
  TrendDataImpl::reg("MCscalePo210","^{210}Po scale",0.95,1.05);
  //TrendDataImpl::reg("Ext_Tl208","Ext ^{208}Tl (cpd/100t)",0,10);
  //TrendDataImpl::reg("Ext_Bi214","Ext ^{214}Bi (cpd/100t)",0,10);
  //TrendDataImpl::reg("Ext_K40","Ext ^{40}K (cpd/100t)",0,10);
 //TrendDataImpl::reg("C11_quenching","^{11}C qch (cpd/100t)",0.8,1);
  //TrendDataImpl::regArray("beta_resolution","g1","g2","g3");
  //TrendDataImpl::regArray_min("beta_resolution",1,0,0);
  //TrendDataImpl::regArray_max("beta_resolution",2,6,3);
  //TrendDataImpl::regArray("beta_resolution","g1");
  //TrendDataImpl::regArray_min("beta_resolution",1);
  //TrendDataImpl::regArray_max("beta_resolution",2);
//  TrendDataImpl::regArray("beta_resolution","#beta_{0}");
//  TrendDataImpl::regArray_min("beta_resolution",0);
//  TrendDataImpl::regArray_max("beta_resolution",40);
 // TrendDataImpl::regArray("mlp_corrections","#varepsilon_{#beta}","#lambda_{#beta}","#varepsilon_{#alpha}","#lambda_{#alpha}");
 // TrendDataImpl::regArray_min("mlp_corrections",0,10,0,10);
 // TrendDataImpl::regArray_max("mlp_corrections",1,100,1,100);
 // TrendDataImpl::regCorrelation("Kr85","Bi210",-0.57);
  //TrendDataImpl::regCorrelation("C11_quenching","Bi210");
  //TrendDataImpl::regCorrelation("Po210_quenching","Bi210");
  //TrendDataImpl::regCorrelation("nu_Be7","Bi210");
 // TrendDataImpl::regCorrelation("beta_ly","Bi210",-0.71);
  //TrendDataImpl::regCorrelation("Ext_K40","Bi210");
  //TrendDataImpl::regCorrelation("Kr85","nu_Be7");
  //TrendDataImpl::regCorrelation("C11_quenching","beta_ly");
}
void load_data(TrendMaker *maker,const std::string &input) {
  std::ifstream inputlists;
  inputlists.open(input.c_str());
  while(true) {
    std::string filename,project;
    inputlists >> filename >> project;
    if(inputlists.fail()) break;
    TrendData *data = Factory<TrendData>::get()->create(project);
    std::ifstream inputlist;
    inputlist.open(filename.c_str());
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
