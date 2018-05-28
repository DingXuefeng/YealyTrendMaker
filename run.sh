#!/bin/bash

# Report Maker
# Author: Xuefeng Ding<xuefeng.ding.physics@gmail.com>
# description:
#    A simple script to make a one-page report

#output folder
output=report
mkdir -p ${output} 2>/dev/null

# var: x title
var="year" 
# output folder
#project=without_Kr
#project=krossi_fixg1g2g3
#files="data/krossi_fixg1g2gg3/%d_result.root"
#project=krossi_fitter2
#project=krossi_fixg1g2g3_krossi_input
#project=krossi_hisconfig_hisinput
#project=krossi_hisconfig_official_input
project=SimulMLPFit
#files="data/${project}/filelist"
#files="../yearly/%d_result.root"
#files="data/with_vped/%d_result.root"
#files="data/without_vped/%d_result.root"
#files="data/without_vped/%d_result.root"

# resample
#var="job" 
#project=resample
#files="."
function add {
  project=$1
  list=$2
  echo "data/${project}/example_filelist" $project >> $list
}
skip=1
function run {
  project=$1
  list=$2
  echo "${var} ${project} ${output} $list $skip" >> task_list
  echo $title >> task_list
  echo $comments >> task_list
}
function pick {
  name=$1
  keyword=$2
  keyword_extra=$3
  list=list_$name

  rm $list
  rm task_list
  for x in {1..216}; do
    y=`ls data | grep ^${x}-`
    if [[ -z $y ]]; then continue; fi
    #if [[ ! -f data/${y}/2012/2012_result.root ]]; then continue; fi
    if [[ $y =~ $keyword ]] || [[ $y =~ $keyword_extra ]]; then 
      add $y $list
      echo "add <$y> <$list>"
    fi
  done
  run $name $list 
  ./main task_list
}
rm report/*.pdf
rm report/trend*.tex

#skip=0
#title="pep FV, w/ \& w/o $^{85}\$Kr penalty, w/ \& w/o fix LY/$\\nu\$(\$^{7}\$Be)"; 
#comments="R-charge;G-charge-geo;B-charge-zcorr;144 fits performed under different conditions: w/ or w/o $^{85}\$Kr penalty, w/ or w/o fix LY/$\\nu\$(\$^{7}\$Be), w/ (140-1500) or w/o externals (140-650), w/ or w/o complementary, using charge/charge-geo/charge-z variable. The correlation are filled in the histogram, and the expected correlation from MC-MC tests are drawn as green band. As we can see the correlation between \$^{210}\$Bi and \$^{85}\$Kr are stronger than expected. We will explain it in the next slides. Besides, the \$^{210}\$Bi and the light yield are expected to be strongly anti-correlated while actually we see positive-correlations, so the trend of \$^{210}\$Bi and light yield are not statistical fluctuations. The \$^{210}\$Bi is really decreasing. The red dot is the weighted sum with the inverser of the square of uncertainty" 
#pick pep "pep-"
#
#title="pep FV, w/ $^{85}\$Kr penalty, free light yield"; 
#comments="R-charge;G-charge-geo;B-charge-zcorr;Look at the case w/ $^{85}\$Kr penalty and free LY. Now the correlation between \$^{210}\$Bi and \$^{85}\$Kr are as expected from toyMC correlation tests. The \$^{85}\$Kr are constrained, and the residual correlation with \$^{210}\$Bi over years are fully statistical."
#pick pepKrfreeLY "-free-.*-Kr.*pep-"
#
#title="pep FV, w/ $^{85}\$Kr penalty, fix light yield"; 
#comments="R-charge;G-charge-geo;B-charge-zcorr;Look at the case w/ $^{85}\$Kr penalty and fix LY. Now the correlation between \$^{210}\$Bi and \$^{85}\$Kr are as expected from toyMC correlation tests. The \$^{85}\$Kr are constrained, and the residual correlation with \$^{210}\$Bi over years are fully statistical."
#pick pepKrfixLY "-fixLY-.*-Kr.*pep-"
#
#title="pep FV, w/o $^{85}$ penalty, free light yield"; 
#comments="R-charge;G-charge-geo;B-charge-zcorr;When we remove the \$^{85}\$Kr penalty, the \$^{85}\$Kr start to increase and the correlation between \$^{210}\$Bi and \$^{85}\$Kr are stronger than expected. Because we have no independent sensitivity on \$^{85}\$Kr, this is still statistical correlation. So we can draw the conclusion that the detector response is not modeled well and \$^{210}\$Bi is pushed away from the physical minimum, and thus the correlation becomes stronger.\\
#At the same time, let's look at the correlation between LY and \$^{210}\$Bi. Because we have sensitivity on both of them, such large positive correlation indicate there is real physics going on and the \$^{210}\$Bi decrease and LY decrease could be real."
#pick pepnoKrfreeLY "-free-.*noKr.*pep-"
#
#title="pep FV, w/o $^{85}$ penalty, fixed light yield"; 
#comments="R-charge;G-charge-geo;B-charge-zcorr;Now let's fix the light yield. We can see that the correlation between \$^{85}\$Kr and \$^{210}\$ becomes even stronger comapred with the LY free cases. We can draw the same conclusion that the modeling is even worse. Actually according to the correlation plot, the more the LY decreases, the less \$^{210}\$Bi decrease there is. 2\% decrease in LY will stop \$^{210}\$Bi from decreasing. When LY free, it decreases by 1\%, while in MC the LY increases by 0.5\%. So this suggests the \$^{210}\$Bi decrease seen by MC fit might be too strong due to bad modeling of the temporal dependence of energy scale at the \$^{210}\$Bi valley."
#pick pepnoKrfixLY "fixLY.*noKr.*pep-"

#skip=1
#title="pep-north or pep-south, w/ \& w/o $^{85}\$Kr penalty, w/ \& w/o fix LY/$\\nu\$(\$^{7}\$Be)"; 
#comments="pep-N vs pep-S, red vs cyan"
#pick pepNS "free-.*-Krpen-pepN-" "free-.*-Krpen-pepS-"

#skip=0
#title="Simultaneous fitting pepFV MLP fit Bi210 constrained";
#comments="160-1500, Bi210 constrained to 17.43\$\\pm\$0.97 cpd/100t, Kr85 constrained to Sandra's result (LL penalty)"
#pick MLP ".*"

skip=0
title="Comparison of LY"
comments="R: norm charge G: geo B: zcorr. 160-1500, Bi210 constrained to 17.43\$\\pm\$0.97 cpd/100t, Kr85 constrained to Sandra's result (LL penalty)"
pick MLP ".*"

#skip=0
#
#title="pep north, w/ $^{85}\$Kr penalty, free light yield"; 
#comments="R-charge;G-charge-geo;B-charge-zcorr;"
#pick pepNKrfreeLY "-free-.*-Kr.*pepN-"
#
#title="pep north, w/ $^{85}\$Kr penalty, fix light yield"; 
#comments="R-charge;G-charge-geo;B-charge-zcorr;"
#pick pepNKrfixLY "-fixLY-.*-Kr.*pepN-"
#
#title="pep north, w/o $^{85}$ penalty, free light yield"; 
#comments="R-charge;G-charge-geo;B-charge-zcorr;"
#pick pepNnoKrfreeLY "-free-.*noKr.*pepN-"
#
#title="pep north, w/o $^{85}$ penalty, fixed light yield"; 
#comments="R-charge;G-charge-geo;B-charge-zcorr;"
#pick pepNnoKrfixLY "fixLY.*noKr.*pepN-"
#
#title="pep south, w/ $^{85}\$Kr penalty, free light yield"; 
#comments="R-charge;G-charge-geo;B-charge-zcorr;"
#pick pepSKrfreeLY "-free-.*-Kr.*pepS-"
#
#title="pep south, w/ $^{85}\$Kr penalty, fix light yield"; 
#comments="R-charge;G-charge-geo;B-charge-zcorr;"
#pick pepSKrfixLY "-fixLY-.*-Kr.*pepS-"
#
#title="pep south, w/o $^{85}$ penalty, free light yield"; 
#comments="R-charge;G-charge-geo;B-charge-zcorr;"
#pick pepSnoKrfreeLY "-free-.*noKr.*pepS-"
#
#title="pep south, w/o $^{85}$ penalty, fixed light yield"; 
#comments="R-charge;G-charge-geo;B-charge-zcorr;"
#pick pepSnoKrfixLY "fixLY.*noKr.*pepS-"

#title="pep FV, w/ $^{85}$ penalty, free light yield 1500"; 
#comments="R-charge;G-charge-geo;B-charge-zcorr;"
#pick pepKrfreeLY1500 "-free-.*1500.*-Kr.*pep-nocomp"
#title="pep FV, w/ $^{85}$ penalty, free light yield 650"; 
#comments="R-charge;G-charge-geo;B-charge-zcorr;"
#pick pepKrfreeLY650 "-free-.*650.*-Kr.*pep-comp"
#pick pepKr "-Kr.*pep-"
#pick pepKrfreeLY "-free-.*-Kr.*pep" "fixpep.*-Kr.*pep-"
#pick pepnoKr "noKr.*pep-"
#pick pepKrfixLY "fixLY.*-Kr.*pep-"
#pick pepnoKrfixLY "fixLY.*noKr.*pep-"
#pick pepnoKrfreeLY "-free-.*noKr.*pep" "fixpep.*-Kr.*pep-"
#pick pepN pepN
#pick pepNKr "-Kr.*pepN"
#pick pepNnoKr noKr.*pepN
#pick pepNKrfixLY "fixLY.*-Kr.*pepN"
#pick pepNnoKrfixLY "fixLY.*noKr.*pepN"
#pick pepNKrfreeLY "-free-.*-Kr.*pepN" "fixpep.*-Kr.*pepN"
#pick pepNnoKrfreeLY "-free-.*noKr.*pepN" "fixpep.*-Kr.*pepN"
#pick pepS pepS
#pick pepSKr "-Kr.*pepS"
#pick pepSnoKr noKr.*pepS
#pick pepSKrfixLY "fixLY.*-Kr.*pepS"
#pick pepSnoKrfixLY "fixLY.*noKr.*pepS"
#pick pepSKrfreeLY "-free-.*-Kr.*pepS" "fixpep.*-Kr.*pepS"
#pick pepSnoKrfreeLY "-free-.*noKr.*pepS" "fixpep.*-Kr.*pepS"
#skip=1
#
#pick selectPvaluepep pep
#pick selectPvaluepepKr "-Kr.*pep"
#pick selectPvaluepepnoKr noKr.*pep
#pick selectPvaluepepKrfixLY "fixLY.*-Kr.*pep"
#pick selectPvaluepepnoKrfixLY "fixLY.*noKr.*pep"
#pick selectPvaluepepKrfreeLY "-free-.*-Kr.*pep" "fixpep.*-Kr.*pep"
#pick selectPvaluepepnoKrfreeLY "-free-.*noKr.*pep" "fixpep.*-Kr.*pep"
#pick selectPvaluepepN pepN
#pick selectPvaluepepNKr "-Kr.*pepN"
#pick selectPvaluepepNnoKr noKr.*pepN
#pick selectPvaluepepNKrfixLY "fixLY.*-Kr.*pepN"
#pick selectPvaluepepNnoKrfixLY "fixLY.*noKr.*pepN"
#pick selectPvaluepepNKrfreeLY "-free-.*-Kr.*pepN" "fixpep.*-Kr.*pepN"
#pick selectPvaluepepNnoKrfreeLY "-free-.*noKr.*pepN" "fixpep.*-Kr.*pepN"
#pick selectPvaluepepS pepS
#pick selectPvaluepepSKr "-Kr.*pepS"
#pick selectPvaluepepSnoKr noKr.*pepS
#pick selectPvaluepepSKrfixLY "fixLY.*-Kr.*pepS"
#pick selectPvaluepepSnoKrfixLY "fixLY.*noKr.*pepS"
#pick selectPvaluepepSKrfreeLY "-free-.*-Kr.*pepS" "fixpep.*-Kr.*pepS"
#pick selectPvaluepepSnoKrfreeLY "-free-.*noKr.*pepS" "fixpep.*-Kr.*pepS"
