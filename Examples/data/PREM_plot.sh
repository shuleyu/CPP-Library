#!/bin/bash

# In Example directory, run the following first:
# ./Run.sh prem.c

awk 'NR>1 {print $1,$2}' PREM_out > vpv.dat
awk 'NR>1 {print $1,$3}' PREM_out > vph.dat
awk 'NR>1 {print $1,$4}' PREM_out > vsv.dat
awk 'NR>1 {print $1,$5}' PREM_out > vsh.dat
awk 'NR>1 {print $1,$6}' PREM_out > rho.dat
awk 'NR>1 {print $1,$2}' PREMSmoothed_out > vp_smoothed.dat
awk 'NR>1 {print $1,$3}' PREMSmoothed_out > vs_smoothed.dat
awk 'NR>1 {print $1,$4}' PREMSmoothed_out > rho_smoothed.dat
awk 'NR>1 {print $1,$2}' PREMX_out > vp_x.dat
awk 'NR>1 {print $1,$3}' PREMX_out > vs_x.dat
awk 'NR>1 {print $1,$4}' PREMX_out > rho_x.dat

gmtset GRID_PEN_PRIMARY = 0.1p,200/200/200,-
gmtset ANNOT_FONT_PRIMARY = 12p

psbasemap -JX6i/-9.5i -R0/14/0/3000 -X1.4i -Y1i -P -Ba5f1:"Density purple (kg/m^3)":S -K > PREM.ps
psbasemap -J -R -Ba2f2g2:"Velocities (km/s)":/a500f500g250:"Depth (km)":WNe -O -K >> PREM.ps

psxy vsh.dat -: -J -R -m -W1p,red,. -O -K >> PREM.ps
psxy vph.dat -: -J -R -m -W1p,blue,. -O -K >> PREM.ps
psxy rho.dat -: -J -R -m -W2p,purple -O -K >> PREM.ps
psxy vsv.dat -: -J -R -m -W2p,red -O  -K >> PREM.ps
psxy vpv.dat -: -J -R -m -W2p,blue -O -K >> PREM.ps
psxy rho_smoothed.dat -: -J -R -m -W1p,black,- -O -K >> PREM.ps
psxy vp_smoothed.dat -: -J -R -m -W1p,black,- -O -K >> PREM.ps
psxy vs_smoothed.dat -: -J -R -m -W1p,black,- -O -K >> PREM.ps
psxy rho_x.dat -: -J -R -m -W1p,green,- -O -K >> PREM.ps
psxy vp_x.dat -: -J -R -m -W1p,green,- -O -K >> PREM.ps
psxy vs_x.dat -: -J -R -m -W1p,green,- -O >> PREM.ps

ps2pdf PREM.ps PREM.pdf

rm -f PREM.ps rho.dat vpv.dat vph.dat vsv.dat vsh.dat rho_smoothed.dat vp_smoothed.dat vs_smoothed.dat rho_x.dat vp_x.dat vs_x.dat

exit 0
