#ifndef ASU_DIFF
#define ASU_DIFF

#include<iostream>
#include<vector>

#include<ASU_tools.hpp>

/******************************************************
 * This C++ tempalte takes the difference from adjacent
 * elements from the input array.
 *
 * const vector<T1> &p  ----  Array.
 *
 * Shule Yu
 * Dec 29 2017
 *
 * Key words: derivative, diff
 *
 * Note: output size is p.size()-1.
***********************************************************/

template<class T>
std::vector<T> Diff(const std::vector<T> &p){

	std::vector<T> ans;

    for (size_t i=0;i+1<p.size();++i)
		ans.push_back(p[i+1]-p[i]);

    return ans;
}

#endif
