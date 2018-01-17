#ifndef ASU_FINDPEAK
#define ASU_FINDPEAK

#include<iostream>
#include<vector>

/**************************************************************
 * This C++ template find the max(abs(amp)) position with the
 * given window for a given input array.
 *
 * const vector<T> &p     ----  Input array.
 * int             &Peak  ----  Input : rough peak position.
 *                              Output: correct peak position.
 * const int       &WB    ----  Window begin, relative to Input Peak.
 * const int       &WL    ----  Window length.
 *
 * Shule Yu
 * Dec 31 2017
 *
 * Key words: peak position
***********************************************************/

template <class T>
void FindPeak(const std::vector<T> &p, int &Peak, const int &WB, const int &WL){

	int n=p.size();

	// Check window.
    if (n<=0 || WL<=0 || Peak+WB<0 || Peak+WB>=n || Peak+WB+WL-1>=n){
		std::cerr <<  __func__ << "; Error: input window exceed array size ..." << std::endl; 
        return;
    }

	T MaxAmp=p[Peak+WB];
	T Amp;
    for (int i=Peak+WB+1;i<Peak+WB+WL;++i){
		Amp=p[i]>0?p[i]:-p[i];
        if (MaxAmp<Amp){
			MaxAmp=Amp;
			Peak=i;
        }
    }

    return;
}

#endif
