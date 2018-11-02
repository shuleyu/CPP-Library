#ifndef ASU_DIFF
#define ASU_DIFF

#include<vector>

/******************************************************
 * This C++ tempalte takes the difference from adjacent
 * elements of the input array.
 *
 * input(s):
 * const vector<T> &p  ----  Array.
 *
 * return(s):
 * vector<T> ans  ----  The "Differnce" array.
 *
 * Shule Yu
 * Dec 29 2017
 *
 * Key words: derivative, difference.
 *
 * Note: output size is p.size()-1.
******************************************************/

template<typename T>
inline std::vector<T> Diff(const std::vector<T> &p){

    std::vector<T> ans;

    for (std::size_t i=0;i+1<p.size();++i)
        ans.push_back(p[i+1]-p[i]);

    return ans;
}

#endif
