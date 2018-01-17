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
 * vector<T1> &p      ----  output signal.
 * const int  &N      ----  Requested signal length.
 * const T2   &dt     ----  Requested sampilng rate.
 * const T3   &sigma  ----  standard deviation, sigma.
 *
 * Shule Yu
 * Jan 01 2018
 *
 * Key words: gaussian function.
***********************************************************/

template<class T1, class T2, class T3>
void GaussianSignal(std::vector<T1> &p, const int &N, const T2 &dt, const T3 &sigma){

	p.clear();

	// Check parameters.

	if (sigma<0 || dt<=0) {
		std::cerr <<  __func__ << "; Error: input sigma < 0 or dt <= 0 ..." << std::endl;
		return;
	}

	if (N<=0) return;

	// Construct the signal.
	p.resize(N,0);

    p[N/2]=GaussianFunction(0,sigma,0);

	if (sigma==0) return;

    for (int i=N/2+1;i<N;++i){
        p[i]=GaussianFunction((i-N/2)*dt,sigma,0);
        p[(N&(~1))-i]=p[i];
    }
	if (N%2==0) p[0]=GaussianFunction(N/2*dt,sigma,0);

	return;
}

#endif
