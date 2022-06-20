ROOTCFLAGS    = $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOTLIBS      = $(shell $(ROOTSYS)/bin/root-config --libs)
ROOTGLIBS     = $(shell $(ROOTSYS)/bin/root-config --glibs)

CXX  = g++
CXX += -I./
CXX += -I./obj/
CXX += -I./src/

CXXFLAGS  = -g -Wall -fPIC -Wno-deprecated
CXXFLAGS += $(ROOTCFLAGS)
CXXFLAGS += $(ROOTLIBS)
CXXFLAGS += $(ROOTGLIBS)
CXXFLAGS += -std=c++14
CXXFLAGS += -fconcepts

OUTLIB = ./obj/

#----------------------------------------------------#


all: runterzina_wfSim

.PHONY: printmakehelp_and_reminder
printmakehelp_and_reminder: runterzina_wfSim.cpp Makefile
	$(info  /******************************************************************************/)
	$(info  * task --> printmakehelp_and_reminder: runterzina_wfSim.cpp Makefile          *)
	$(info  * $$@ ----> $@                                         *)
	$(info  * $$< --------------------------------> $<                   *)
	$(info  * $$^ --------------------------------> $^          *)
	$(info  /******************************************************************************/)

obj/wfSim.o: src/wfSim.cpp src/wfSim.hh src/wfSimConfStr.hh
	$(CXX) -c -o $@ $< $(CXXFLAGS) 

obj/sipmAvalancheTree.o: src/sipmAvalancheTree.cpp src/sipmAvalancheTree.hh 
	$(CXX) -c -o $@ $< $(CXXFLAGS) 

runterzina_wfSim: runterzina_wfSim.cpp src/wfSimConfStr.hh obj/wfSim.o obj/sipmAvalancheTree.o
	$(CXX) -o $@ $^ $(CXXFLAGS)

clean:
	rm -f runterzina_wfSim
	rm -f *~
	rm -f src/*~
	rm -f wfData/*~
	rm -f $(OUTLIB)*.o
