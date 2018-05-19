#ifndef ASU_GAUSSIANBLUR
#define ASU_GAUSSIANBLUR

#include<iostream>
#include<vector>
#include<cmath>

#include<Convolve.hpp>
#include<GaussianSignal.hpp>

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
 * inputs(s):
 * vector<vector<T1>> &p      ----  Input 2D array pointer.
 * const T2           &dt     ----  Sampling rate (in sec.)
 * const T3           &sigma  ----  Gaussian function parameter (in sec.)
 *
 * return(s):
 * vector<vector<T1>> &p (in-place)
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
            std::cerr <<  "Error in " << __func__ << ": input 2D array size error ..." << std::endl;
            return;
        }

    // Check parameters.
    if (sigma<=0) {
        std::cerr <<  "Warning in " << __func__ << ": input sigma <= 0, signal unchanged ..." << std::endl;
        return;
    }

    // Gaussian signal length.
    double GaussianLength=std::max(50.0,2*sigma*sqrt(2*log(1000)));
    int GaussianN=1+(int)(GaussianLength/dt);

    // Set up the gaussian signal.
    std::vector<double> Gaussian=GaussianSignal(GaussianN,dt,sigma);

    // Convolve input signals with guassian function.
    // Truncate the smoothed signals into original size (keep the center part.)
    for (auto &item:p)
        item=Convolve(item,Gaussian,true,true);

    return ;
}

#endif
