#ifndef ASU_MOVINGAVERAGE
#define ASU_MOVINGAVERAGE

#include<iostream>
#include<iterator>
#include<numeric>
#include<vector>

// #include<Convolve.hpp>

/*************************************************
 * This C++ template returns the moving averaged
 * results of input vector.
 *
 * input(s):
 * const T   PBegin  ----  Input array begin iterator.
 * const T   PEnd    ----  Input array end iterator.
 * const int &npts   ----  Number of pionts to average. (at edges, use 1~npts point averaging)
 *
 * output(s):
 * vector<double> ans  ----  Results.
 *
 * Shule Yu
 * Jun 23 2018
 *
 * Key words: moving average.
*************************************************/

template <typename T>
std::vector<double> MovingAverage(const T PBegin, const T PEnd, const int &npts){

    std::vector<double> ans;
    if (PBegin==PEnd || npts<=0) return ans;


//     auto res=Convolve(std::vector<double> (PBegin,PEnd),std::vector<double> (npts,1.0/npts),false,true);
//     res=Convolve(res,std::vector<double> (npts,1.0/npts),false,true);
//     return std::vector<double> (res.begin()+npts,res.begin()+npts+std::distance(PBegin,PEnd));

    ans.resize(std::distance(PBegin,PEnd));
    std::vector<double> S(ans.size());
    std::partial_sum(PBegin,PEnd,S.begin());

    for (std::size_t i=0;i<ans.size();++i){
        std::size_t B=std::max(0,(int)i-npts/2),E=std::min((int)ans.size()-1,(int)i+(npts-1)/2);
        ans[i]=(S[E]-S[B]+*next(PBegin,B))/(E-B+1);
    }

    return ans;
}

template <typename T>
std::vector<double> MovingAverage(const std::vector<T> &p, const int &npts){
    return MovingAverage(p.begin(),p.end(),npts);
}

#endif
