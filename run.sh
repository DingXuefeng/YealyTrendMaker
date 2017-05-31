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
  echo "./main ${var} ${project} ${output} $list $skip"
  ./main ${var} ${project} ${output} $list $skip
}
function pick {
  name=$1
  keyword=$2
  keyword_extra=$3
  list=list_$name

  rm $list
  for x in {1..216}; do
    y=`ls data | grep ^${x}-`
    if [[ ! -f data/${y}/2012/2012_result.root ]]; then continue; fi
    if [[ $y =~ $keyword ]] || [[ $y =~ $keyword_extra ]]; then 
      add $y $list
    fi
  done
  run $name $list
}
skip=0
pick pep pep
pick pepKr "-Kr.*pep"
pick pepnoKr noKr.*pep
pick pepKrfixLY "fixLY.*-Kr.*pep"
pick pepnoKrfixLY "fixLY.*noKr.*pep"
pick pepKrfreeLY "-free-.*-Kr.*pep" "fixBe7.*-Kr.*pep"
pick pepnoKrfreeLY "-free-.*noKr.*pep" "fixBe7.*-Kr.*pep"
pick Be7N Be7N
pick Be7NKr "-Kr.*Be7N"
pick Be7NnoKr noKr.*Be7N
pick Be7NKrfixLY "fixLY.*-Kr.*Be7N"
pick Be7NnoKrfixLY "fixLY.*noKr.*Be7N"
pick Be7NKrfreeLY "-free-.*-Kr.*Be7N" "fixBe7.*-Kr.*Be7N"
pick Be7NnoKrfreeLY "-free-.*noKr.*Be7N" "fixBe7.*-Kr.*Be7N"
pick Be7S Be7S
pick Be7SKr "-Kr.*Be7S"
pick Be7SnoKr noKr.*Be7S
pick Be7SKrfixLY "fixLY.*-Kr.*Be7S"
pick Be7SnoKrfixLY "fixLY.*noKr.*Be7S"
pick Be7SKrfreeLY "-free-.*-Kr.*Be7S" "fixBe7.*-Kr.*Be7S"
pick Be7SnoKrfreeLY "-free-.*noKr.*Be7S" "fixBe7.*-Kr.*Be7S"
#skip=1
#
#pick selectPvaluepep pep
#pick selectPvaluepepKr "-Kr.*pep"
#pick selectPvaluepepnoKr noKr.*pep
#pick selectPvaluepepKrfixLY "fixLY.*-Kr.*pep"
#pick selectPvaluepepnoKrfixLY "fixLY.*noKr.*pep"
#pick selectPvaluepepKrfreeLY "-free-.*-Kr.*pep" "fixBe7.*-Kr.*pep"
#pick selectPvaluepepnoKrfreeLY "-free-.*noKr.*pep" "fixBe7.*-Kr.*pep"
#pick selectPvalueBe7N Be7N
#pick selectPvalueBe7NKr "-Kr.*Be7N"
#pick selectPvalueBe7NnoKr noKr.*Be7N
#pick selectPvalueBe7NKrfixLY "fixLY.*-Kr.*Be7N"
#pick selectPvalueBe7NnoKrfixLY "fixLY.*noKr.*Be7N"
#pick selectPvalueBe7NKrfreeLY "-free-.*-Kr.*Be7N" "fixBe7.*-Kr.*Be7N"
#pick selectPvalueBe7NnoKrfreeLY "-free-.*noKr.*Be7N" "fixBe7.*-Kr.*Be7N"
#pick selectPvalueBe7S Be7S
#pick selectPvalueBe7SKr "-Kr.*Be7S"
#pick selectPvalueBe7SnoKr noKr.*Be7S
#pick selectPvalueBe7SKrfixLY "fixLY.*-Kr.*Be7S"
#pick selectPvalueBe7SnoKrfixLY "fixLY.*noKr.*Be7S"
#pick selectPvalueBe7SKrfreeLY "-free-.*-Kr.*Be7S" "fixBe7.*-Kr.*Be7S"
#pick selectPvalueBe7SnoKrfreeLY "-free-.*noKr.*Be7S" "fixBe7.*-Kr.*Be7S"
