
CC = c++
FLAGS = --std=c++14 -Wall
COMP = $(CC) $(FLAGS)

SACHOME = /opt/sac
SACHOME1 = /usr/local/sac
CCODEDIR = /home/shule/Research/Fun.C.c002
INCLUDE = -I. -I$(SACHOME)/include -I$(SACHOME1)/include -I/opt/local/include -I${CCODEDIR}

all: libASU_tools_cpp.a
	@echo > /dev/null

libASU_tools_cpp.a: Kmeans.fun.o RenderSeed.fun.o Decimal2Binary.fun.o ShellExec.fun.o PointsInPolygon.fun.o CombinePolygons.fun.o GridStretch.fun.o StraightLineJunction.fun.o DotDist.fun.o
	ar cr libASU_tools_cpp.a Kmeans.fun.o RenderSeed.fun.o Decimal2Binary.fun.o ShellExec.fun.o PointsInPolygon.fun.o CombinePolygons.fun.o GridStretch.fun.o StraightLineJunction.fun.o DotDist.fun.o

Kmeans.fun.o: Kmeans.fun.cpp ASU_tools.hpp
	$(COMP) -c Kmeans.fun.cpp $(INCLUDE)
RenderSeed.fun.o: RenderSeed.fun.cpp ASU_tools.hpp
	$(COMP) -c RenderSeed.fun.cpp $(INCLUDE)
Decimal2Binary.fun.o: Decimal2Binary.fun.cpp ASU_tools.hpp
	$(COMP) -c Decimal2Binary.fun.cpp $(INCLUDE)
ShellExec.fun.o: ShellExec.fun.cpp ASU_tools.hpp
	$(COMP) -c ShellExec.fun.cpp $(INCLUDE)
PointsInPolygon.fun.o: PointsInPolygon.fun.cpp ASU_tools.hpp
	$(COMP) -c PointsInPolygon.fun.cpp $(INCLUDE)
CombinePolygons.fun.o: CombinePolygons.fun.cpp ASU_tools.hpp
	$(COMP) -c CombinePolygons.fun.cpp $(INCLUDE)
GridStretch.fun.o: GridStretch.fun.cpp ASU_tools.hpp
	$(COMP) -c GridStretch.fun.cpp $(INCLUDE)
StraightLineJunction.fun.o: StraightLineJunction.fun.cpp ASU_tools.hpp
	$(COMP) -c StraightLineJunction.fun.cpp $(INCLUDE)
DotDist.fun.o: DotDist.fun.cpp ASU_tools.hpp
	$(COMP) -c DotDist.fun.cpp $(INCLUDE)

clean:
	rm -f *.fun.o libASU_tools_cpp.a
