#ifndef ASU_INTERPOLATE
#define ASU_INTERPOLATE

#include<iostream>
#include<vector>
#include<cmath>
#include<algorithm>

/****************************************************************
 * This c++ template is only a wrapper for the Wiginterp function
 * included in same library. I did some modification: now whether
 * x is ascending or descending doesn't matter. The only
 * restriction on x is stric monotonic. The SAC source code is in
 * Wiginterp.fun.cpp.
 *
 * input(s):
 * const vector<T1> &x       ----  Time of original signal.
 * const vector<T2> &y       ----  Amplitudes of original signal.
 * const vector<T3> &xx      ----  Time of interpolated signal.
 *
 * return(s):
 * vector<double> yy  ----  Interpolated signal.
 * Shule Yu
 * Nov 14 2017
 *
 * Key words: interpolate, wiggins
****************************************************************/

template<class T1, class T2, class T3>
std::vector<double> Interpolate(const std::vector<T1> &x, const std::vector<T2> &y, const std::vector<T3> &xx){

    // Check array size.
    int n=x.size();
    if (x.size()!=y.size() || n<=1) {
        std::cerr <<  __func__ << "; Error: input arrays size <=1 or don't match ..." << std::endl;
        return {};
    }

    // Check x is strictly sorted.

    auto cmp=[](const T1 &x, const T1 &y){
        return x<=y;
    };

    if (!std::is_sorted(x.begin(),x.end(),cmp) && !std::is_sorted(x.rbegin(),x.rend(),cmp)) {
        std::cerr <<  __func__ << "; Error: input x is either not sorted or has repeating value ..." << std::endl;
        return {};
    }

    // check is x even sampling.
    double dx;
    for (size_t i=1;i<x.size();++i){
        double d=x[i]-x[i-1];
        if (i==1) dx=d;
        else if (fabs(d)<=fabs(dx)*0.99 || fabs(dx)*1.01<=fabs(d)) {
            dx=0;
            break;
        }
    }

    // Calculate epsi.
    double epsi=0;
    for (size_t i=1;i<x.size();++i)
        epsi+=fabs((y[i]-y[i-1])/(x[i]-x[i-1]));
    epsi*=(1e-4/(n-1));

    if (epsi==0) return std::vector<double> (xx.size(),y[0]);

    // modified from sac source code wigint.c

    std::vector<double> yy(xx.size(),0);
    double Min=std::min(x[0],x.back()),Max=std::max(x[0],x.back());

    for (size_t i=0;i<xx.size();++i) {

        if (xx[i]<Min || xx[i]>Max) {
            yy[i]=0.0/0.0;
            continue;
        }

        int j;
        double ld,lu,h;

        if (dx==0) {
            for (j=0;j<n;++j){
                if ( (x[0]<x.back() && x[j]>=xx[i]) || (x[0]>x.back() && x[j]<=xx[i]) )
                    break;
            }
            if (xx[i]==x[j]) {
                yy[i]=y[j];
                continue;
            }
        }
        else {
            j=(int)((xx[i]-x[0])/dx);
            if (xx[i]==x[j]) {
                yy[i]=y[j];
                continue;
            }
            ++j;
        }
        h=x[j]-x[j-1];
        ld=xx[i]-x[j-1];
        lu=xx[i]-x[j];

        double hs=h*h,hc=hs*h,lds=ld*ld,lus=lu*lu;

        double amd,amu,am;
        amd=amu=am=(y[j]-y[j-1])/h;
        if (j!=1) amd=(y[j-1]-y[j-2])/(dx==0?x[j-1]-x[j-2]:dx);
        if (j!=n-1) amu=(y[j+1]-y[j])/(dx==0?x[j+1]-x[j]:dx);

        double w, wd, wu;
        wd = 1.0/std::max( fabs( amd ), epsi );
        w  = 1.0/std::max( fabs( am  ), epsi );
        wu = 1.0/std::max( fabs( amu ), epsi );

        double spd, spu;
        spd = (wd*amd + w*am)/(wd + w);
        spu = (w*am + wu*amu)/(w + wu);

        double t1, t2, t3, t4;
        t1 = y[j-1]*(lus/hs + 2*ld*lus/hc);
        t2 = y[j]  *(lds/hs - 2*lu*lds/hc);
        t3 = spd*ld*lus/hs;
        t4 = spu*lu*lds/hs;
        yy[i] = t1 + t2 + t3 + t4;
    }
    return yy;
}

#endif
