#ifndef ASU_PNORM
#define ASU_PNORM

#include<iostream>
#include<vector>
#include<cmath>

/***********************************************************
 * This C++ template returns the p-norm of input vector.
 *
 * input(s):
 * const <T> &x  ----  Input array.
 * const int &p  ----  p.
 *
 * return(s):
 * double ans  ----  (Sum(|x|^p))^(1/p)
 *
 * Shule Yu
 * Jan 10 2018
 *
 * Key words: p norm.
 *
 * Reference: https://en.wikipedia.org/wiki/Norm_(mathematics)#p-norm
***********************************************************/

template<typename T>
double PNorm(const T XBegin, const T XEnd, const int &p){
    std::size_t n=std::distance(XBegin,XEnd);
    if (n<=0) return 0;
    if (p==0) {
        std::cerr <<  "Error in " << __func__ << ": p is zero ..." << std::endl;
        return 0;
    }

    double Sum=0;

    for (auto it=XBegin;it!=XEnd;++it) Sum+=pow(fabs(*it),p);

    return pow(Sum,1.0/p);
}

template<typename T>
double PNorm(const std::vector<T> &x, const int &p){
    return PNorm(x.begin(),x.end(),p);
}

#endif
