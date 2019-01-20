#ifndef ASU_ENUMERATE
#define ASU_ENUMERATE

#include<iostream>
#include<vector>

/*****************************************************************
 * This C++ template returns the enumerate result of input vector.
 *
 * input(s):
 * const vector<std::size_t> &p  ----  a vector indicate the strict upperbound of each bit.
 *
 * return(s):
 * vector<vector<size_t>> ans    ----  Each row is an enumerate result.
 *
 * Shule Yu
 * Jan 20 2019
 *
 * Key words: enumeration.
*****************************************************************/

std::vector<std::vector<std::size_t>> Enumerate(const std::vector<std::size_t> &p){

    std::vector<std::vector<std::size_t>> ans;

    int n=p.size();
    if (n==0) return ans;
    for (const auto &item:p) if (item==0) return ans;
    
    std::vector<std::size_t> cur(n,0);

    int i=n;
    while (i>=0){

        if (i==n) {
            ans.push_back(cur);
            --i;
        }
        else {
            ++cur[i];
            if (cur[i]==p[i]){
                cur[i]=0;
                --i;
            }
            else i=n;
        }
    }

    return ans;
}

template<typename T>
std::vector<std::vector<T>> Enumerate(const std::vector<std::vector<T>> &p){

    std::vector<std::vector<T>> ans;

    int n=p.size();
    if (n==0) return ans;
    for (const auto &item:p) if (item.empty()) return ans;
    
    std::vector<T> cur(n);
    std::vector<std::size_t> Cnt(n,0);
    for (int i=0;i<n;++i) cur[i]=p[i][0];

    int i=n;
    while (i>=0){

        if (i==n) {
            ans.push_back(cur);
            --i;
        }
        else {
            ++Cnt[i];
            if (Cnt[i]==p[i].size()){
                Cnt[i]=0;
                cur[i]=p[i][0];
                --i;
            }
            else {
                cur[i]=p[i][Cnt[i]];
                i=n;
            }
        }
    }

    return ans;
}
#endif
