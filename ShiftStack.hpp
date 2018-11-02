#ifndef ASU_SHIFTSTACK
#define ASU_SHIFTSTACK

#include<iostream>
#include<iterator>
#include<vector>
#include<numeric>

#include<AvrStd.hpp>

/*********************************************************
 * This C++ template shift and stack the input signals.
 * Standard deviation is estimated, weighting is optional.
 *
 * input(s):
 * const vector<vecotr<T1>> &p  ----  2D input array.
 * const vecotr<int>        &s  ----  Amount of shift for each row.
 * const vecotr<T2>         &w  ----  Weighting for each row (value between 0~1).
 *
 * Negative s indicate this row is shifted to the left.
 * Positive s indicate this row is shifted to the right.
 * Negative w indicate flip the sign of p for this row.
 *
 * output(s):
 * pair<vector<double>,vector<double>> ans  ----  {stacked signal, std of the stack}
 *
 * Shule Yu
 * Dec 21 2017
 *
 * Key words: shift and stack, mean, standard deviation.
*********************************************************/

template <typename T1, typename T2=double>
std::pair<std::vector<double>,std::vector<double>> ShiftStack(const std::vector<std::pair<T1,T1>> &P, 
                                                              const std::vector<int> &s=std::vector<int>(),
                                                              const std::vector<T2> &w=std::vector<T2>()){
    int m=P.size();

    // Check VPBegin size.
    if (m==0) return {};

    // Check each trace length.
    int n=std::distance(P[0].first,P[0].second);
    for (const auto &item:P) { 
        if (std::distance(item.first,item.second)!=n) {
            std::cerr <<  "Error in " << __func__ << ": input 2D array size error ..." << std::endl;
            return {};
        }
    }

    // Check weight & shift length.
    if (!w.empty() && w.size()!=P.size()) {
        std::cerr <<  "Error in " << __func__ << ": input weight size error ..." << std::endl;
        return {};
    }

    if (!s.empty() && s.size()!=P.size()) {
        std::cerr <<  "Error in " << __func__ << ": input shift size error ..." << std::endl;
        return {};
    }

    // Check sum weight.
    auto f=[](const T2 &sum, const T2 &w1){return sum+fabs(w1);};
    if (!w.empty() && std::accumulate(w.begin(),w.end(),0.0,f)==0) return {};


    // Calculate shift stack and std.

    std::vector<double> Stack(n,0),STD(n,0),Tmp(m,0);

    for (int i=0;i<n;++i){

        for (int j=0;j<m;++j){

            int Shift=(s.empty()?0:s[j]%n);
            int X=i-Shift;
            Tmp[j]=((X<0 || X>n)?0:*std::next(P[j].first,X));
        }

        auto res=AvrStd(Tmp,w);
        Stack[i]=res.first;
        STD[i]=res.second;

    }

    return {Stack,STD};
}

template <typename T1, typename T2=double>
std::pair<std::vector<double>,std::vector<double>> ShiftStack(const std::vector<std::vector<T1>> &p, const std::vector<int> &s=std::vector<int>(), const std::vector<T2> &w=std::vector<T2>()){

    // Call ShiftStack.
    typedef typename std::vector<T1>::const_iterator DataIt;
    std::vector<std::pair<DataIt,DataIt>> Tmp;
    for (const auto &item:p) Tmp.push_back({item.begin(),item.end()});
    return ShiftStack(Tmp,s,w);
}

#endif
