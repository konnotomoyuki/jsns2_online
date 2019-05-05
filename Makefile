ifndef JSNS2_HOME
  $(error JSNS2_HOME is undefined)
endif

.PHONY : all clean tools base framework database rawdata display module runcontrol slc

MAINLIBS  = base framework database rawdata display 
SUBPACKS  = module tools runcontrol
ALLPACKS  = $(MAINLIBS) $(SUBPACKS)

all: inc $(ALLPACKS) 

$(MAINLIBS): %:
	@ make -f Makefile.src TARGET=$@ 

$(SUBPACKS): %: 
	@ make -C $@/

clean: 
	@ for dir in $(ALLPACKS) slc; \
	  do make -f Makefile.src TARGET=$$dir clean; done
	@ rm -f lib include bin/*

inc:
	@ mkdir -p include lib
	@ for dir in $(MAINLIBS); \
	  do if [ -d ${JSNS2_HOME}/$$dir/include ]; then rm -f include/$$dir; ln -sf ${JSNS2_HOME}/$$dir/include include/$$dir; fi; done

