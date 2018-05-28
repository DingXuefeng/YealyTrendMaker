#!/bin/bash
function compareA1100 {
scp -r sissa:/home/xding/BX-Analysis/bx-GooStats/charge_fit/withValley.root .
mv withValley.root periodAll.root
root -b -q split.C
cat >example_filelist<<EOF
2012 2012.root
2013 2013.root
2014 2014.root
2015 2015.root
2016 2016.root
2017 2017.root
EOF
mkdir 1-withValley
cd 1-withValley
cd ..
mkdir 2-withoutValley
cd 2-withoutValley
scp -r sissa:/home/xding/BX-Analysis/bx-GooStats/charge_fit/withoutValley.root .
mv withoutValley.root periodAll.root
root -b -q split.C
cat >example_filelist<<EOF
2012 2012.root
2013 2013.root
2014 2014.root
2015 2015.root
2016 2016.root
2017 2017.root
EOF
cd ..
mkdir 3-A1100withValley
cd 3-A1100withValley
scp -r sissa:/home/xding/BX-Analysis/bx-GooStats/charge_fit/A1100withValley.root .
mv A1100withValley.root periodAll.root
root -b -q split.C
cat >example_filelist<<EOF
2012 2012.root
2013 2013.root
2014 2014.root
2015 2015.root
2016 2016.root
2017 2017.root
EOF
cd ..
}
function compareGeoEtc {
mkdir 1-normal-colorR
cd 1-normal-colorR
scp -r sissa:/home/xding/BX-Analysis/bx-GooStats/charge_fit/Year_norm.root .
mv Year_norm.root periodAll.root
root -b -q ../split.C
cat >example_filelist<<EOF
2012 2012.root
2013 2013.root
2014 2014.root
2015 2015.root
2016 2016.root
2017 2017.root
EOF
cd ..
mkdir 2-geo-colorG
cd 2-geo-colorG
scp -r sissa:/home/xding/BX-Analysis/bx-GooStats/charge_fit/Year_geo.root .
mv Year_geo.root periodAll.root
root -b -q ../split.C
cat >example_filelist<<EOF
2012 2012.root
2013 2013.root
2014 2014.root
2015 2015.root
2016 2016.root
2017 2017.root
EOF
cp ../example_filelist .
cd ..
mkdir 3-zcorr-colorB
cd 3-zcorr-colorB
scp -r sissa:/home/xding/BX-Analysis/bx-GooStats/charge_fit/Year.root .
mv Year.root periodAll.root
root -b -q ../split.C
cat >example_filelist<<EOF
2012 2012.root
2013 2013.root
2014 2014.root
2015 2015.root
2016 2016.root
2017 2017.root
EOF
cp ../example_filelist .
cd ..
}
compareGeoEtc
