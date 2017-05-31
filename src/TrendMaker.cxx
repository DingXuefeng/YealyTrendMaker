#include "TrendMaker.h"
#include <iostream>
//#include "Factory.h"
#include "TLegend.h"
#include "TChain.h"
#include "TROOT.h"
#include "TGraphErrors.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TLatex.h"
#include "TPave.h"
#include "TF1.h"
#include "TList.h"
#include "TH1D.h"
#include <cassert>
#include "TrendData.h"
static int latex_j = 0;
TrendMakerImpl::TrendMakerImpl(const std::string &var) :
  m_var(var),m_project(),m_out(),label_vars(),file_paths(),make_tex(false),
  vars(),names(),arrays(),array_names(),latexf(),Ncolums(5)
{}

void TrendMakerImpl::make_plots(bool skip_lowp_) {
  skip_lowp = skip_lowp_;
  if(m_out=="") m_out=".";
  if(!graphs.size()) gather_graphs();
  if(make_tex) latexf.open(m_out+"/trend_"+m_project+"_"+m_var+".tex");
  if(make_tex) latexf << "\\begin{frame}[plain]{"<<m_project<<"}"<<std::endl;
  if(graphs.size()>12) Ncolums = 5; else Ncolums = 4;
  get_p_values();
  for( auto grs : graphs ) {
    make_plot(grs.first /* label */,grs.second /* vector of TGraphErrors */);
  }

  for( auto cor : correlation ) {
    make_correlation( cor );
  }
  if(make_tex) latexf << "\\end{frame}"<<std::endl;
  if(make_tex) latexf << "\\begin{frame}[plain]"<<std::endl;
  make_legend();
  if(make_tex) latexf << "\\end{frame}"<<std::endl;
  if(make_tex) latexf.close();
}
void TrendMakerImpl::get_p_values() {
  for ( auto ggr : graphs ) { /* std::map<Label,std::vector<TGraphErrors*> > graphs; */
    if(ggr.first.name=="likelihood_p_values") {
      for( auto gr : ggr.second ) 
        p_values.push_back(gr->GetY());
      break;
    }
  }
}

void TrendMakerImpl::make_plot(const Label &label,std::vector<TGraphErrors *> grs) {
  color = 0;
  Ncolors = grs.size();
  std::string name(label.name),legend(label.legend);
  if(make_tex) latexf<<"\\includegraphics[width="<<0.999/Ncolums<<"\\textwidth]{"<<m_project<<"_"<<name<<"_cc}"<<std::endl;
  if(make_tex&&!(((latex_j++)+1)%Ncolums)) latexf<<"\\\\"<<std::endl;
  TCanvas *cc = new TCanvas((name+"_cc").c_str(),legend.c_str(),800,600);
  gStyle->SetOptTitle(0);
  static bool first = true;
  if(first) { 
    gROOT->ProcessLine(".x ~/styles/TStyle_GSSI_dingxf_light.C"); 
    gStyle->SetOptFit(0); 
    gStyle->SetPadGridX(false); 
    gStyle->SetPadGridY(true); 
    //gStyle->SetPalette(kDeepSea);
    gStyle->SetPalette(55);
    //gStyle->SetPalette(51);
    first = false; 
  }
  dataset_i = 0;
  for( auto gr : grs) {
    draw_on_pad(name,legend,gr);
    ++dataset_i;
  }
  TGraphErrors *weighted_gr = draw_on_pad(name,legend,weighted(grs));
  weighted_gr->SetMarkerColor(kRed);
  weighted_gr->SetLineColor(kRed);
  weighted_gr->SetLineWidth(4);
  weighted_gr->SetMarkerStyle(20);
  weighted_gr->SetDrawOption("P");
  TLegend *tlegend_tmp = gPad->BuildLegend();
  tlegend = (TLegend*)(tlegend_tmp->Clone());
  gPad->GetListOfPrimitives()->Remove((TObject*)tlegend_tmp);
  cc->Print((m_out+"/"+m_project+"_"+name+"_cc.pdf").c_str());
}
TGraphErrors *TrendMakerImpl::weighted(std::vector<TGraphErrors*> &grs) {
  std::vector<double> y,ey;
  TGraphErrors *first_gr = grs.front();
  for( int j = 0;j<first_gr->GetN();++j) {
    double sum = 0;
    double e_sum = 0;
    double p_sum = 0;
    int i = 0;
    for( auto gr: grs ) {
      sum += gr->GetY()[j]; // j: year
      e_sum += gr->GetEY()[j]; // j: year
      p_sum += p_values.at(i)[j]; // i: datasets
      ++i;
    }
    y.push_back(sum/p_sum);
    ey.push_back(e_sum/p_sum);
  }
  return new TGraphErrors(first_gr->GetN(),first_gr->GetX(),&y[0],first_gr->GetEX(),&ey[0]);
}

