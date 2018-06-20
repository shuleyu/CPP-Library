#!/bin/bash

# ====================================================================
# This script shows an example to read parameters from command line.
#
# Shule Yu
# Jun 19 2018
# ====================================================================

make Code=ReadParameters
[ $? -ne 0 ] && exit 1

# C++ code.
./ReadParameters.out 2 2 3 << EOF
1
4
Yes,Minister
Humphrey
9.4
9.5
9.0
EOF

exit 0
