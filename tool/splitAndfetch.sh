#!/bin/bash
scp -r sissa:/home/xding/BX-Analysis/bx-GooStats/charge_fit/periodAll.root .
root -b -q split.C'("periodAll.root")'
cat >example_filelist<<EOF
2014 2014.root
2015 2015.root
2016 2016.root
2017 2017.root
EOF