void TrendMakerImpl::gather_graphs() {
  for(auto data : datas) {
    TGraphErrors *gr;
    std::string name,legend;
    while((gr = data->next_graph(name,legend))) {
      Label label { name, legend };
      if(graphs.find(label)==graphs.end()) {
        graphs.insert(std::make_pair(label,std::vector<TGraphErrors *>()));
      }
      graphs.at(label).push_back(gr);
    }
  }
  fill_correlations();
}

void TrendMakerImpl::fill_correlations() {
  int i = 0;
  for(auto cor : TrendDataImpl::get_correlation_items()) {
    const std::string &var1(cor.first);
    const std::string &var2(cor.second);
    TH1D *h_corr = new TH1D((var1+"_"+var2).c_str(),(var1+" "+var2).c_str(),20,-1,1);
    for( auto data : datas ) {
      if(data->is_corrvar_fixed().at(i)) 
        h_corr->Fill(data->get_correlations().at(i));
    }
    ++i;
    correlation.push_back(h_corr);
  }
}

Int_t NextPaletteColor(int fNextPaletteColor,int fNumPaletteColor) {
  static const Int_t ncolors = TColor::GetNumberOfColors();
  Int_t i = (fNextPaletteColor+0.0)/(fNumPaletteColor-1)*(ncolors-2)+1;
  //std::cout<<i<<" "<<ncolors<<" "<<fNextPaletteColor<<" "<<fNumPaletteColor<<std::endl;
  return TColor::GetColorPalette(i);
}

