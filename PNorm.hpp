#ifndef ASU_PNORM
#define ASU_PNORM

#include<iostream>
#include<vector>
#include<cmath>

#include<ASU_tools.hpp>

/*******************************************************
 * This C++ template returns the p-norm of input vector.
 *
 * const <T1> &x  ----  Input array.
 * const T2   &p  ----  p.
 *
 * Shule Yu
 * Jan 10 2018
 *
 * Key words: p norm.
 *
 * Reference: https://en.wikipedia.org/wiki/Norm_(mathematics)#p-norm
***********************************************************/

template<class T1, class T2>
double PNorm(const std::vector<T1> &x, const T2 &p){

    if (x.size()<=0) return 0;
    if (p==0) {
		std::cerr <<  __func__ << "; Error: p is zero ..." << std::endl; 
		return 0;
	}

    double Sum=0;

	for (const auto &item:x) Sum+=pow(fabs(item),p);

    return pow(Sum,1.0/p);
}

#endif
