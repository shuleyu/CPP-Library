#ifndef ASU_HANNTAPER
#define ASU_HANNTAPER

#include<iostream>
#include<cmath>
#include<vector>

/***********************************************************
 * This C++ template taper both ends of the input signal,
 * using a Hanning Window.
 *
 * input(s):
 * vector<T>    &p  ----  Signal that get tapered. length is npts.
 * const double &w  ----  Taper one-side length compared to npts.
 *                        (width max at 0.5, which means the taper
 *                        covers the whole signal)
 *
 *                        e.g.
 *
 *                         Signal:    ************************* (npts=25)
 *                         Target:    tttt*****************tttt (taper one-side length=4)
 *
 *                                    --> input w should be 4/25.
 *
 * return(s):
 * vector<T> &p (in-place)
 * Shule Yu
 * Jan 22 2018
 *
 * Key words: Hanning window taper.
 *
 * Reference: https://en.wikipedia.org/wiki/Hann_function
***********************************************************/

template<typename T>
void HannTaper(std::vector<T> &p,const double &w){

    if (w>0.5) {
        std::cerr <<  "Error in " << __func__ << ": taper window too big ..." << std::endl;
        return;
    }

    int n=p.size(),N=round(n*w);
    if (N<=1) return;

    double df=M_PI/(N-1);

    for (int i=0;i<N;++i){
        double weight=0.5-0.5*cos(df*i);
        p[i]*=weight;
        p[n-1-i]*=weight;
    }
    return;
}

#endif
