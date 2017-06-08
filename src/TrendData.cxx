#include "TrendData.h"
#include <iostream>
//#include "Factory.h"
#include "TChain.h"
#include "TGraphErrors.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TLatex.h"
#include "TPave.h"
#include "TF1.h"
#include "TList.h"
#include <cassert>
std::vector<std::string> TrendDataImpl::vars;
std::map<std::string,std::string> TrendDataImpl::names;
std::map<std::string,int> TrendDataImpl::arrays;
std::map<std::string,std::vector<std::string> > TrendDataImpl::array_names;
std::vector<std::string> TrendDataImpl::graph_names;
std::vector<std::string> TrendDataImpl::graph_legend;
std::map<std::string,double> TrendDataImpl::config_ymin;
std::map<std::string,double> TrendDataImpl::config_ymax;
std::multimap<std::string,std::string> TrendDataImpl::correlation_terms;
std::vector<double> TrendDataImpl::correlation_exp;
TrendDataImpl::TrendDataImpl(const std::string &var) :
  m_var(var),label_vars(),file_paths(),
  varIt(label_vars.end()),fIt(file_paths.end()),
  correlation(),graphs()
{}

void TrendDataImpl::addFiles(double label_var,const std::string &file_path) { 
  label_vars.push_back(label_var); 
  file_paths.push_back(file_path); 
  varIt = label_vars.begin();
  fIt = file_paths.begin();
}

bool TrendDataImpl::next(double &label_var,std::string &file_path) const {
//  std::cout<<"next :"<<(varIt==label_vars.end())<<" "<<(fIt==file_paths.end())<<std::endl;
  if(varIt==label_vars.end()) {
    varIt = label_vars.begin();
    fIt = file_paths.begin();
    return false;
  }
  label_var = *varIt; ++varIt;
  file_path = *fIt; ++fIt;
  return true;
}
void TrendDataImpl::reg(const std::string &var,const std::string &name,double min_,double max_) {
  vars.push_back(var);
  names.insert(std::make_pair(var,name));
  config_ymin.insert(std::make_pair(var,min_));
  config_ymax.insert(std::make_pair(var,max_));
}
void TrendDataImpl::regArray(const std::string &var,const std::string &name1,const std::string &name2,const std::string &name3,const std::string &name4,const std::string &name5,const std::string &name6) {
  int size = 0;
  std::vector<std::string> a_names;
  a_names.push_back(name1); a_names.push_back(name2); a_names.push_back(name3); a_names.push_back(name4); a_names.push_back(name5); a_names.push_back(name6);
  for(std::vector<std::string>::const_iterator nameIt = a_names.begin();
      nameIt!=a_names.end();++nameIt) {
    if(*nameIt!="") { ++size; array_names[var].push_back(*nameIt); }
  }
  arrays.insert(std::make_pair(var,size));
  regArray_min(var);
  regArray_max(var);
}

void TrendDataImpl::regArray_min(const std::string & var,double min1,double min2,double min3,double min4,double min5,double min6) {
  std::vector<double> a_mins;
  a_mins.push_back(min1); a_mins.push_back(min2); a_mins.push_back(min3); a_mins.push_back(min4); a_mins.push_back(min5); a_mins.push_back(min6);
  for(int i = 0;i<arrays.at(var);++i) {
    config_ymin[Form("%s_%d",var.c_str(),i)] = a_mins.at(i);
  }
}

void TrendDataImpl::regArray_max(const std::string & var,double max1,double max2,double max3,double max4,double max5,double max6) {
  std::vector<double> a_maxs;
  a_maxs.push_back(max1); a_maxs.push_back(max2); a_maxs.push_back(max3); a_maxs.push_back(max4); a_maxs.push_back(max5); a_maxs.push_back(max6);
  for(int i = 0;i<arrays.at(var);++i) {
    config_ymax[Form("%s_%d",var.c_str(),i)] = a_maxs.at(i);
  }
}

void TrendDataImpl::regCorrelation(const std::string &var1,const std::string &var2,double exp_cor) {
  correlation_terms.insert(std::make_pair(var1,var2));
  correlation_exp.push_back(exp_cor);
}

