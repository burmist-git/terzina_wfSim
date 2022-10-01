RELVERSION  = $(shell cat .release)

ROOTCFLAGS    = $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOTLIBS      = $(shell $(ROOTSYS)/bin/root-config --libs)
ROOTGLIBS     = $(shell $(ROOTSYS)/bin/root-config --glibs)

MakefileFullPath = $(abspath $(lastword $(MAKEFILE_LIST)))
MakefileDirFullPath = $(shell dirname $(MakefileFullPath))
INSTALLDIR = $(MakefileDirFullPath)/install.$(RELVERSION)/


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

all: makedir runterzina_wfSim

makedir:
	mkdir -p $(OUTLIB);

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

obj/genSiPMwf.o: src/genSiPMwf.cpp src/genSiPMwf.hh src/wfParametrisationStr.hh
	$(CXX) -c -o $@ $< $(CXXFLAGS) 

obj/sipmAvalancheTree.o: src/sipmAvalancheTree.cpp src/sipmAvalancheTree.hh 
	$(CXX) -c -o $@ $< $(CXXFLAGS) 

obj/libwfsim.so: obj/wfSim.o obj/genSiPMwf.o obj/sipmAvalancheTree.o
	$(CXX) -shared -o $@ $^

runterzina_wfSim: runterzina_wfSim.cpp src/wfSimConfStr.hh obj/wfSim.o obj/sipmAvalancheTree.o obj/genSiPMwf.o
	$(CXX) -o $@ $^ $(CXXFLAGS)

generate_waveform_template: generate_waveform_template.C
	$(CXX) -o $@ $^ $(CXXFLAGS)

fit_LEE: fit_LEE.C
	$(CXX) -o $@ $^ $(CXXFLAGS)

install: makedir obj/libwfsim.so
	mkdir -p $(INSTALLDIR);
	cp $(OUTLIB)libwfsim.so $(INSTALLDIR)libwfsim.so
	cp src/*.hh $(INSTALLDIR).

cleaninstall:
	rm -rf $(INSTALLDIR)

clean:
	rm -f runterzina_wfSim
	rm -f generate_waveform_template
	rm -f *~
	rm -f src/*~
	rm -f wfData/*~
	rm -f $(OUTLIB)*.o
	rm -f $(OUTLIB)*.so
