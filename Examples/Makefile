# Related directories.
SACHOME  := /opt/sac-101.6
SACHOME2 := /opt/sac
GMTDIR   := RePlacedGMTdir
Code     := UniQUEcoDeName

# Compile parameters & dirs.
COMP     := c++ -std=c++14 -Wall -fPIC
INCDIR   := -I.. -I/opt/local/include -I$(SACHOME)/include -I$(SACHOME2)/include -I$(GMTDIR)/include
LIBDIR   :=                           -L$(SACHOME)/lib     -L$(SACHOME2)/lib     -L$(GMTDIR)/lib64

# LIBS     := -lsac -lsacio -lgsl -lgslcblas -lfftw3 -lm
LIBS     := -lsac -lsacio -lmlpack -lgmt -larmadillo -lgsl -lgslcblas -lfftw3 -lm

# example program files & its dependency files.
EGFILES  := $(wildcard *.cpp)
EGDEPS   := $(patsubst %.cpp, %.d, $(EGFILES))
EGEXECS  := $(patsubst %.cpp, %.out, $(EGFILES))
ifneq ($(Code),UniQUEcoDeName)
EGEXECS  := $(Code).out
endif

all: $(EGEXECS)
	@echo > /dev/null

# resolve dependencies automatically.
-include $(EGDEPS)

%.out: %.cpp
	@echo "Updating $@ ..."
	@$(COMP) -MD -MP $< -o $@ $(INCDIR) $(LIBDIR) $(LIBS)

clean:
	rm -f *.d *.out
