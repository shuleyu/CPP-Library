#ifndef ASU_MOLLIFIERFUNCTION
#define ASU_MOLLIFIERFUNCTION

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
 * input(s):
 * const double &x      ----  x.
 * const double &delta  ----  delta.
 *
 * return(s):
 * double ans  ----  Value of the mollifier function.
 *
 * Shule Yu
 * Dec 29 2017
 *
 * Key words: mollifier
 *
 * Reference: https://en.wikipedia.org/wiki/Mollifier#Concrete_example
***********************************************************/

inline double MollifierFunction(const double &x, const double &delta){
    if (fabs(x)>=delta) return 0;
    else return exp(-1.0/(1-(1.0*x/delta)*(1.0*x/delta)))/delta;
}

#endif
