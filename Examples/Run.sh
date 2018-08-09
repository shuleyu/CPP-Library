#!/bin/bash

# For GMT.out to run, need to add dynamic library linking path:
GMTDIR=/opt/gmt-5.4.3
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${GMTDIR}/lib64

[ $# -ne 1 ] || ! [ -e $1 ] && echo "Usage: input c++ code as \$1." && exit 1;
CodeName=$1
CodeName=${CodeName%.*}

make Code=${CodeName} GMTDIR=${GMTDIR}
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
