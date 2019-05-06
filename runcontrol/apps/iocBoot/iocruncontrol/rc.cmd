#!../../bin/linux-x86_64/runcontrol

## You may have to change rcbrigded to something else
## everywhere it appears in this file

< envPaths

cd ${TOP}

## Register all support components
dbLoadDatabase "dbd/runcontrol.dbd"
runcontrol_registerRecordDeviceDriver pdbbase

## Load record instances
dbLoadRecords("db/rc.db", "name=Rec")

rcConfig("/home/tkonno/jsns2_online/runcontrol/slc/lib/libjsns2_slc_noROOT.so", "EB", "Rec")

cd ${TOP}/iocBoot/${IOC}
iocInit

