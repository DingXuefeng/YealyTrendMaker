#ifndef TrendData_H
#define TrendData_H
/********************************************************
 * class TrendData
 * author: Xuefeng Ding<xuefeng.ding.physics@gmail.com>
 * description:
 *   This class is the holder of the 210Bi trends
 * ChangeLog:
 *  2017.03.07 Convert macro to c++ class
 *  2017.05.24 Split data and trend to allow to overlap
 *  different data sets
 *******************************************************/
#include <string>
class TGraphErrors;
#include <vector>
#include <algorithm>
#include <map>
class TrendData {
  public:
    virtual ~TrendData() {}
    virtual void addFiles(double label_var,const std::string &file_path) = 0;
    //virtual void addPeriodAll(const std::string &file_path) = 0;
    virtual TGraphErrors *next_graph(std::string &name,std::string &legend) = 0;
    virtual std::vector<double> &get_correlations() = 0;
    virtual double xmin() const = 0;
    virtual double xmax() const = 0;
    virtual std::vector<bool> &is_corrvar_fixed() = 0 ;
};
class TrendDataImpl : public TrendData {
  // the conrete implementation class
  public:
    TrendDataImpl(const std::string &var);
    static void regRate(const std::string &var,const std::string &name,double min_ = 0,double max_ = 0) ;
    static void reg(const std::string &var,const std::string &name,double min_ = 0,double max_ = 0) ;
    static void regArray(const std::string &var,const std::string &name1,const std::string &name2,const std::string &name3="",const std::string &name4="",const std::string &name5="",const std::string &name6="") ;
    static void regArray_min(const std::string & var,double min1=0,double min2=0,double min3=0,double min4=0,double min5=0,double min6=0) ;
    static void regArray_max(const std::string & var,double max1=0,double max2=0,double max3=0,double max4=0,double max5=0,double max6=0) ;
    static void regCorrelation(const std::string & var1,const std::string &var2,double exp_cor);
    void addFiles(double label_var,const std::string &file_path) ;
    TGraphErrors *next_graph(std::string &name,std::string &legend);
    std::vector<double> &get_correlations() { return correlation; }
    std::vector<bool> &is_corrvar_fixed() { return corrvar_fixed; }
    static std::multimap<std::string,std::string> &get_correlation_items() { return correlation_terms; }
    static std::vector<double> &get_correlation_exp() { return correlation_exp; }
    double xmin() const { return *std::min_element(label_vars.begin(),label_vars.end()); }
    double xmax() const { return *std::max_element(label_vars.begin(),label_vars.end()); }
    static std::map<std::string,double> &get_config_ymin() { return config_ymin; }
    static std::map<std::string,double> &get_config_ymax() { return config_ymax; }
    static const std::string &pretty_name(const std::string &name) { return names.at(name); }
  private:
    void make_graphs();
    bool next(double &label_var,std::string &file_path) const;
    const std::string m_var;
    std::vector<double> label_vars;
    std::vector<bool> corrvar_fixed;
    std::vector<std::string> file_paths;
    mutable std::vector<double>::const_iterator varIt;
    mutable std::vector<std::string>::const_iterator fIt;
    static std::vector<std::string> vars;
    static std::map<std::string,std::string> names;
    static std::map<std::string,int> arrays;
    static std::vector<double> correlation_exp;
    static std::multimap<std::string,std::string> correlation_terms;
    std::vector<double> correlation;
    static std::map<std::string,std::vector<std::string> > array_names;
    static std::vector<std::string> graph_names;
    static std::vector<std::string> graph_legend;
    static std::map<std::string,double> config_ymin;
    static std::map<std::string,double> config_ymax;
    std::vector<TGraphErrors*> graphs;
};
#endif
