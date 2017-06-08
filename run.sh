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
project=krossi_hisconfig_official_input
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
    if [[ ! -f data/${y}/2012/2012_result.root ]]; then continue; fi
    if [[ $y =~ $keyword ]] || [[ $y =~ $keyword_extra ]]; then 
      add $y $list
    fi
  done
  run $name $list 
  ./main task_list
}
skip=0
title="pep FV, w/ \& w/o $^{85}$ penalty, w/ \& w/o fix LY/$\\nu\$(\$^{7}\$Be)"; 
comments="144 fits performed under different conditions: w/ or w/o $^{85}\$Kr penalty, w/ or w/o fix LY/$\\nu\$(\$^{7}\$Be), w/ (140-1500) or w/o externals (140-650), w/ or w/o complementary, using charge/charge-geo/charge-z variable. The correlation are filled in the histogram, and the expected correlation from MC-MC tests are drawn as green band. As we can see the correlation between \$^{210}\$Bi and \$^{85}\$Kr are stronger than expected. We will explain it in the next slides. Besides, the \$^{210}\$Bi and the light yield are expected to be strongly anti-correlated while actually we see positive-correlations, so the trend of \$^{210}\$Bi and light yield are not statistical fluctuations. The \$^{210}\$Bi is really decreasing. The red dot is the weighted sum with the inverser of the square of uncertainty" 
pick pep "pep-"
title="pep FV, w/o $^{85}$ penalty, free light yield"; 
comments="fits under different conditions overlapped. Now the correlation between \$^{210}\$Bi and \$^{85}\$Kr are as expected, indicating the increase of \$^{85}\$Kr is only statistical correlation." 
pick pepnoKrfreeLY "-free-.*noKr.*pep" "fixpep.*-Kr.*pep-"
title="pep FV, w/o $^{85}$ penalty, fixed light yield"; 
comments="fits under different conditions overlapped. Now the correlation between \$^{210}\$Bi and \$^{85}\$Kr are stronger, and the \$^{210}\$Bi decrease is stronger compared with the LY free case."
pick pepnoKrfixLY "fixLY.*noKr.*pep-"
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
