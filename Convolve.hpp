#ifndef ASU_CONVOLVE
#define ASU_CONVOLVE

#include<iostream>
#include<vector>

#include<ASU_tools.hpp>

/***********************************************************
 * This C++ template returns convolution result of two input
 * array.
 *
 * vector<T1> &x  ----  Array x.
 * vector<T2> &y  ----  Array y.
 *
 * Shule Yu
 * Dec 29 2017
 *
 * Key words: convolution.
***********************************************************/

template<class T1, class T2>
std::vector<double> Convolve(const std::vector<T1> &x, const std::vector<T2> &y){

    if (x.empty() || y.empty()){
		std::cerr <<  __func__ << "; Error: input array size is zero ..." << std::endl; 
		return {};
    }

	int m=x.size(),n=y.size();

	std::vector<double> ans;
    for (int i=0;i<m+n-1;++i){
		ans.push_back(0);
        int Begin=std::max(i-n+1,0);
        int End=std::min(i+1,m);
        for (int j=Begin;j<End;++j) ans.back()+=x[j]*y[i-j];
    }

    return ans;
}

#endif
