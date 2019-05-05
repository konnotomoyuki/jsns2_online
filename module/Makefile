PACKAGES=$(shell ls | grep -v __template__ | grep -v Makefile)

all: inc
	@ for dir in $(PACKAGES); \
	  do if [ -d $$dir ]; then make -f Makefile.src TARGET=$$dir ; fi; done

clean: 
	@ for dir in $(PACKAGES); \
	  do if [ -d $$dir ]; then make -f Makefile.src TARGET=$$dir clean ; fi; done

inc:
	@ for dir in $(PACKAGES); \
	  do if [ -d $$dir ]; then rm -f $(JSNS2_HOME)/include/$$dir; ln -sf $(JSNS2_HOME)/module/$$dir/include $(JSNS2_HOME)/include/$$dir; fi; done
