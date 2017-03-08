#include "TrendMaker.h"
#include "Factory.h"
#include "TString.h"
#include <cassert>
int main(int argc,char *argv[]) {
  assert(argc==4);
  TrendMaker *maker = Factory<TrendMaker>::get()->create(argv[1]);
  maker->set_output_path(argv[2],true);
  for(int year = 2012; year<=2016;++year)
    maker->addFiles(year,Form(argv[3],year));
  maker->reg("chi2_ndf","#chi^{2} / NDF");
  maker->reg("beta_ly","LY (p.e./MeV)");
  maker->reg("nu_Be7_rate","#nu ^{7}Be (cpd/100t)");
  maker->reg("nu_pp_rate","#nu pp (cpd/100t)");
  maker->reg("Bi210_rate","^{210}Bi (cpd/100t)");
//  maker->reg("Ext_Tl208_rate","Ext ^{208}Tl (cpd/100t)");
//  maker->reg("Ext_Bi214_rate","Ext ^{214}Bi (cpd/100t)");
//  maker->reg("Ext_K40_rate","Ext ^{40}K (cpd/100t)");
  maker->reg("Kr85_rate","^{85}Kr (cpd/100t)");
  maker->reg("Po210_rate","^{210}Po (cpd/100t)");
  maker->reg("Po210_quenching","^{210}Po qch (cpd/100t)");
  maker->reg("C14_rate","^{14}C (cpd/100t)");
  maker->reg("synthpup_rate","synthpup (cpd/100t)");
  maker->reg("C11_rate","^{11}C (cpd/100t)");
  maker->reg("C11_quenching","^{11}C qch (cpd/100t)");
  maker->regArray("beta_resolution","g1","g2","g3/500","g4");
  maker->make_plots();
  return 0;
}
