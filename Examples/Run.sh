#!/bin/bash

if [ $# -ne 1 ] || ! [ -e $1 ]
then
    echo "Usage: input c code as \$1."
    exit 1;
fi

SRCDIR=${PWD}
CCODEDIR=/home/shule/Research/Fun.C.c002
CPPCODEDIR=/home/shule/Research/Fun.C++.c003
SACHOME=/opt/sac

INCLUDE="-I${CPPCODEDIR} -I${CCODEDIR} -I${SACHOME}/include -I/opt/local/include -I/usr/include"
LIBDIR="-L${CPPCODEDIR} -L${CCODEDIR} -L${SACHOME}/lib -L/opt/local/lib"
LIBs="-lASU_tools -lsac -lsacio -lgsl -lgslcblas -lfftw3 -lm "


# ======== Compile c++ code ========

c++ -std=c++14 -Wall $1 ${INCLUDE} ${LIBDIR} ${LIBs}

# ======== Run c++ code ========
if [ $? -ne 0 ]
then
    echo "$1 compile failed ..."
    exit 1
fi

./a.out

exit 0
