TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = runcontrol
# runcontrol.dbd will be created and installed
DBD += runcontrol.dbd

# runcontrol.dbd will be made up from these files:
runcontrol_DBD += base.dbd

# Include dbd files from all support applications:
runcontrol_DBD += deviceRC.dbd
runcontrol_DBD += ioc_shell.dbd

# Add all the support libraries needed by this IOC
#runcontrol_LIBS += xxx

# runcontrol_registerRecordDeviceDriver.cpp derives from runcontrol.dbd
runcontrol_SRCS += runcontrol_registerRecordDeviceDriver.cpp
runcontrol_SRCS += devRC.cpp
runcontrol_SRCS += deviceRC.c
runcontrol_SRCS += ioc_shell.c

# Build the main IOC entry point on workstation OSs.
runcontrol_SRCS_DEFAULT += runcontrolMain.cpp
runcontrol_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#runcontrol_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
runcontrol_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

jsns2_base_noROOT_DIR = $(JSNS2_HOME)/runcontrol/base/lib
jsns2_slc_noROOT_DIR = $(JSNS2_HOME)/runcontrol/slc/lib

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

USR_INCLUDES += -I$(JSNS2_HOME)/runcontrol/base/include -I$(JSNS2_HOME)/runcontrol/slc/include
USR_LIBS += jsns2_base_noROOT jsns2_slc_noROOT
USR_LIBS += m pthread dl z rt 

#readline ncurses 

