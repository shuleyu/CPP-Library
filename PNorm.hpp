#ifndef ASU_PNORM
#define ASU_PNORM

#include<iostream>
#include<vector>
#include<cmath>

/***********************************************************
 * This C++ template returns the p-norm of input vector.
 *
 * input(s):
 * const <T1> &x  ----  Input array.
 * const T2   &p  ----  p.
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

template<class T1, class T2>
double PNorm(const T1 XBegin, const T1 XEnd, const T2 &p){
    size_t n=std::distance(XBegin,XEnd);
    if (n<=0) return 0;
    if (p==0) {
        std::cerr <<  "Error in " << __func__ << ": p is zero ..." << std::endl;
        return 0;
    }

    double Sum=0;

    for (auto it=XBegin;it!=XEnd;++it) Sum+=pow(fabs(*it),p);

    return pow(Sum,1.0/p);
}

template<class T1, class T2>
double PNorm(const std::vector<T1> &x, const T2 &p){
    return PNorm(x.begin(),x.end(),p);
}

#endif
