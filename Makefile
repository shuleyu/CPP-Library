
CC = c++
FLAGS = --std=c++14 -Wall -fPIC
COMP = $(CC) $(FLAGS)

SACHOME = /opt/sac-101.6
INCLUDE = -I. -I/opt/local/include -I$(SACHOME)/include

all: libASU_tools_cpp.a
	@echo > /dev/null

libASU_tools_cpp.a: Combination.fun.o Decimal2Binary.fun.o GrayCode.fun.o ShellExec.fun.o Wiginterp.fun.o
	ar cr libASU_tools_cpp.a Combination.fun.o Decimal2Binary.fun.o GrayCode.fun.o ShellExec.fun.o Wiginterp.fun.o

Combination.fun.o: Combination.fun.cpp ASU_tools.hpp
	$(COMP) -c Combination.fun.cpp $(INCLUDE)
Decimal2Binary.fun.o: Decimal2Binary.fun.cpp ASU_tools.hpp
	$(COMP) -c Decimal2Binary.fun.cpp $(INCLUDE)
GrayCode.fun.o: GrayCode.fun.cpp ASU_tools.hpp
	$(COMP) -c GrayCode.fun.cpp $(INCLUDE)
ShellExec.fun.o: ShellExec.fun.cpp ASU_tools.hpp
	$(COMP) -c ShellExec.fun.cpp $(INCLUDE)
Wiginterp.fun.o: Wiginterp.fun.cpp ASU_tools.hpp
	$(COMP) -c Wiginterp.fun.cpp $(INCLUDE)

clean:
	rm -f *.fun.o libASU_tools_cpp.a
