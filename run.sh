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
function run {
  project=$1
  list=$2
  echo "./main ${var} ${project} ${output} $list"
  ./main ${var} ${project} ${output} $list
}
function pick {
  name=$1
  keyword=$2
  list=list_$name

  rm $list
  for x in {1..216}; do
    y=`ls data | grep ^${x}-`
    if [[ ! -f data/${y}/2012/2012_result.root ]]; then continue; fi
    if [[ $y =~ $keyword ]]; then 
      add $y $list
    fi
  done
  run $name $list
}

pick pep pep
pick Be7N Be7N
pick Be7S Be7S
