ifndef JSNS2_HOME
  $(error JSNS2_HOME is undefined)
endif

.PHONY : all clean base slc apps

SUBPACKAGES = base slc apps

all: inc $(SUBPACKAGES)

$(SUBPACKAGES): %:
	@ cd $@; make ; cd -

clean: 
	@ for dir in $(SUBPACKAGES); \
	  do cd $$dir; make clean; cd -; done
	@ rm -f lib/*
	@ rm -f include/*

inc:
	@ ln -sf $(JSNS2_HOME)/base/include base/
	@ ln -sf $(JSNS2_HOME)/base/src base/
	@ ln -sf $(JSNS2_HOME)/slc/include slc/
	@ ln -sf $(JSNS2_HOME)/slc/src slc/
