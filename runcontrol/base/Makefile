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

DIRNAME    = base
LIBNAME    = jsns2_$(DIRNAME)
LIBTARGET  = $(LIBDIR)/lib$(LIBNAME)_noROOT.so

SRCS     = $(wildcard $(SRCDIR)/*.cc)
INCS     = $(wildcard $(INCDIR)/*.hh)
OBJS     = $(SRCS:$(SRCDIR)/%.cc=$(TMPDIR)/%.o)
DEPS     = $(OBJS:%.o=%.d)
DICT     = lib$(LIBNAME)
DICTC    = tmp/$(DICT).cxx
HEADS    = $(shell ls $(INCDIR)/*.hh | xargs -i basename {})

CFLAGS   = -g -fPIC -O2 -Wall
CFLAGS  += -I$(SLCDIR)/include
#CFLAGS  += -I$(shell root-config --cflags)

.PHONY : all clean

all: $(DEPS) $(LIBTARGET) install

install: 
#	@ ln -sf $(JSNS2_HOME)/$(DIRNAME)/lib/lib* $(JSNS2_HOME)/lib/
	@ echo "<< install $(DIRNAME) succeede!! >>"

clean:
	@ echo "<< cleaning all $(DIRNAME) >>"
	@ $(RM) *~ */*~ */*/*~ \#*\#* */\#*\#* tmp/*.{d,o} lib/*.so
	@ $(RM) tmp/*.{d,o,cxx,pcm} lib/*.{so,pcm,rootmap} src/*~ include/*~ tools/*~ 

$(LIBTARGET) : $(OBJS)
	@ mkdir -p lib
	@ echo "<< creating shared librariy $@ >>"
#	@ $(CXX) -shared  $(CFLAGS) $^ $(DICTC) -o $@ $(LIBS)
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
