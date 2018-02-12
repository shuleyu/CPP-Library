#ifndef ASU_NORMALIZE
#define ASU_NORMALIZE

#include<iostream>
#include<vector>
#include<limits>

#include<Amplitude.hpp>

/******************************************************
 * This C++ template returns the amplitude of an array,
 * Simutaneously, normalize the input array using the
 * amplitude (divide every value by the amplitude).
 *
 * If Window!={},
 * Return the amplitude within the window, normalize
 * the whole input using that amplitude.
 *
 * input(s):
 * vector<T> &p            ----  Array of datatype T.
 * const pair<int,int> &W  ----  (Optional) Window.
 *                               W.first: window begin.
 *                               W.second: window length.
 *
 * return(s):
 * vector<T> &p (in-place)
 * T ans  ----  Amplitude of the trace (within a window).
 *
 * Shule Yu
 * Dec 29 2017
 *
 * Note: Original data destroyed.
 *       If the given window exceed data limit, adjust the
 *       window to fit the data and gives a warning.
 *
 * Key words: normalize
***********************************************************/

template<class T>
T Normalize(std::vector<T> &p, std::pair<int,int> W={0,std::numeric_limits<int>::max()}){

    // Check array size.
    if (p.empty()) {
        std::cerr <<  "Error in " << __func__ << ": input array size is zero ..." << std::endl;
        return 0;
    }

    int n=p.size();

    // Check window value.
    if (W.second<=0) {
        std::cerr <<  "Error in " << __func__ << ": window length <= 0 ..." << std::endl;
        return 0;
    }

    if (W.first>=n || W.first+W.second<=0) {
        std::cerr <<  "Error in " << __func__ << ": window isn't covering the input array..." << std::endl;
        return 0;
    }

    if (W.first<0) {
        W.first=0;
        std::cerr <<  "Warning in " << __func__ << ": window start is set to 0..." << std::endl;
    }


    if (W.first+W.second>n) {
        if (W.second!=std::numeric_limits<int>::max())
        std::cerr <<  "Warning in " << __func__ << ": window end is set to p.end() ..." << std::endl;
        W.second=n-W.first;
    }

    // Find AMP.
    T AMP=Amplitude(std::vector<T>(p.begin()+W.first,p.begin()+W.first+W.second)).first;
    if (AMP==0.0){
        std::cerr <<  "Warning in " << __func__ << ": input array is zeros ..." << std::endl;
        return 0;
    }

    // Normalize array.
    for (auto &item:p) item/=AMP;

    return AMP;
}

#endif
