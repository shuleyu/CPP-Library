
CC = c++
FLAGS = --std=c++14 -Wall
COMP = $(CC) $(FLAGS)

SACHOME = /opt/sac
SACHOME1 = /usr/local/sac
CCODEDIR = /home/shule/Research/Fun.C.c002
INCLUDE = -I. -I$(SACHOME)/include -I$(SACHOME1)/include -I/opt/local/include -I${CCODEDIR}

all: libASU_tools_cpp.a
	@echo > /dev/null

libASU_tools_cpp.a: Kmeans.fun.o RenderSeed.fun.o Decimal2Binary.fun.o
	ar cr libASU_tools_cpp.a Kmeans.fun.o RenderSeed.fun.o Decimal2Binary.fun.o

Kmeans.fun.o: Kmeans.fun.cpp ASU_tools.hpp
	$(COMP) -c Kmeans.fun.cpp $(INCLUDE)
RenderSeed.fun.o: RenderSeed.fun.cpp ASU_tools.hpp
	$(COMP) -c RenderSeed.fun.cpp $(INCLUDE)
Decimal2Binary.fun.o: Decimal2Binary.fun.cpp ASU_tools.hpp
	$(COMP) -c Decimal2Binary.fun.cpp $(INCLUDE)

clean:
	rm -f *.fun.o libASU_tools_cpp.a
