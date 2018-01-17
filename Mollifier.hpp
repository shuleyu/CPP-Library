#ifndef ASU_MOLLIFIER
#define ASU_MOLLIFIER

#include<cmath>

/***********************************************************
 * This C++ template returns the value of mollifier function
 * for the given width (delta).
 *
 * The formula for the mollifier function is defined as:
 *
 * f(x)=1/delta*g(x/delta).
 *
 * the formula for g is:
 *
 * g(x)=exp(-1/(1-|x|^2))
 *
 * const T1 &x      ----  x.
 * const T2 &delta  ----  delta.
 *
 * Shule Yu
 * Dec 29 2017
 *
 * Key words: mollifier
 *
 * Reference: https://en.wikipedia.org/wiki/Mollifier#Concrete_example
***********************************************************/

template<class T1, class T2>
double Mollifier(const T1 &x, const T2 &delta){

	if (fabs(x)>=delta) return 0;
    else return exp(-1.0/(1-(1.0*x/delta)*(1.0*x/delta)))/delta;
}

#endif