void TrendDataImpl::make_graphs() {
  std::vector<double> x,ex;
  double label_var(0); std::string file_path;
  TChain *tree = new TChain("fit_results");
  while(next(label_var,file_path)) {
    tree->Add(file_path.c_str());
    if(x.size()>0&&label_var<x.back()) {
      std::cout<<"don't be crazy.. put label var in order!"<<std::endl;
      std::abort();
    }
    x.push_back(label_var);
    ex.push_back(0);
  }

  std::map<std::string,double> var_holder,evar_holder;
  std::map<std::string,std::vector<double> > y,ey;
  for(std::vector<std::string>::const_iterator varIt = vars.begin();
      varIt!=vars.end();++varIt) {
    assert(var_holder.find(*varIt)==var_holder.end()); // to avoid corruption
    var_holder.insert(std::make_pair(*varIt,0));
    y.insert(std::make_pair(*varIt,std::vector<double>()));
    tree->SetBranchAddress(varIt->c_str(),&var_holder.at(*varIt));
    evar_holder.insert(std::make_pair(*varIt,0));
    ey.insert(std::make_pair(*varIt,std::vector<double>()));
    if(tree->GetBranchStatus(("err_"+*varIt).c_str())) {
      tree->SetBranchAddress(("err_"+*varIt).c_str(),&evar_holder.at(*varIt));
    }
  }
  std::map<std::string,std::vector<double> > array_holder,earray_holder;
  std::map<std::string,std::vector<std::vector<double> > > array_y,array_ey;
  for(std::map<std::string,int>::const_iterator arrayIt = arrays.begin();
      arrayIt!=arrays.end();++arrayIt) {
    assert(array_holder.find(arrayIt->first)==array_holder.end()); // to avoid corruption
    array_holder.insert(std::make_pair(arrayIt->first,std::vector<double>(6)));
    array_y.insert(std::make_pair(arrayIt->first,std::vector<std::vector<double> >(arrayIt->second,std::vector<double>())));
    tree->SetBranchAddress(arrayIt->first.c_str(),&(array_holder.at(arrayIt->first)[0]));
    earray_holder.insert(std::make_pair(arrayIt->first,std::vector<double>(6)));
    array_ey.insert(std::make_pair(arrayIt->first,std::vector<std::vector<double> >(arrayIt->second,std::vector<double>())));
    if(tree->GetBranchStatus(("err_"+arrayIt->first).c_str())) {
      tree->SetBranchAddress(("err_"+arrayIt->first).c_str(),&earray_holder.at(arrayIt->first)[0]);
    }
  }

  assert(!(tree->GetEntries()%x.size()));
  int Npertree = tree->GetEntries()/x.size();
  std::vector<double> final_x,final_ex;
  for(int i = 0;i<x.size();++i) {
    tree->GetEntry(i*Npertree+Npertree-1);
    //if(var_holder.at(vars.front())<0.05) continue;
    final_x.push_back(x.at(i));
    final_ex.push_back(ex.at(i));
    for(std::vector<std::string>::const_iterator varIt = vars.begin();
        varIt!=vars.end();++varIt) {
      y.at(*varIt).push_back(var_holder.at(*varIt));
      ey.at(*varIt).push_back(evar_holder.at(*varIt));
    }
    for(std::map<std::string,int>::const_iterator arrayIt = arrays.begin();
        arrayIt!=arrays.end();++arrayIt)
      for(int i = 0;i<arrayIt->second;++i) {
        array_y.at(arrayIt->first).at(i).push_back(array_holder.at(arrayIt->first).at(i));
        array_ey.at(arrayIt->first).at(i).push_back(earray_holder.at(arrayIt->first).at(i));
      }
  }

  for(std::vector<std::string>::const_iterator varIt = vars.begin();
      varIt!=vars.end();++varIt) {
    graphs.push_back(new TGraphErrors(final_x.size(),&final_x[0],&y.at(*varIt)[0],&final_ex[0],&ey.at(*varIt)[0]));
    if(graph_names.size()<graphs.size()) {
      graph_names.push_back(*varIt);
      graph_legend.push_back(names.at(*varIt));
    }
  }
  for(std::map<std::string,int>::const_iterator arrayIt = arrays.begin();
      arrayIt!=arrays.end();++arrayIt)
    for(int i = 0;i<arrayIt->second;++i) {
      graphs.push_back(new TGraphErrors(final_x.size(),&final_x[0],&array_y.at(arrayIt->first).at(i)[0],&final_ex[0],&array_ey.at(arrayIt->first).at(i)[0]));
      if(graph_names.size()<graphs.size()) {
        graph_names.push_back(Form("%s_%d",arrayIt->first.c_str(),i));
        graph_legend.push_back(array_names.at(arrayIt->first).at(i));
      }
    }
  for( auto gr : graphs ) {
    gr->SetName(m_var.c_str());
    gr->SetTitle(m_var.c_str());
  }
  for(auto cor : correlation_terms ) {
    auto get_index = [&] (const std::string &var) { for(int i = 0;i<graph_names.size();++i) if( graph_names.at(i)==var ) { return i; } throw std::runtime_error("cannot find ["+var+"]"); };
    TGraphErrors *gr1 = graphs.at(get_index(cor.first));
    TGraphErrors *gr2 = graphs.at(get_index(cor.second));
    TGraphErrors *gr = new TGraphErrors(final_x.size()-1,gr1->GetY(),gr2->GetY(),gr1->GetEY(),gr2->GetEY());
    correlation.push_back(gr->GetCorrelationFactor());
    bool ok1 = false;
    for(int i = 0;i<final_x.size();++i) {
      if(gr1->GetY()[i]!=gr1->GetY()[0]) { ok1 = true; break; }
    }
    bool ok2 = false;
    for(int i = 0;i<final_x.size();++i) {
      if(gr2->GetY()[i]!=gr2->GetY()[0]) { ok2 = true; break; }
    }
    corrvar_fixed.push_back(ok1&&ok2);
  }
}
TGraphErrors *TrendDataImpl::next_graph(std::string &name,std::string &legend) {
  static std::vector<TGraphErrors*>::const_iterator grIt = graphs.end();
  static std::vector<std::string>::const_iterator grNameIt = graph_names.end();
  static std::vector<std::string>::const_iterator grLegendIt = graph_legend.end();
  if(!graphs.size()) { make_graphs(); grIt = graphs.begin(); grNameIt = graph_names.begin(); grLegendIt = graph_legend.begin(); }
  if(grNameIt==graph_names.end()) {
    name = "";
    grIt = graphs.begin();
    grNameIt = graph_names.begin();
    grLegendIt = graph_legend.begin();
    return nullptr;
  } else {
    name = *(grNameIt++);
    legend = *(grLegendIt++);
    return *(grIt++);
  }
}
