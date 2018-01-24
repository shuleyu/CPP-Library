#ifndef ASU_UNIONINTERVALS
#define ASU_UNIONINTERVALS

#include<vector>
#include<map>

/***********************************************************
 * This C++ template returns the union result of input close
 * intervals.
 *
 * vector<pair<T,T>> &x  ----  Intervals.
 *
 * return:
 * vector<pair<T,T>> &y  ----  Union results.
 *
 *
 * Shule Yu
 * Jan 21 2018
 *
 * Key words: union, intervals
***********************************************************/

template<class T>
std::vector<std::pair<T,T>> UnionIntervals(const std::vector<std::pair<T,T>> &x){

    std::vector<std::pair<T,T>> ans;

    if (x.empty()) return ans;

    std::map<T,int> M;

    for (auto &item:x) {
        ++M[item.first];
        --M[item.second];
    }

    int Cur=0,Prev=0;
    std::pair<T,T> p;
    for (auto &item:M) {
        Cur+=item.second;
        if (Prev==0 && Cur!=0)
            p.first=item.first;
        else if (Prev!=0 && Cur==0){
            p.second=item.first;
            ans.push_back(p);
        }
        Prev=Cur;
    }
    return ans;
}

#endif
