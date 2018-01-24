#ifndef ASU_SNR
#define ASU_SNR

#include<iostream>
#include<vector>

#include<Envelope.hpp>
#include<SimpsonRule.hpp>

/***********************************************************
 * This C++ template evaluate the signal to noise ratio of
 * input signal (S).
 *
 * 1. Calculate envelope sqrt(S^2+H^2). H is the Hilbert transform of S.
 * 2. Integrate between singal window and noise window.
 * 3. Calculate the ratio and averaged by window lengths.
 *
 * const vector<T> &p     ----  Input array pointer.
 * const int       &nloc  ----  Noise window start position.
 * const int       &nlen  ----  Noise window length.
 * const int       &sloc  ----  Signal window start position.
 * const int       &slen  ----  Signal window length.
 *
 * Shule Yu
 * Jan 20 2018
 *
 * Key words: signal to noise ratio, SNR
***********************************************************/

template<class T>
double SNR(const std::vector<T> &p,int nloc,int nlen,int sloc,int slen){

    int npts=p.size();

	if (nloc<0 || nloc>npts || nlen<0 || nloc+nlen>npts){
        std::cerr <<  __func__ << "; Error: Noise window error ..." << std::endl;
		return 0.0;
	}
	if (sloc<0 || sloc>npts || slen<0 || sloc+slen>npts){
        std::cerr <<  __func__ << "; Error: Signal window error ..." << std::endl;
		return 0.0;
	}

    auto res=Envelope(p);

    // integrate envelope using Simpson's rule. (ignore h/3 term).
    double Slevel=SimpsonRule(std::vector<T> (res.begin()+sloc,res.begin()+sloc+slen));
    double Nlevel=SimpsonRule(std::vector<T> (res.begin()+nloc,res.begin()+nloc+nlen));

    // average with respect to their length.
    return Slevel/Nlevel*(1.0*nlen/slen);
}

#endif
