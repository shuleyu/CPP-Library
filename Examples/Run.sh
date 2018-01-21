#!/bin/bash

set -e

[ $# -ne 1 ] || ! [ -e $1 ] && echo "Usage: input c++ code as \$1." && exit 1;

SRCDIR=${PWD}
CPPCODEDIR=${PWD}/..
SACHOME=/opt/sac
INCLUDE="-I${CPPCODEDIR} -I${SACHOME}/include -I/opt/local/include -I/usr/include"
LIBDIR="-L${CPPCODEDIR} -L${SACHOME}/lib -L/opt/local/lib"
LIBs="-lASU_tools_cpp -lsac -lsacio -lmlpack -larmadillo -lgsl -lgslcblas -lfftw3 -lm"

cd ${CPPCODEDIR}
make
cd ${SRCDIR}

# ======== Compile c++ code ========

c++ -std=c++14 -Wall $1 ${INCLUDE} ${LIBDIR} ${LIBs}
[ $? -ne 0 ] && echo "$1 compile failed ..." && exit 1

# ======== Run Code ========
time ./a.out
# rm -f a.out

echo ""

exit 0
