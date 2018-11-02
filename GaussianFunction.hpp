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
 * const double &x      ----  x.
 * const double &sigma  ----  standard deviation, sigma.
 * const double &mu     ----  mean, mu.
 *
 * return(s):
 * double ans  ----  Gaussian function value.
 *
 * Shule Yu
 * Jan 01 2018
 *
 * Key words: gaussian function.
***********************************************************/

double GaussianFunction(const double &x, const double &sigma, const double &mu){

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
