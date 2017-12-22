#ifndef ASU_AMPLITUDE
#define ASU_AMPLITUDE

#include<iostream>
#include<vector>

#include<ASU_tools.hpp>

/*************************************************
 * This C++ template returns the amplitude and the 
 * positions of that amplitude.
 *
 * vector<T> p  ----  Array of datatype T.
 *
 * Shule Yu
 * Dec 19 2017
 *
 * Key words: amplitude
*************************************************/

template <class T>
std::pair<T,std::vector<size_t>> Amplitude(const std::vector<T> &p){

	if (p.size()==0) {
		std::cerr <<  __func__ << ": input array length is zero ..." << std::endl; 
		return {};
	}


	T amp=p[0]>0?p[0]:-p[0];
	std::pair<T,std::vector<size_t>> ans{amp,{0}};

	for (size_t i=1;i<p.size();++i){
		amp=p[i]>0?p[i]:-p[i];

		if (ans.first<amp) {
			ans.first=amp;
			ans.second={i};
		}
		else if (ans.first==amp)
			ans.second.push_back(i); 
	}

	return ans;
}

#endif
