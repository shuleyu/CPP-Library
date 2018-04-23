# Related directories.
SACHOME  := /opt/sac-101.6
SACHOME2 := /opt/sac

# Compile parameters & dirs.
COMP     := c++ --std=c++14 -Wall -fPIC
INCDIR   := -I. -I/opt/local/include -I$(SACHOME)/include -I$(SACHOME2)/include
LIBDIR   := -L. -L$(SACHOME)/lib -L$(SACHOME2)/lib
# LIBS     := -lASU_tools_cpp -lsac -lsacio -lgsl -lgslcblas -lfftw3 -lm
LIBS     := -lASU_tools_cpp -lsac -lsacio -lmlpack -larmadillo -lgsl -lgslcblas -lfftw3 -lm

# function files
SRCFILES := $(wildcard *.cpp)
DEPFILES := $(patsubst %.cpp, %.d, $(SRCFILES))
OBJS     := $(patsubst %.cpp, %.o, $(SRCFILES))

# example program files under $(EGDIR)
EGDIR    := ./Examples
EGFILES  := $(wildcard $(EGDIR)/*.cpp)
EGEXECS  := $(patsubst %.cpp, %.out, $(EGFILES))
EGDEPS   := $(patsubst %.cpp, %.d, $(EGFILES))

all: libASU_tools_cpp.a $(OBJS)
	@echo > /dev/null

examples: libASU_tools_cpp.a $(EGEXECS)
	@echo > /dev/null

# resolve dependencies automatically.
-include $(DEPFILES) $(EGDEPS)

libASU_tools_cpp.a: $(OBJS)
	@echo "Updating $@ ..."
	@ar cr libASU_tools_cpp.a $(OBJS)

%.o: %.cpp
	@echo "Updating $@ ..."
	@$(COMP) -MD -MP -c $< -o $@ $(INCDIR)

%.out: %.cpp
	@echo "Updating $@ ..."
	@$(COMP) -MD -MP $< -o $@ $(INCDIR) $(LIBDIR) $(LIBS)

# for code depends on ASU_tools.hpp
ASU_tools.hpp: libASU_tools_cpp.a
	@touch $@

clean:
	rm -f *.d *.o libASU_tools_cpp.a $(EGDIR)/*.d $(EGDIR)/*.out
