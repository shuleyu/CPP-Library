
CC = c++
FLAGS = --std=c++14 -Wall
COMP = $(CC) $(FLAGS)

SACHOME = /opt/sac
SACHOME1 = /usr/local/sac
CCODEDIR = /home/shule/Research/Fun.C.c002
CCODEDIR1 = /Users/shule/Research/Fun.C.c002
CCODEDIR2 = /Users/Shule/Research/Fun.C.c002
INCLUDE = -I. -I$(SACHOME)/include -I$(SACHOME1)/include -I/opt/local/include -I${CCODEDIR} -I${CCODEDIR1} -I${CCODEDIR2}

all: libASU_tools_cpp.a
	@echo > /dev/null

libASU_tools_cpp.a: Kmeans.fun.o Decimal2Binary.fun.o ShellExec.fun.o PointsInPolygon.fun.o CombinePolygons.fun.o GridStretch.fun.o StraightLineJunction.fun.o STDCC.fun.o SolvePoly.fun.o SH_Coeff.fun.o Combination.fun.o
	ar cr libASU_tools_cpp.a Kmeans.fun.o Decimal2Binary.fun.o ShellExec.fun.o PointsInPolygon.fun.o CombinePolygons.fun.o GridStretch.fun.o StraightLineJunction.fun.o STDCC.fun.o SolvePoly.fun.o SH_Coeff.fun.o Combination.fun.o

Kmeans.fun.o: Kmeans.fun.cpp ASU_tools.hpp
	$(COMP) -c Kmeans.fun.cpp $(INCLUDE)
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
STDCC.fun.o: STDCC.fun.cpp ASU_tools.hpp
	$(COMP) -c STDCC.fun.cpp $(INCLUDE)
SolvePoly.fun.o: SolvePoly.fun.cpp ASU_tools.hpp
	$(COMP) -c SolvePoly.fun.cpp $(INCLUDE)
SH_Coeff.fun.o: SH_Coeff.fun.cpp ASU_tools.hpp
	$(COMP) -c SH_Coeff.fun.cpp $(INCLUDE)
Combination.fun.o: Combination.fun.cpp ASU_tools.hpp
	$(COMP) -c Combination.fun.cpp $(INCLUDE)

clean:
	rm -f *.fun.o libASU_tools_cpp.a
