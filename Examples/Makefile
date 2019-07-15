# Usage:
# 1. Compile all example codes:
#    $ make SAC=${SACHOME}
# 2. Compile certain code:
#    $ make Code=Amplitdue.cpp SAC=${SACHOME}

# Directories.
SAC      := ${SACHOME}
Code     := UniQUEcoDeName

# Compile flags & library dirs.
COMP     := c++ -std=c++17 -Wall -fPIC
INCDIR   := -I/opt/local/include -I$(SAC)/include -I..
LIBDIR   := -L/opt/local/lib -L$(SAC)/lib
LIBS     := -lsac -lsacio -lgmt -lmariadb -larmadillo -lgsl -lgslcblas -lnetcdf -lfftw3 -lm # -lmlpack

# by default, compile all cpp files.
EGFILES  := $(wildcard *.cpp)
EGDEPS   := $(patsubst %.cpp, %.d, $(EGFILES))
EGEXECS  := $(patsubst %.cpp, %.out, $(EGFILES))

# if called with "Code=SomeCode.cpp", only compile this code:
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
