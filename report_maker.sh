#!/bin/bash

# Report Maker
# Author: Xuefeng Ding<xuefeng.ding.physics@gmail.com>
# description:
#    A simple script to make a one-page report

# var: x title
var="year" 
# output folder
output=report
#files="../yearly/%d_result.root"
files="../gpu/trend/%d_result.root"
mkdir output 2>/dev/null
./main ${var} ${output} ${files}
