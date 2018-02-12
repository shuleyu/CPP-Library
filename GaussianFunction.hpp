#ifndef ASU_GAUSSIANFUNCTION
#define ASU_GAUSSIANFUNCTION

#include<iostream>
#include<cmath>

/***********************************************************
 * This C++ template returns the value of gaussian function.
 *
 * The amplitude of the peak is 1/sqrt(2pi)/sigma: if
 * integrated from -\infty to \infty the result is 1.
 *
 * Namely, the return is:
 *
 * f(x)=1/sqrt(2pi)/sigma * e^{-(x-mu)^2/2/sigma^2}
 *
 * input(s):
 * const T1 &x      ----  x.
 * const T2 &sigma  ----  standard deviation, sigma.
 * const T3 &mu     ----  mean, mu.
 *
 * return(s):
 * double ans  ----  Gaussian function value.
 *
 * Shule Yu
 * Jan 01 2018
 *
 * Key words: gaussian function.
***********************************************************/

template<class T1, class T2, class T3>
double GaussianFunction(const T1 &x, const T2 &sigma, const T3 &mu){

    if (sigma<0) {
        std::cerr <<  "Error in " << __func__ << ": input sigma < 0 ..." << std::endl;
        return 0;
    }
    if (sigma==0){
        if (x==mu) return 1;
        else return 0;
    }
    else return exp(-0.5*(x-mu)*(x-mu)/sigma/sigma)/sigma/sqrt(2*M_PI);
}

#endif
