echo "{" > rootlogon.C
echo "  gSystem->Load(\"libjsns2_base.so\");" >> rootlogon.C
echo "  gSystem->Load(\"libjsns2_framework.so\");" >> rootlogon.C
echo "  gSystem->Load(\"libjsns2_rawdata.so\");" >> rootlogon.C
echo "  gSystem->Load(\"libjsns2_database.so\");" >> rootlogon.C
echo "  gSystem->Load(\"libjsns2_display.so\");" >> rootlogon.C

ls lib | grep .so | grep -v "base\|framwork\|rawdata\|database\|display\|slc" |\
 awk '{print "  gSystem->Load(\""$1"\");"}' >> rootlogon.C
cat __rootlogon_C_ >> rootlogon.C
echo "}" >> rootlogon.C
