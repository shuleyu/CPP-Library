#ifndef ASU_SORTWITHINDEX
#define ASU_SORTWITHINDEX

#include<vector>
#include<algorithm>

/*************************************************
 * This C++ template takes the same input as sort
 * in the standard library. Returns the index of
 * sorted elements of their original position.
 *
 * Note: must pass cmp. (C++ Can't have default callables?)
 *       original array is sorted in-place.
 *
 * intput(s):
 * vector<T1>::iterator Begin  ----  Sort begin position.
 * vector<T1>::iterator End    ----  Sort end position.
 * T2 cmp                      ----  callable for comparison.
 *
 * return(s):
 * vector<size_t> ans  ----  Original index for each element in the sorted array.
 *                           Original[ans[i]]=Sorted[i].
 *
 * Shule Yu
 * Dec 28 2017
 *
 * Key words: sort, index
*************************************************/

template <class T1, class T2>
std::vector<size_t> SortWithIndex(T1 Begin,T1 End,T2 cmp) {

    // Initialize original index locations.
    std::vector<size_t> idx;
    size_t index=0;
    for (auto it=Begin;it!=End;++it) idx.push_back(index++);

    // Sort indexes based on comparing values.
    auto cmp2=[&cmp,&Begin](const size_t &i1, const size_t &i2) {
        return cmp(*(Begin+i1),*(Begin+i2));
    };
    sort(idx.begin(),idx.end(),cmp2);

    // Actually sort the input array.
    sort(Begin,End,cmp);

    return idx;
}

#endif
