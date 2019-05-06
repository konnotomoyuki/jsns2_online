ifndef JSNS2_HOME
  $(error JSNS2_HOME is undefined)
endif

.PHONY : all clean tools base framework database rawdata display module runcontrol slc

MAINLIBS  = base framework database rawdata display 
SUBPACKS  = module tools 

# Uncomment this line to compile DAQ slow control software 
#SLCPACKS  = slc runcontrol

ALLPACKS  = $(MAINLIBS) $(SUBPACKS) $(SLCPACKS)

all: inc $(ALLPACKS) 

$(MAINLIBS): %:
	@ make -f Makefile.src TARGET=$@ 

$(SUBPACKS): %: 
	@ make -C $@/

$(SLCPAKCS): %:
	@ make -C $@/

clean: 
	@ for dir in $(MAINLIBS); \
	  do make -f Makefile.src TARGET=$$dir clean; done
	@ for dir in $(SUBPACKS) slc; \
	  do make -C $$dir/ clean; done
	@ rm -rf lib include bin/*

inc:
	@ mkdir -p include lib
	@ for dir in $(MAINLIBS); \
	  do if [ -d ${JSNS2_HOME}/$$dir/include ]; then rm -f include/$$dir; ln -sf ${JSNS2_HOME}/$$dir/include include/$$dir; fi; done

