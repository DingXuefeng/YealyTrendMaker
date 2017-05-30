#ifndef TrendMaker_H
#define TrendMaker_H
/********************************************************
 * class TrendMaker
 * author: Xuefeng Ding<xuefeng.ding.physics@gmail.com>
 * description:
 *   This class is designed to load the fit result from 
 *  rootfiles and make trend plots. Example usage: the
 *  result of the yearly fitting, the scan of the 
 *  dependence on some variables for systematics studies
 * ChangeLog:
 *  2017.03.07 Convert macro to c++ class
 *  2017.05.24 Split data and trend to allow to overlap
 *  different data sets
 *******************************************************/
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <algorithm>
class TH1;
class TGraphErrors;
class TrendData;
class TLegend;
class TrendMaker {
  public:
    virtual void set_output_path(const std::string &project,const std::string &out,bool make_tex_ = false) = 0; // optional. if set, the output result will be saved in the output path
    virtual void add_data(TrendData *data) = 0;
    virtual void make_plots() = 0;
    virtual ~TrendMaker() {}
};
struct Label {
  std::string name;
  std::string legend;
  bool operator<(const Label &r) const { return name<r.name || (name==r.name && legend<r.legend); }
};
class TrendMakerImpl : public TrendMaker {
  // the conrete implementation class
  public:
    TrendMakerImpl(const std::string &var);
    void set_output_path(const std::string &project,const std::string &out,bool make_tex_ ) { m_project = project; m_out = out; make_tex = make_tex_; } 
    void add_data(TrendData *data) { datas.push_back(data); }
    void make_plots();
  private:
    void make_correlation(TH1 *h_corr);
    void gather_graphs();
    void make_plot(const Label &legend,std::vector<TGraphErrors *> grs);
    void make_legend();
    void draw_on_pad(const std::string &name,const std::string &legend,TGraphErrors *gr) ;
    double xmin() const;
    double xmax() const;
    void fill_correlations();

    std::vector<TrendData*> datas;
    std::map<Label,std::vector<TGraphErrors*> > graphs;

    TLegend *tlegend;

    const std::string m_var;
    void fill();
    std::string m_project;
    std::string m_out;
    std::vector<double> label_vars;
    std::vector<std::string> file_paths;
    bool make_tex;
    std::vector<std::string> vars;
    std::map<std::string,std::string> names;
    std::map<std::string,int> arrays;
    std::map<std::string,std::vector<std::string> > array_names;
    std::ofstream latexf;
    std::vector<TH1*> correlation;
    int Ncolums;
    int color;
    int Ncolors;
};
#endif
