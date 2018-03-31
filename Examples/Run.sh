#!/bin/bash

[ $# -ne 1 ] || ! [ -e $1 ] && echo "Usage: input c++ code as \$1." && exit 1;
CodeName=$1

SRCDIR=${PWD}
CPPCODEDIR=${PWD}/..
cd ${CPPCODEDIR}
make examples
[ $? -ne 0 ] && exit 1

cd ${SRCDIR}
time ./${CodeName%cpp}out

echo ""

exit 0
