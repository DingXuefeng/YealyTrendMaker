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
 *******************************************************/
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <algorithm>
class TrendMaker {
  public:
    virtual void set_output_path(const std::string &out,bool make_tex_ = false) = 0; // optional. if set, the output result will be saved in the output path
    virtual void reg(const std::string &var,const std::string &name) = 0;
    virtual void regArray(const std::string &var,const std::string &name1,const std::string &name2,const std::string &name3="",const std::string &name4="",const std::string &name5="",const std::string &name6="") = 0;
    virtual void make_plots() = 0;
    virtual ~TrendMaker() {};
    virtual void addFiles(double label_var,const std::string &file_path) = 0;
    virtual bool next(double &label_var,std::string &file_path) const = 0;
};
class TrendMakerImpl : public TrendMaker {
  // the conrete implementation class
  public:
    TrendMakerImpl(const std::string &var);
    void set_output_path(const std::string &out,bool make_tex_ ) { m_out = out; make_tex = make_tex_; } // optional. if set, the output result will be saved in the output path
    void reg(const std::string &var,const std::string &name) ;
    void regArray(const std::string &var,const std::string &name1,const std::string &name2,const std::string &name3="",const std::string &name4="",const std::string &name5="",const std::string &name6="") ;
    void make_plots();
    void addFiles(double label_var,const std::string &file_path) { label_vars.push_back(label_var); file_paths.push_back(file_path); }
  private:
    void draw_on_pad(const std::string &name,const std::string &legend,const std::vector<double> &x,const std::vector<double> &y,const std::vector<double> &ex,const std::vector<double> &ey) ;
    bool next(double &label_var,std::string &file_path) const;
    double xmin() const { return *std::min_element(label_vars.begin(),label_vars.end()); }
    double xmax() const { return *std::max_element(label_vars.begin(),label_vars.end()); }
    const std::string m_var;
    std::string m_out;
    std::vector<double> label_vars;
    std::vector<std::string> file_paths;
    bool make_tex;
    std::vector<std::string> vars;
    std::map<std::string,std::string> names;
    std::map<std::string,int> arrays;
    std::map<std::string,std::vector<std::string> > array_names;
    std::ofstream latexf;
};
#endif
