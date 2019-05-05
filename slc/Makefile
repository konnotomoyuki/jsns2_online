CXX     = g++
CP      = cp
MAKE    = make
MKDIR   = mkdir -p
AR      = ar rcsv
RM      = rm -f

TARGET   = .
SLCDIR   = .
SRCDIR   = $(TARGET)/src
INCDIR   = $(TARGET)/include
TMPDIR   = $(SLCDIR)/tmp
LIBDIR   = ./lib

DIRNAME    = slc
LIBNAME    = jsns2_$(DIRNAME)
LIBTARGET  = $(LIBDIR)/lib$(LIBNAME).so

SRCS     = $(wildcard $(SRCDIR)/*.cc)
INCS     = $(wildcard $(INCDIR)/*.hh)
OBJS     = $(SRCS:$(SRCDIR)/%.cc=$(TMPDIR)/%.o)
DEPS     = $(OBJS:%.o=%.d)
DICT     = lib$(LIBNAME)
DICTC    = tmp/$(DICT).cxx
HEADS    = $(shell ls $(INCDIR)/*.hh | xargs -i basename {})

INCPATH  = -I$(INCDIR)
INCPATH  += -I$(JSNS2_HOME)/include/base

CFLAGS   = -g -fPIC -O2 -Wall
CFLAGS  += $(INCPATH)
#CFLAGS  += -I$(shell root-config --cflags)

.PHONY : all clean

all: $(DEPS) $(LIBTARGET) install

install: 
	@ rm -f $(JSNS2_HOME)/include/$(DIRNAME)
	@ ln -sf $(JSNS2_HOME)/$(DIRNAME)/include $(JSNS2_HOME)/include/$(DIRNAME)
	@ ln -sf $(JSNS2_HOME)/$(DIRNAME)/lib/lib* $(JSNS2_HOME)/lib
	@ echo "<< install $(DIRNAME) succeede!! >>"

clean:
	@ echo "<< cleaning all $(DIRNAME) >>"
	@ $(RM) *~ */*~ */*/*~ \#*\#* */\#*\#* tmp/*.{d,o} lib/*.so
	@ $(RM) tmp/*.{d,o,cxx,pcm} lib/*.{so,pcm,rootmap} src/*~ include/*~ tools/*~ 

dict : 
	@ echo "<< compiling $@ >>"
	@ cd $(INCDIR)/; rootcling -f ../$(DICTC) $(INCPATH) $(HEADS) LinkDef.h

$(LIBTARGET) : $(OBJS)
	@ mkdir -p lib
	@ echo "<< creating shared librariy $@ >>"
	@ $(CXX) -shared  $(CFLAGS) $^ -o $@ $(LIBS)
#	@ mv tmp/*.pcm lib/
	@ echo "<< compiling succeede!! >>"

$(OBJS) : $(TMPDIR)/%.o : $(SRCDIR)/%.cc $(INCDIR)/%.hh
	@ echo "<< compiling $@ >>"
	@ $(CXX) -c $(CFLAGS) $< -o $@ 

$(DEPS) : $(TMPDIR)/%.d: $(SRCDIR)/%.cc $(INCDIR)/%.hh
	@ $(MKDIR) $(TMPDIR)
	@ echo "<< making dependency $@ >>"
	@ $(CXX) $(CFLAGS) -MM -MT $(basename $@).o $< -o $@

ifeq ($(findstring clean,${MAKECMDGOALS}),)
  -include ${DEPS}
endif
