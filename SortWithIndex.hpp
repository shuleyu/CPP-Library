#ifndef ASU_SORTWITHINDEX
#define ASU_SORTWITHINDEX

#include<iterator>
#include<vector>
#include<algorithm>

#include<ReorderUseIndex.hpp>

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
 * const bool &ActualSortIt    ----  default: true. If false, input is not sorted.
 *
 * return(s):
 * vector<std::size_t> ans  ----  Original index for each element in the sorted array.
 *                                Original[ans[i]]=Sorted[i].
 *
 * Shule Yu
 * Dec 28 2017
 *
 * Key words: sort, index
*************************************************/

template <typename T1, typename T2=std::less<typename std::iterator_traits<T1>::value_type> >
std::vector<std::size_t> SortWithIndex(T1 Begin, T1 End, T2 cmp=T2(), const bool &ActualSortIt=true) {

    // Initialize original index locations.
    std::vector<std::size_t> idx;
    std::size_t index=0;
    for (auto it=Begin;it!=End;++it) idx.push_back(index++);

    // Sort indexes based on comparing values.
    auto cmp2=[&cmp,&Begin](const std::size_t &i1, const std::size_t &i2) {
        return cmp(*(Begin+i1),*(Begin+i2));
    };
    sort(idx.begin(),idx.end(),cmp2);

    // Actually sort the input array.
    if (ActualSortIt) ReorderUseIndex(Begin,End,idx);

    return idx;
}

template <typename T>
std::vector<std::size_t> SortWithIndex(T Begin, T End, const bool &ActualSortIt) {
    return SortWithIndex(Begin, End, std::less<typename std::iterator_traits<T>::value_type> (), ActualSortIt);
}

#endif
