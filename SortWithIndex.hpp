#ifndef ASU_SORTWITHINDEX
#define ASU_SORTWITHINDEX

#include<vector>
#include<algorithm>

#include<ASU_tools.hpp>

/*************************************************
 * This C++ template takes the same input as sort
 * in the standard library. Returns the index of
 * sorted elements of their original position.
 *
 * Note: must pass cmp. (Can't have default?)
 *
 * vector<T>::iterator Begin  ----  Sort begin position.
 * vector<T>::iterator End    ----  Sort end position.
 * F cmp                      ----  lambda for comparison.
 *
 * Shule Yu
 * Dec 28 2017
 *
 * Key words: sort, index
*************************************************/

template <class T, class F>
std::vector<size_t> SortWithIndex(T Begin,T End,F cmp) {

	// Initialize original index locations.
	std::vector<size_t> idx;
	size_t index=0;
	for (auto it=Begin;it!=End;++it) idx.push_back(index++);

	// Sort indexes based on comparing values.
	auto cmp2=[&cmp,&Begin](const size_t &i1, const size_t &i2) {
		return cmp(*(Begin+i1),*(Begin+i2));
	};
	sort(idx.begin(),idx.end(),cmp2);

	// Sort array.
	sort(Begin,End,cmp);

	return idx;
}

#endif
