#for x in {100..216}; do
#for x in {37..42} {55..60} {73..78} {91..96} {145..150} {163..168} {181..186} {199..204}; do
for x in {1..6}; do
#  name=$(find . -name "$x-*")
  for year in {2012..2017}; do
    scp -r borexino:/storage/gpfs_data/borexino/users/dingbx/ChargeSysStudy/fitOnDataYearByYear/massive_scripts/${x}-* . 1>/dev/null 2>/dev/null
#    echo "scp -r borexino:/storage/gpfs_data/borexino/users/dingbx/ChargeSysStudy/fitOnDataYearByYear/massive_scripts/${x}-*/$year $name/ 1>/dev/null 2>/dev/null"
  done
  echo "Fetching [$x]"
done
for x in `ls`; do
  cp example_filelist $x/
done
