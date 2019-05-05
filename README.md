# JSNS2 Online Analysis Software Toolkit
JSNS2 Online Analysis Software Tool is a general software framework for
online data analysis including event builidng, pulse reconstruction and event display.

## Requirement :
- Linux OS (Tested with scientific linux 7)
- Root 6 (6.14.06 or later)

# Initial setup
- Install ROOT with htto enabled. see https://root.cern.ch/building-root
- Edit path to root in senenv script
~~~
source ${HOME}/root/bin/thisroot.sh
~~~
- Copy rootrc to your home by following command:
~~~bash
$ cp __rootrc__ ${HOME}/.rootrc
~~~
- In addtion you can also compile slow control software if you have EPICS-base.
Uncomment a line for SLCPACKS in Makefile:
~~~
# Uncomment this line to compile DAQ slow control software                                             
#SLCPACKS  = slc runcontrol
~~~


