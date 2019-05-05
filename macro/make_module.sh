packname=$1
modname=$2

dir=${JSNS2_HOME}/module/${packname}
mkdir -p ${dir}
mkdir -p ${dir}/src
mkdir -p ${dir}/include
mkdir -p ${dir}/tmp
mkdir -p ${dir}/lib
template=${JSNS2_HOME}/module/__template__

sed "s/__template__/${modname}/g" ${template}/include/LinkDef.h  > ${dir}/include/LinkDef.h
sed "s/__template__/${modname}/g" ${template}/include/__template__Module.hh  > ${dir}/include/${modname}Module.hh
sed "s/__template__/${modname}/g" ${template}/src/__template__Module.cc  > ${dir}/src/${modname}Module.cc
sed "s/__template__/${modname}/g" ${template}/Makefile | sed "s/__packname__/${packname}/g" > ${dir}/Makefile




