CXX     = g++
RM      = rm -rf
CP      = cp -f

BINDIR  = $(JSNS2_HOME)/bin
SRCDIR  = ./src
INCDIR  = ./include
TMPDIR  = $(JSNS2_HOME)/tmp
LIBDIR  = $(JSNS2_HOME)/lib

LIBS   += -lm -lpthread -ldl -lrt
LIBS   += -L$(JSNS2_HOME)/lib 
LIBS   += -ljsns2_base
LIBS   += -ljsns2_framework
LIBS   += -ljsns2_rawdata
LIBS   += -ljsns2_database
LIBS   += -ljsns2_display
LIBS   += -ljsns2_rawIO
LIBS   += -ljsns2_rootIO
LIBS   += -ljsns2_recoPulseSimple
LIBS   += -ljsns2_viewPulse
LIBS   += -ljsns2_hitMap
LIBS   += -ljsns2_dqmHist
LIBS   += -ljsns2_dummyEvent
LIBS   += $(shell root-config --libs)
LDBS   += $(shell root-config --libs --glibs) -lGeom -lTreeViewer -lgsl -lgslcblas -lstdc++ -DLINUX
LIBS   += -lMLP -lMinuit -lTreePlayer -lTMVA -lTMVAGui -lXMLIO  -lMLP -lGeom -lEve

CFLAGS  = -g -Wall -fPIC -D_REENTRANT -DLINUX
CFLAGS += -I$(JSNS2_HOME)/include/base 
CFLAGS += -I$(JSNS2_HOME)/include/framework
CFLAGS += -I$(JSNS2_HOME)/include/rawdata 
CFLAGS += -I$(JSNS2_HOME)/include/rawIO
CFLAGS += -I$(JSNS2_HOME)/include/rootIO
CFLAGS += -I$(JSNS2_HOME)/include/recoPulseSimple
CFLAGS += -I$(JSNS2_HOME)/include/viewPulse
CFLAGS += -I$(JSNS2_HOME)/include/hitMap
CFLAGS += -I$(JSNS2_HOME)/include/dqmHist
CFLAGS += -I$(JSNS2_HOME)/include/dummyEvent
CFLAGS += -I$(shell root-config --cflags)

MAINS   = $(wildcard *.cc)
TARGETS = $(MAINS:%.cc=$(BINDIR)/%)

SRCS   = $(wildcard $(SRCDIR)/*.cc)
DEPS   = $(SRCS:$(SRCDIR)/%.cc=$(TMPDIR)/%.d)
OBJS   = $(SRCS:$(SRCDIR)/%.cc=$(TMPDIR)/%.o)

.PHONY : all clean 

all: $(TARGETS)

force:
	@ $(RM) $(TARGETS)
	@ make all

clean:
	@ echo '<< cleaning directory >>'
	@ $(RM) *~ */*~ \#*\#* */\#*\#* $(TARGETS)

$(TARGETS): $(BINDIR)/% : %.cc $(OBJS)
	@ echo '<< creating executable $@ >>'
	@ $(CXX) $(CFLAGS) $(LIBS)  $^ -o $@ $(GLIBS) $(LOADLIBES)
	@ $(RM) *~ */*~ \#*\#* */\#*\#*
	@ echo '<< compiling succeded!! >>'

$(OBJS): $(TMPDIR)/%.o : $(SRCDIR)/%.cc $(INCDIR)/%.h
	@ echo '<< compiling $@ >>'
	@ $(CXX) -c $(CFLAGS) $< -o  $@

$(DEPS) : $(TMPDIR)/%.d: $(SRCDIR)/%.cc $(INCDIR)/%.h
	@ if [ ! -d $(TMPDIR) ];then mkdir $(TMPDIR) ;fi
	@ echo '<< making dipendency $@ >>'
	@ $(CXX) $(CFLAGS) -MM -MT $(basename $@).o $< -o $@

ifeq ($(findstring clean,${MAKECMDGOALS}),)
  -include ${DEPS}
endif

