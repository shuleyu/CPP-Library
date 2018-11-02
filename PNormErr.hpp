#ifndef ASU_PNORMERR
#define ASU_PNORMERR

#include<iostream>
#include<vector>
#include<cmath>

#include<PNorm.hpp>

/*************************************************
 * This C++ template returns the p-norm difference
 * between two input arrays.
 *
 * definition is:
 *
 *  | x - y | _ p
 * ----------------
 *    | y | _ p
 *
 * input(s):
 * const <T1> &x  ----  Input array.
 * const <T2> &y  ----  Model array.
 * const int  &p  ----  p.
 *
 * return(s):
 * double ans  ----  p-norm difference between x and y
 *
 * Shule Yu
 * Jan 10 2018
 *
 * Key words: p norm error.
*************************************************/

template<typename T1, typename T2>
double PNormErr(const T1 XBegin, const T1 XEnd, const T2 YBegin, const T2 YEnd, const int &p){

    std::size_t m=std::distance(XBegin,XEnd),n=std::distance(YBegin,YEnd);

    if (m!=n) {
        std::cerr <<  "Error in " << __func__ << ": input arrays x and y size don't match ..." << std::endl;
        return 0;
    }
    if (m<=0) return 0;
    if (p==0) {
        std::cerr <<  "Error in " << __func__ << ": p is zero ..." << std::endl;
        return 0;
    }

    double Sum=0;
    for (std::size_t i=0;i<m;++i) Sum+=pow(fabs((*std::next(XBegin,i))-(*std::next(YBegin,i))),p);

    return pow(Sum,1.0/p)/PNorm(YBegin,YEnd,p);
}

template<typename T1, typename T2>
double PNormErr(const std::vector<T1> &x, const std::vector<T2> &y, const int &p){
    return PNormErr(x.begin(),x.end(),y.begin(),y.end(),p);
}

#endif
