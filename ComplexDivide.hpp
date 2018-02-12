#ifndef ASU_COMPLEXDIVIDE
#define ASU_COMPLEXDIVIDE

#include<iostream>

/******************************************************
 * This C++ template return division result between two
 * complex numbers:
 *
 *        a+bi
 * e+fi= ------
 *        c+di
 *
 * input(s):
 * const T1 &a   ----  a.
 * const T2 &b   ----  b.
 * const T3 &c   ----  c.
 * const T4 &d   ----  d.
 *
 * return(s):
 * pair<double,double> ans  ----  {e,f}
 *
 * Shule Yu
 * Dec 28 2017
 *
 * Key words: complex, divide, division.
******************************************************/

template<class T1, class T2, class T3, class T4>
inline std::pair<double,double> ComplexDivide(const T1 &a, const T2 &b, const T3 &c, const T4 &d){

    // Check denominator.
    if (c==0 && d==0){
        std::cerr <<  "Error in " << __func__ << ": denominator is zero ..." << std::endl;
        return {};
    }

    return {1.0*(a*c+b*d)/(c*c+d*d),1.0*(b*c-a*d)/(c*c+d*d)};
}

#endif
