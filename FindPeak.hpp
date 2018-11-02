#ifndef ASU_FINDPEAK
#define ASU_FINDPEAK

#include<iostream>
#include<vector>

/**************************************************************
 * This C++ template find the max(abs(amp)) position within the
 * given window for a given input array.
 *
 * input(s):
 * const vector<T> &p     ----  Input array.
 * const int       &Peak  ----  rough peak position.
 * const int       &WB    ----  Window begin, relative to Input Peak.
 * const int       &WL    ----  Window length.
 *
 * return(s):
 * std::size_t ans  ----  correct peak position.
 *
 *
 * Shule Yu
 * Dec 31 2017
 *
 * Key words: peak position
**************************************************************/

template <typename T>
std::size_t FindPeak(const std::vector<T> &p, const int &Peak, const int &WB, const int &WL){

    int n=p.size();

    // Check window.
    if (n<=0 || WL<=0 || Peak+WB<0 || Peak+WB>=n || Peak+WB+WL-1>=n){
        std::cerr <<  "Error in " << __func__ << ": input window exceed array size ..." << std::endl;
        return -1;
    }

    std::size_t ans=Peak;
    T MaxAmp=p[Peak]>0?p[Peak]:-p[Peak],Amp;
    for (int i=Peak+WB;i<Peak+WB+WL;++i){
        Amp=p[i]>0?p[i]:-p[i];
        if (MaxAmp<Amp){
            MaxAmp=Amp;
            ans=i;
        }
    }

    return ans;
}

#endif
