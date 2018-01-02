#ifndef ASU_GAUSSIANBLUR
#define ASU_GAUSSIANBLUR

#include<iostream>
#include<cmath>
#include<vector>

#include<ASU_tools.hpp>

/***********************************************************
 * This C++ template blur input signals with given gaussian
 * function parameter (sigma, bassically).
 *
 * The length of the gaussian signal is at least 50 sec.
 *
 * If sigma is very large, the length of the gaussian signal
 * is chosen so that at the end points the amplitude is 0.1%
 * of the amplitude at the peak.
 *
 * vector<vector<T1>> &p      ----  Input 2D array pointer.
 * const T2           &dt     ----  Sampling rate (in sec.)
 * const T3           &sigma  ----  Gaussian function parameter.
 *
 * Shule Yu
 * Jan 01 2018
 *
 * Key words: gaussian, blur, low pass filter.
***********************************************************/

template<class T1, class T2, class T3>
void GaussianBlur(std::vector<std::vector<T1>> &p, const T2 &dt, const T3 &sigma){

	// Check p size.
	if (p.empty()) return;

	size_t N=p[0].size();
	if (N==0) return;
	for (auto &item:p)
		if (item.size()!=N) {
			std::cerr <<  __func__ << "; Error: input 2D array size error ..." << std::endl;
			return;
		}

	// Check parameters.
	if (sigma<=0) {
		std::cerr <<  __func__ << "; Warning: input sigma <= 0, signal unchanged ..." << std::endl;
		return;
	}

	// Gaussian signal length.
	double GaussianLength=std::max(50.0,2*sigma*sqrt(2*log(1000)));
    int GaussianN=1+(int)(GaussianLength/dt);

	// Set up the gaussian signal.
	std::vector<T1> Gaussian;
	GaussianSignal(Gaussian,GaussianN,dt,sigma);

    // Convolve input signals with guassian function.
    // Truncate the smoothed signals into original size (keep the center part.)
	for (auto &item:p) {
		auto res=Convolve(item,Gaussian,1);
		for (size_t i=0;i<item.size();++i) item[i]=res[i];
	}

    return ;
}

#endif
