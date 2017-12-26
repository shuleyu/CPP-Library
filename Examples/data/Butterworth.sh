#!/bin/bash


# Generate files:
# 
# 	Butterworth_infile
# 	Butterworth_bp_SAC
# 	Butterworth_lp_SAC
# 	Butterworth_hp_SAC

sac >/dev/null << EOF
fg seis
w alpha tmpfile.txt
fg seis
BANDPASS BUTTER CORNER 0.1 0.4 NPOLES 2 PASSES 2
w alpha tmpfile_bp.txt
fg seis
LOWPASS BUTTER CORNER 0.4 NPOLES 2 PASSES 2
w alpha tmpfile_lp.txt
fg seis
HIGHPASS BUTTER CORNER 0.1 NPOLES 2 PASSES 2
w alpha tmpfile_hp.txt
q
EOF

awk 'NR>30 {print $1"\n"$2"\n"$3"\n"$4"\n"$5}' tmpfile.txt > tmpfile_$$
sed '/^\s*$/d' tmpfile_$$ > Butterworth_infile

awk 'NR>30 {print $1"\n"$2"\n"$3"\n"$4"\n"$5}' tmpfile_bp.txt > tmpfile_$$
sed '/^\s*$/d' tmpfile_$$ > Butterworth_bp_SAC 

awk 'NR>30 {print $1"\n"$2"\n"$3"\n"$4"\n"$5}' tmpfile_lp.txt > tmpfile_$$
sed '/^\s*$/d' tmpfile_$$ > Butterworth_lp_SAC 

awk 'NR>30 {print $1"\n"$2"\n"$3"\n"$4"\n"$5}' tmpfile_hp.txt > tmpfile_$$
sed '/^\s*$/d' tmpfile_$$ > Butterworth_hp_SAC 

rm -f tmpfile*

exit 0
