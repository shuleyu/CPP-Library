#ifndef ASU_CONFIDENCEINTERVAL
#define ASU_CONFIDENCEINTERVAL

#include<iostream>
#include<vector>
#include<cmath>
#include<gsl/gsl_cdf.h>

#include<ASU_tools.hpp>

/*****************************************************************
 * This C++ template returns the Level% confidence interval of
 * input measurements (p). Assuming the normal distribution error.
 *
 * Formula is:
 *
 * I = ta,n-1 * Sn / sqrt(n)
 *
 * const vector<T1> &p      ----  Measurements.
 * const double     &Level  ----  Request confidence level. ( 95% use 95 )
 *
 * Shule Yu
 * Apr 20 2015
 *
 * Key words: confidence level.
 *
 * Reference: 
 *     https://en.wikipedia.org/wiki/Student%27s_t-distribution#Confidence_intervals
*****************************************************************/

template<class T1>
double ConfidenceInterval(std::vector<T1> &p,const double &Level){

    if (p.size()<=1) return 0;
	int n=p.size();

    return gsl_cdf_tdist_Pinv(1-(1-Level*1.0/100)/2,n-1)*AvrStd(p).second/sqrt(n);
}

#endif