TGraphErrors *TrendMakerImpl::draw_on_pad(const std::string &name,const std::string &legend,TGraphErrors *gr) {
//  std::cout<<"Plotting ["<<gr<<"] on pad with X1 "<<gr->GetX()[0]<<" "<<" Y1 "<<gr->GetY()[0]<<std::endl;
  if(legend.find("cpd")!=std::string::npos&&gr->GetY()[0]>86400) {
    std::vector<double> y,ey;
    for(int i = 0;i<gr->GetN();++i) {
      y.push_back(gr->GetY()[i]/86400);
      ey.push_back(gr->GetEY()[i]/86400);
    }
    TGraphErrors *gr_new = new TGraphErrors(gr->GetN(),gr->GetX(),&y[0],gr->GetEX(),&ey[0]);
//    std::cout<<std::string(legend).replace(legend.find("cpd"),3,"Bq")<<std::endl;
    return draw_on_pad(name,std::string(legend).replace(legend.find("cpd"),3,"Bq"),gr_new);
  }
  if(gPad->GetListOfPrimitives()->GetSize()) gr->Draw("L3"); else {
    gr->Draw("AL3");
    gr->GetXaxis()->SetTitle(m_var.c_str());
    gr->GetXaxis()->SetNdivisions(505);
    gr->GetXaxis()->SetNoExponent();
    gr->GetYaxis()->SetTitle(legend.c_str());
//    if(name=="Kr85_rate") gr->GetYaxis()->SetRangeUser(-5,20);
//    if(name=="Bi210_rate") { gr->GetYaxis()->SetRangeUser(0,35); }
//    if(name=="likelihood_p_values") { gr->GetYaxis()->SetRangeUser(0,1); }
    double ymin = TrendDataImpl::get_config_ymin().at(name);
    double ymax = TrendDataImpl::get_config_ymax().at(name);
    std::cout<<name<<" "<<ymin<<" "<<ymax<<std::endl;
    if(!((ymin==0)&&(ymax==0))) gr->GetYaxis()->SetRangeUser(ymin,ymax);
    if(name=="likelihood_p_values") gStyle->SetPadGridY(false); 
  }
  gr->SetMarkerStyle(20);
  TIter next(gPad->GetListOfPrimitives());
  TObject *obj;
  while(( obj = next() )) {
    if(obj->ClassName()==std::string("TLatex")) {
      gPad->GetListOfPrimitives()->Remove(obj);
      delete obj;
    }
  }
  TLatex *la = new TLatex;
  la->SetTextFont(132);
  la->SetNDC();
  la->SetTextSize(0.1);
  la->DrawLatex(0.2,0.838,legend.c_str());
  Int_t i = NextPaletteColor(color++,Ncolors);
  gr->SetLineColor(i);
  gr->SetMarkerColor(i);
  gr->SetFillColorAlpha(i,p_values.at(dataset_i)[gr->GetN()-1]);
  gr->SetFillStyle(1001);
  //gr->SetFillStyle(3144);
  gr->SetMarkerColor(i);
  //gr->Fit("pol0","Q0","0");
  //TF1 *p0 = static_cast<TF1*>(gr->GetListOfFunctions()->FindObject("pol0"));
  //assert(p0);
  //double ymin = p0->GetParameter(0)-p0->GetParError(0);
  //double ymax = p0->GetParameter(0)+p0->GetParError(0);
  //TPave *pave = new TPave(xmin(),ymin,xmax(),ymax,0,"br");
  //pave->SetFillColor(i);
  //pave->SetFillStyle(3013);
  //pave->SetLineWidth(2);
  //pave->Draw();
  return gr;
}
double TrendMakerImpl::xmin() const { return datas.front()->xmin(); }
double TrendMakerImpl::xmax() const { return datas.front()->xmax(); }
void TrendMakerImpl::make_correlation(TH1 *h_corr) {
  std::string name(h_corr->GetName()),legend(h_corr->GetTitle());
  if(make_tex) latexf<<"\\includegraphics[width="<<0.999/Ncolums<<"\\textwidth]{"<<m_project<<"_"<<name<<"_cc}"<<std::endl;
  if(make_tex&&!(((latex_j++)+1)%Ncolums)) latexf<<"\\\\"<<std::endl;
  TCanvas *cc = new TCanvas((name+"_cc").c_str(),legend.c_str(),800,600);
  h_corr->Draw();
  h_corr->GetXaxis()->SetTitle("correlation");
  h_corr->GetYaxis()->SetTitle("Entries");

  TIter next(gPad->GetListOfPrimitives());
  TObject *obj;
  while(( obj = next() )) {
    if(obj->ClassName()==std::string("TLatex")) {
      gPad->GetListOfPrimitives()->Remove(obj);
      delete obj;
    }
  }
  TLatex *la = new TLatex;
  la->SetTextFont(132);
  la->SetNDC();
  la->SetTextSize(0.1);
  la->DrawLatex(0.2,0.838,legend.c_str());

  cc->Print((m_out+"/"+m_project+"_"+name+"_cc.pdf").c_str());
}
void TrendMakerImpl::make_legend() {
  std::string name("legend"),legend("legend");
  if(make_tex) latexf<<"\\includegraphics[width=0.999\\textwidth]{"<<m_project<<"_"<<name<<"_cc}"<<std::endl;
  if(make_tex&&!(((latex_j++)+1)%Ncolums)) latexf<<"\\\\"<<std::endl;
  TCanvas *cc = new TCanvas((name+"_cc").c_str(),legend.c_str(),800,600);
  tlegend->Draw();
  tlegend->SetNColumns(2);
  tlegend->ConvertNDCtoPad();
  tlegend->SetX1(0.01);
  tlegend->SetY1(0.01);
  tlegend->SetX2(0.99);
  tlegend->SetY2(0.99);
  cc->Print((m_out+"/"+m_project+"_"+name+"_cc.pdf").c_str());
}
