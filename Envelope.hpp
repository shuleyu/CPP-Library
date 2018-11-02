#ifndef ASU_ENVELOPE
#define ASU_ENVELOPE
// Need sci-libs/fftw

#include<vector>
#include<cmath>

#include<ShiftPhase.hpp>

/**************************************************************
 * This C++ template calculate the envelope of the input trace.
 *
 * First make a Hilbert transform of input: y=Hilbert(x).
 * Then calculate the envelope as: z=sqrt(x^2+y^2).
 *
 * input(s):
 * const vector<T> &x  ----  Input signal.
 *
 * return(s):
 * vector<double> ans  ----  Envelope signal.
 *
 * Shule Yu
 * Jan 20 2018
 *
 * Dependence: fftw-3.
 *
 * Key words : fast fourier transform, envelope, hilbert.
**************************************************************/

template<typename T>
std::vector<double> Envelope(const std::vector<T> &x){

    if (x.empty()) return {};

    // Find Hilbert.
    auto ans=ShiftPhase(x,-90);

    // Find Envelope.
    for (std::size_t i=0;i<x.size();++i)
        ans[i]=sqrt(ans[i]*ans[i]+x[i]*x[i]);

    return ans;
}

#endif
