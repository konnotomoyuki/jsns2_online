echo "# make_module.sh"
if [ $# -ne 2 ]; then
    echo "# usage : ./make_module.sh <packname> <modname>"
    echo "# packname is directory name of your module under ./module "
    echo "# modname is module class name assigned as <modname>Module.hh etc"
    exit 1
fi

packname=$1
modname=$2

dir=${JSNS2_HOME}/module/${packname}
template=${JSNS2_HOME}/module/__template__
echo "<< making directory "${dir} ">>"
mkdir -p ${dir}
mkdir -p ${dir}/src
mkdir -p ${dir}/include
mkdir -p ${dir}/tmp
mkdir -p ${dir}/lib

echo "<< making  "${dir}/include/LinkDef.h" >>"
sed "s/__template__/${modname}/g" ${template}/include/LinkDef.h  > ${dir}/include/LinkDef.h
echo "<< making  "${dir}/include/${modname}Module.hh" >>"
sed "s/__template__/${modname}/g" ${template}/include/__template__Module.hh > ${dir}/include/${modname}Module.hh
echo "<< making  "${dir}/src/${modname}Module.cc" >>"
sed "s/__template__/${modname}/g" ${template}/src/__template__Module.cc > ${dir}/src/${modname}Module.cc
echo "<< making  "${dir}/Makefile" >>"
sed "s/__template__/${modname}/g" ${template}/Makefile | sed "s/__packname__/${packname}/g" > ${dir}/Makefile
echo "<< making  "${dir}/rootlib.txt" >>"
sed "s/__template__/${modname}/g" ${template}/rootlib.txt > ${dir}/rootlib.txt
echo "<< done! >>"




