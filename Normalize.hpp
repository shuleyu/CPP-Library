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
 * Normalize the input within a certain window, return
 * the amplitude within the window.
 *
 *
 * vector<T> &p            ----  Array of datatype T.
 * const pair<int,int> &W  ----  Window(Optional)
 * 	                             W.first: window begin.
 * 	                             W.second: window length.
 *
 * Shule Yu
 * Dec 29 2017
 *
 * Note: Original data destroyed.
 *       Will adjust the window to fit the data, gives a
 *       warning.
 *
 * Key words: normalize.
***********************************************************/

template<class T>
T Normalize(std::vector<T> &p, std::pair<int,int> W={0,std::numeric_limits<int>::max()}){

	// Check array size.
	if (p.empty()) {
		std::cerr <<  __func__ << "; Error: input array size is zero ..." << std::endl; 
		return 0;
	}

	int n=p.size();

	// Check window value.
	if (W.second<=0) {
		std::cerr <<  __func__ << "; Error: window length <= 0 ..." << std::endl; 
		return 0;
	}

	if (W.first>=n || W.first+W.second<=0) {
		std::cerr <<  __func__ << "; Error: window isn't covering the input array..." << std::endl; 
		return 0;
	}

	if (W.first<0) {
		W.first=0;
		std::cerr <<  __func__ << "; Warning: window start is set to 0..." << std::endl; 
	}


	if (W.first+W.second>n) {
		if (W.second!=std::numeric_limits<int>::max())
			std::cerr <<  __func__ << "; Warning: window end is set to p.end() ..." << std::endl; 
		W.second=n-W.first;
	}

    // Find AMP.
    T AMP=Amplitude(std::vector<T>(p.begin()+W.first,p.begin()+W.first+W.second)).first;
    if (AMP==0.0){
		std::cerr <<  __func__ << "; Warning: input array is zeros ..." << std::endl; 
        return 0;
    }

    // Normalize array.
    for (auto &item:p) item/=AMP;

    return AMP;
}

#endif
