
# Compilor settings.
CC       := c++
FLAGS    := --std=c++14 -Wall -fPIC
COMP     := $(CC) $(FLAGS)

# library include/lib directories.
SACHOME  := /opt/sac-101.6
INCLUDE  := -I. -I/opt/local/include -I$(SACHOME)/include
LIBDIR   := -L. -L$(SACHOME)/lib

# library names.
LIBs     := -lASU_tools_cpp -lsac -lsacio -lmlpack -larmadillo -lgsl -lgslcblas -lfftw3 -lm

# *fun.cpp files under ./
SRCFILES := $(wildcard *.cpp)
OBJFILES := $(patsubst %.cpp, %.o, $(SRCFILES))
DEPFILES := $(patsubst %.cpp, %.d, $(SRCFILES))

# *cpp example programs under EGDIR.
EGDIR    := ./Examples
EGFILES  := $(wildcard $(EGDIR)/*.cpp)
EGEXECS  := $(patsubst %.cpp, %.out, $(EGFILES))
EGDEPS   := $(patsubst %.cpp, %.d, $(EGFILES))


all: libASU_tools_cpp.a $(EGEXECS)
	rm -f *d *.o $(EGDIR)/*.d
	@echo > /dev/null

# resolve dependencies automatically.
-include $(DEPFILES) $(EGDEPS)

libASU_tools_cpp.a: $(OBJFILES)
	ar cr libASU_tools_cpp.a $(OBJFILES)

%.o: %.cpp
	$(COMP) -MMD -MP -c $< -o $@ $(INCLUDE)

$(EGDIR)/%.out: $(EGDIR)/%.cpp
	$(COMP) -MMD -MP $< -o $@ $(INCLUDE) $(LIBDIR) $(LIBs)

clean:
	rm -f *.d *.o libASU_tools_cpp.a $(EGDIR)/*.d $(EGDIR)/*.out
