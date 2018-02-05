#ifndef ASU_GAUSSIANSIGNAL
#define ASU_GAUSSIANSIGNAL

#include<iostream>
#include<vector>

#include<GaussianFunction.hpp>

/***********************************************************
 * This C++ template returns the a gaussian function.
 *
 * The return signal's peak is at the center (N/2) with
 * amplitude 1/sqrt(2pi)/sigma.
 *
 * For other points, x value is calculated by sampling rate.
 * Then amplitude is calculated as:
 *
 * f(x)=1/sqrt(2pi)/sigma * e^{-x^2/2/sigma^2}
 *
 * const int  &N      ----  Requested signal length.
 * const T1   &dt     ----  Requested sampilng rate.
 * const T2   &sigma  ----  standard deviation, sigma.
 *
 * return(s):
 * vector<double> ans  ----  output signal.
 *
 * Shule Yu
 * Jan 01 2018
 *
 * Key words: gaussian function.
***********************************************************/

template<class T1, class T2>
std::vector<double> GaussianSignal(const int &N, const T1 &dt, const T2 &sigma){

	// Check parameters.

	if (sigma<0 || dt<=0) {
		std::cerr <<  __func__ << "; Error: input sigma < 0 or dt <= 0 ..." << std::endl;
		return {};
	}

	if (N<=0) return {};

	// Construct the signal.
	std::vector<double> ans(N,0);

    ans[N/2]=GaussianFunction(0,sigma,0);

	if (sigma==0) return {};

    for (int i=N/2+1;i<N;++i){
        ans[i]=GaussianFunction((i-N/2)*dt,sigma,0);
        ans[(N&(~1))-i]=ans[i];
    }
	if (N%2==0) ans[0]=GaussianFunction(N/2*dt,sigma,0);

	return ans;
}

#endif
