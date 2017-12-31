#include<cmath>

#include<ASU_tools.hpp>

/***********************************************************
 * This C function uses wiggins algorithm to interpolate for
 * higher sampling rate. While used in down sampling may
 * results in aliasing effects.
 *
 * double *x       ----  Origin signal x.
 * double *y       ----  Origin signal y=y(x).
 * int    npts     ----  Origin signal length.
 * double *xx      ----  Target xx.
 * double *yy      ----  Target yy.
 * int    Newnpts  ----  Target signal length.
 * boo    IsEven   ----  true means x is equally sampled
 *                       false means x is inconsistantly sampled.
 *
 * Shule Yu
 * Mar 27 2014
 *
 * Key words: interpolate, wiggins
 *
 * Reference:
 *     SAC source code.
***********************************************************/

void Wiginterp(double *x, double *y, int npts, double *xx, double *yy, int Newnpts, bool IsEven){

    double dx,epsi;
	if (IsEven) {
        dx=(*(x+1))-(*x);
        epsi=GetEps1(y,npts,dx);
	}
    else {
        epsi=GetEps2(y,npts,x);
        dx=0.0;
    }

    // taken from sac source code wigint.c
	int j, n1;
	double a, am, amd, amu, dxd, dxj, dxj1, dxj1s, dxjs, dxu, dy, dyd,
	 dyu, h, hc, hs, sp, sp1, t1, t2, t3, t4, w, wd, wu;

	double *const X = &x[0] - 1;
	double *const Y = &y[0] - 1;
    double t;

	/*=====================================================================
	 * PURPOSE:  Interpolates evenly or unevenly spaced data.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    X:       X array if unevenly spaced, first x if even. [f]
	 *    Y:       Y array. [fa]
	 *    NPTS:    Length of (X and) Y arrays. [i]
	 *    DX:      Set to 0.0 if unevenly spaced, to sampling interval
	 *             if evenly spaced. [f]
	 *    EPSI:    water level used in interpolation [f]
	 *    T:       Time value to interpolate to. [f]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    F:       Interpolated y value. [f]
	 *=====================================================================
	 * MODULE/LEVEL:  SCM/4
	 *=====================================================================
	 * REFERENCE: Wiggins, 1976, BSSA, 66, p.2077.
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    100719:  no option on epsi -- just takes input value (jas/vt)
         *    970202:  Modified order of input arguments.
	 *    86xxxx:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  870202 (Prolog only.)
	 *===================================================================== */
    int count;
    double *movex=xx;
    double *movey=yy;
    for (count=0;count<Newnpts;count++){

        t=*movex;

        if( dx == 0. )
            goto L_10;
        j = (int)( (t - X[1])/dx );
        dxj = t - X[1] - (double)( j )*dx;
        j = j + 1;
        if( dxj == 0. )
            goto L_99;
        h = dx;
        dxj1 = dxj - h;
        /* dxd = h; */
        /* dxu = h; */
        goto L_50;
L_10:
        for( j = 1; j <= npts; j++ ){
            a = X[j] - t;
            if( a > 0. )
                goto L_30;
        }
L_30:
        j = j - 1;
        dxj = t - X[j];
        if( dxj == 0. )
            goto L_99;
        h = X[j + 1] - X[j];
        dxj1 = t - X[j + 1];
L_50:
        if(j == npts)
            goto L_99;
        hs = h*h;
        hc = hs*h;
        dxjs = dxj*dxj;
        dxj1s = dxj1*dxj1;
        dy = Y[j + 1] - Y[j];
        am = dy/h;
        amd = am;
        amu = am;
        if( j == 1 )
            goto L_60;
        if( dx != 0. ){
            dxd = dx;
            }
        else{
            dxd = X[j] - X[j - 1];
            }
        dyd = Y[j] - Y[j - 1];
        amd = dyd/dxd;
L_60:
        n1 = j + 1;
        if( n1 == npts )
            goto L_70;
        if( dx != 0. ){
            dxu = dx;
        }
        else{
            dxu = X[j + 2] - X[j + 1];
        }
        dyu = Y[j + 2] - Y[j + 1];
        amu = dyu/dxu;
L_70:
        wd = 1./fmax( fabs( amd ), epsi );
        w = 1./fmax( fabs( am ), epsi );
        wu = 1./fmax( fabs( amu ), epsi );
        sp = (wd*amd + w*am)/(wd + w);
        sp1 = (w*am + wu*amu)/(w + wu);
        t1 = Y[j]*(dxj1s/hs + 2.*dxj*dxj1s/hc);
        t2 = Y[j + 1]*(dxjs/hs - 2.*dxj1*dxjs/hc);
        t3 = sp*dxj*dxj1s/hs;
        t4 = sp1*dxjs*dxj1/hs;
        *movey = t1 + t2 + t3 + t4;
        goto L_100;
L_99:
        *movey = Y[j];
L_100:

        movex++;
        movey++;
    }

    return ;
}

double GetEps1(double *y, int npts, double del) {

    double EPS=0;
    int    count;

    for(count=0;count<npts-1;count++){
        EPS+=fabs(y[count+1]-y[count]);
    }
    EPS*=(1e-4/(npts-1)/del);
    return EPS;
}

double GetEps2(double *y, int npts, double *x) {

    double EPS=0;
    int    count;

    for(count=0;count<npts-1;count++){
        EPS+=fabs((y[count+1]-y[count])/(x[count+1]-x[count]));
    }
    EPS*=(1e-4/(npts-1));
    return EPS;
}
