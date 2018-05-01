# Related directories.
SACHOME  := /opt/sac-101.6
SACHOME2 := /opt/sac

# Compile parameters & dirs.
COMP     := c++ -std=c++14 -Wall -fPIC
INCDIR   := -I. -I/opt/local/include -I$(SACHOME)/include -I$(SACHOME2)/include
LIBDIR   :=                          -L$(SACHOME)/lib     -L$(SACHOME2)/lib
# LIBS     := -lsac -lsacio -lgsl -lgslcblas -lfftw3 -lm
LIBS     := -lsac -lsacio -lmlpack -larmadillo -lgsl -lgslcblas -lfftw3 -lm

# function files
SRCFILES := $(wildcard *.cpp)
DEPFILES := $(patsubst %.cpp, %.d, $(SRCFILES))
OBJS     := $(patsubst %.cpp, %.o, $(SRCFILES))

# example program files under $(EGDIR)
EGDIR    := ./Examples
EGFILES  := $(wildcard $(EGDIR)/*.cpp)
EGEXECS  := $(patsubst %.cpp, %.out, $(EGFILES))
EGDEPS   := $(patsubst %.cpp, %.d, $(EGFILES))

all: $(EGEXECS)
	@echo > /dev/null

# resolve dependencies automatically.
-include $(DEPFILES) $(EGDEPS)

%.out: %.cpp
	@echo "Updating $@ ..."
	@$(COMP) -MD -MP $< -o $@ $(INCDIR) $(LIBDIR) $(LIBS)

clean:
	rm -f $(EGDIR)/*.d $(EGDIR)/*.out
