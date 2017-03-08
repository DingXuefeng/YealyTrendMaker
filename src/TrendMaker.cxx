#include "TrendMaker.h"
#include <iostream>
#include "Factory.h"
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
TrendMakerImpl::TrendMakerImpl(const std::string &var) :
  m_var(var),m_out()
{}

bool TrendMakerImpl::next(double &label_var,std::string &file_path) const {
  static std::vector<double>::const_iterator varIt = label_vars.begin();
  static std::vector<std::string>::const_iterator fIt = file_paths.begin();
  if(varIt==label_vars.end()) {
    varIt = label_vars.begin();
    fIt = file_paths.begin();
    return false;
  }
  label_var = *varIt; ++varIt;
  file_path = *fIt; ++fIt;
  return true;
}
void TrendMakerImpl::reg(const std::string &var,const std::string &name) {
  vars.push_back(var);
  names.insert(std::make_pair(var,name));
}
void TrendMakerImpl::regArray(const std::string &var,const std::string &name1,const std::string &name2,const std::string &name3,const std::string &name4,const std::string &name5,const std::string &name6) {
  int size = 0;
  std::vector<std::string> a_names;
  a_names.push_back(name1); a_names.push_back(name2); a_names.push_back(name3); a_names.push_back(name4); a_names.push_back(name5); a_names.push_back(name6);
  for(std::vector<std::string>::const_iterator nameIt = a_names.begin();
      nameIt!=a_names.end();++nameIt) {
    if(*nameIt!="") { ++size; array_names[var].push_back(*nameIt); }
  }
//  if(var=="beta_resolution"||var=="alpha_resolution") assert(size==6);
//  if(var=="mlp_correction") assert(size==4);
  arrays.insert(std::make_pair(var,size));
}
void TrendMakerImpl::make_plots() {
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

  assert(tree->GetEntries()==x.size());
  for(int i = 0;i<x.size();++i) {
    tree->GetEntry(i);
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

  if(make_tex) latexf.open(m_out+"/trend_"+m_var+".tex");
  for(std::vector<std::string>::const_iterator varIt = vars.begin();
      varIt!=vars.end();++varIt)
    draw_on_pad(*varIt,names.at(*varIt),x,y.at(*varIt),ex,ey.at(*varIt));
  for(std::map<std::string,int>::const_iterator arrayIt = arrays.begin();
      arrayIt!=arrays.end();++arrayIt)
    for(int i = 0;i<arrayIt->second;++i)
      draw_on_pad(Form("%s_%d",arrayIt->first.c_str(),i),array_names.at(arrayIt->first).at(i),x,array_y.at(arrayIt->first).at(i),ex,array_ey.at(arrayIt->first).at(i));
  if(make_tex) latexf.close();
}
#include "TROOT.h"
void TrendMakerImpl::draw_on_pad(const std::string &name,const std::string &legend,const std::vector<double> &x,const std::vector<double> &y,const std::vector<double> &ex,const std::vector<double> &ey) {
  if(legend.find("cpd")!=std::string::npos&&y.front()>86400) {
    std::vector<double> newy,newey;
    std::vector<double>::const_iterator yIt = y.begin();
    std::vector<double>::const_iterator eyIt = ey.begin();
    for(;yIt!=y.end();++yIt,++eyIt) {
      newy.push_back(*yIt/86400);
      newey.push_back(*eyIt/86400);
    }
    std::cout<<std::string(legend).replace(legend.find("cpd"),3,"Bq")<<std::endl;
    return draw_on_pad(name,std::string(legend).replace(legend.find("cpd"),3,"Bq"),x,newy,ex,newey);
  }
  TCanvas *cc = new TCanvas((name+"_cc").c_str(),legend.c_str(),800,600);
  gStyle->SetOptTitle(0);
  static bool first = true;
  if(first) { gROOT->ProcessLine(".x ~/styles/TStyle_GSSI_dingxf_light.C"); gStyle->SetOptFit(0); gStyle->SetPadGridX(false); gStyle->SetPadGridY(false); first = false; }
  if(make_tex) latexf<<"\\includegraphics[width=0.2\\textwidth]{"<<m_out<<"_"<<name<<"_cc}"<<std::endl;
  static int j = 0;
  if(make_tex&&!(((j++)+1)%5)) latexf<<"\\\\"<<std::endl;
  TGraphErrors *gr = new TGraphErrors(x.size(),&x[0],&y[0],&ex[0],&ey[0]);
  gr->Draw("AP");
  gr->SetMarkerStyle(20);
  if(name=="Kr85_rate") gr->GetYaxis()->SetRangeUser(-5,20);
//  if(name=="nu_pp_rate") gr->GetYaxis()->SetRangeUser(200,300);
  gr->GetXaxis()->SetTitle(m_var.c_str());
  gr->GetXaxis()->SetNdivisions(505);
  gr->GetXaxis()->SetNoExponent();
  gr->GetYaxis()->SetTitle(legend.c_str());
  //  pf.at(i)->GetYaxis()->SetRangeUser(y_min.at(i),y_max.at(i));
  TLatex *la = new TLatex;
  la->SetTextFont(132);
  la->SetNDC();
  la->SetTextSize(0.1);
  la->DrawLatex(0.2,0.838,legend.c_str());
  gPad->Update(); gPad->Modified();
  gr->Fit("pol0","Q0","0");
  TF1 *p0 = static_cast<TF1*>(gr->GetListOfFunctions()->FindObject("pol0"));
  assert(p0);
  double ymin = p0->GetParameter(0)-p0->GetParError(0);
  double ymax = p0->GetParameter(0)+p0->GetParError(0);
  TPave *pave = new TPave(xmin(),ymin,xmax(),ymax,0,"br");
  pave->SetFillColor(3);
  pave->SetFillStyle(3013);
  pave->SetLineWidth(2);
  pave->Draw();
  //  double N = inj_rate.at(i)*exposure;
  //  double inj_rate_low = (N-sqrt(N))/exposure;
  //  double inj_rate_up = (N+sqrt(N))/exposure;
  //  TPave *inj = new TPave(test_par[0],inj_rate_low,test_par[1],inj_rate_up,0,"br");
  //  inj->SetFillColor(4);
  //  inj->SetFillStyle(3013);
  //  inj->SetLineWidth(2);
  //  inj->Draw();
  cc->Print((m_out+"/"+m_out+"_"+name+"_cc.pdf").c_str());
}
