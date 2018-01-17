#ifndef ASU_COMPLEXDIVIDE
#define ASU_COMPLEXDIVIDE

#include<iostream>

/******************************************************
 * This C++ template return division result between two
 * complex number:
 *
 *        a+bi
 * e+fi= ------
 *        c+di
 *
 * const T1 &a   ----  a.
 * const T2 &b   ----  a.
 * const T3 &c   ----  a.
 * const T4 &d   ----  a.
 *
 * Shule Yu
 * Dec 28 2017
 *
 * Key words: complex, divide, division.
******************************************************/

template<class T1, class T2, class T3, class T4>
std::pair<double,double> ComplexDivide(const T1 &a, const T2 &b, const T3 &c, const T4 &d){

	// Check denominator.
    if (c==0 && d==0){
		std::cerr <<  __func__ << "; Error: denominator is zero ..." << std::endl;
        return {};
    }

    return {1.0*(a*c+b*d)/(c*c+d*d),1.0*(b*c-a*d)/(c*c+d*d)};
}

#endif
