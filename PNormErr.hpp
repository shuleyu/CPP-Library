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
 * const <T1> &x  ----  Input array.
 * const <T2> &y  ----  Model array.
 * const T3   &p  ----  p.
 *
 * Shule Yu
 * Jan 10 2018
 *
 * Key words: p norm error.
***********************************************************/

template<class T1, class T2, class T3>
double PNormErr(const std::vector<T1> &x, const std::vector<T2> &y, const T3 &p){

    if (x.size()!=y.size()) {
		std::cerr <<  __func__ << "; Error: input arrays x and y size don't match ..." << std::endl;
		return 0;
	}
    if (x.size()<=0) return 0;
    if (p==0) {
		std::cerr <<  __func__ << "; Error: p is zero ..." << std::endl;
		return 0;
	}

    double Sum=0;
	for (size_t i=0;i<x.size();++i) Sum+=pow(fabs(x[i]-y[i]),p);

    return pow(Sum,1.0/p)/PNorm(y,p);
}

#endif
