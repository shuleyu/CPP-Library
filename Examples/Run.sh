#!/bin/bash

[ $# -ne 1 ] || ! [ -e $1 ] && echo "Usage: input c++ code as \$1." && exit 1;
CodeName=$1
CodeName=${CodeName%.*}

make Code=${CodeName} SAC=${SACHOME}
[ $? -ne 0 ] && exit 1

time ./${CodeName}.out

echo ""

# Remove tabs
# for file in `ls *cpp`
# do
#     sed 's/\t/    /g' ${file} > x
#     mv x ${file}
# done


exit 0
