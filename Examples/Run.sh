#!/bin/bash

[ $# -ne 1 ] || ! [ -e $1 ] && echo "Usage: input c++ code as \$1." && exit 1;

SRCDIR=${PWD}
CCODEDIR=/home/shule/Research/Fun.C.c002
CPPCODEDIR=${PWD}/../
SACHOME=/opt/sac
INCLUDE="-I${CPPCODEDIR} -I${CCODEDIR} -I${SACHOME}/include -I/opt/local/include -I/usr/include"
LIBDIR="-L${CPPCODEDIR} -L${CCODEDIR} -L${SACHOME}/lib -L/opt/local/lib"
LIBs="-lASU_tools_cpp -lASU_tools -lsac -lsacio -lmlpack -larmadillo -lgsl -lgslcblas -lfftw3 -lm"

cd ${CCODEDIR}
make
cd ${CPPCODEDIR}
make
cd ${SRCDIR}

# ======== Compile c++ code ========

c++ -std=c++14 -Wall $1 ${INCLUDE} ${LIBDIR} ${LIBs}
[ $? -ne 0 ] && echo "$1 compile failed ..." && exit 1

# ======== Run Code ========
./a.out

exit 0
