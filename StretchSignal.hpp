#ifndef ASU_STRETCHSIGNAL
#define ASU_STRETCHSIGNAL

#include<iostream>
#include<cmath>
#include<vector>

#include<Interpolate.hpp>

/***********************************************************
 * This C++ template stretch/squeeze input signal horizontally
 * according to the given parameter.
 *
 * inpput(s):
 * const vector<T> &p  ----  Input signal. signal length is npts.
 * const double     &r  ----  Horizontal stretch factor. output signal length is r*npts.
 *
 *                             e.g.
 *                             r = 1 means the original trace.
 *                             r = 0.5 gives a squeezed trace. (squeeze by half)
 *                             r = 2 gives stretched trace. (stretch to 2 times wide)
 * return(s):
 * vector<double> ans  ----  Stretched trace.
 *
 * Shule Yu
 * Jan 21 2018
 *
 * Key words: stretch signal
 * Notice: Will not check aliasing effects. Use with caution.
***********************************************************/

template<typename T>
std::vector<double> StretchSignal(const std::vector<T> &p, const double &r){

    std::vector<double> ans;

    if (p.size()<=1) {
        std::cerr <<  "Error in " << __func__ << ": input array size <=1 ..." << std::endl;
        return ans;
    }

    int n=p.size(),N=1+round(1.0*(n-1)*r);
    if (N<=1) return ans;

    std::vector<double> x(n,0);
    for (int i=0;i<n;++i) x[i]=i;


    double dt=1.0*(n-1)/(N-1);
    std::vector<double> xx(N,0);
    for (int i=0;i<N;++i) xx[i]=i*dt;

    ans=Interpolate(x,p,xx);

    return ans;
}

#endif
