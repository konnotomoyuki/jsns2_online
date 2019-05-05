# JSNS2 Online Analysis Software Toolkit
JSNS2 Online Analysis Software Tool is a general software framework for
online data analysis including event builidng, pulse reconstruction and event display.

## Requirement :
- Linux OS (Tested with scientific linux 7)
- Root 6 (6.14.06 or later)
- bash
- (Optional) epics-base R3.14
 
# Initial setup
1. Install and setup ROOT
  - If you want to use web DQM, rebuild ROOT with http enabled. See more https://root.cern.ch/building-root
  - Edit path to root in senenv script in a line of `source ${HOME}/root/bin/thisroot.sh`
  - Copy rootrc to your home by following command: `cp ./__rootrc__ ${HOME}/.rootrc`
1. (Optional) In addtion you can also compile slow control software if you have EPICS-base.
Edit `EPICS_BASE` in setenv script to fit your EPIC-base. 
And then, uncomment a line of `SLCPACKS` in Makefile:

Now, it is ready to compile the package!

# Compiling the package

