#ifndef ASU_BOOTSTRAP
#define ASU_BOOTSTRAP

#include<iostream>
#include<vector>
#include<random>
#include<chrono>

#include<ShiftStack.hpp>

/**********************************************************
 * This C++ template return the bootstrap stacking results
 * (bootstrap average and std) on an array of signals.
 *
 * input(s):
 * const vector<vector<T1>> &p            ----  2D input array.
 * const int                &BootN        ----  Re-roll times.
 * vector<vector<double>>   &RerollStack  ----  stacks for each re-roll.
 * const vecotr<T2>         &w            ----  (Optional) Weight for each signal.
 *
 * output(s):
 * vector<vector<double>>              &RerollStack (in-place)
 * pair<vector<double>,vector<double>> ans  ----  average and standard deviation of
 *                                                bootstarp stacks.
 *
 * Shule Yu
 * Dec 21 2017
 *
 * Note: Negative w indicate flip the sign of p for this row.
 *
 * Key words: bootstrap, mean, standard deviation.
**********************************************************/

template <class T1, class T2=double>
std::pair<std::vector<double>,std::vector<double>> Bootstrap(const std::vector<std::vector<T1>> &p,const int &BootN,
                                                             std::vector<std::vector<double>> &RerollStack, const std::vector<T2> &w=std::vector<T2>()){

    // Check p size.
	if (p.empty()) return {};

    size_t N=p[0].size();
    for (auto &item:p)
        if (item.size()!=N) {
            std::cerr <<  "Error in " << __func__ << ": input 2D array size error ..." << std::endl;
            return {};
        }

    // Check weight size.
    if (!w.empty() && w.size()!=p.size()) {
        std::cerr <<  "Error in " << __func__ << ": input weight size error ..." << std::endl;
        return {};
    }

    // random seed.
    auto seed=std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine e(seed);

    // Prepare bootstrap details.
    RerollStack.clear();

    int Cnt=0,m=p.size(),n=p[0].size();
    std::uniform_int_distribution<int> u(0,m-1);
    std::vector<int> R(m);
    std::vector<double> W(m);
    std::vector<double> SumWeight(BootN,m);
    while (Cnt<BootN){


        // re-roll m traces.
        for (auto &item: R) item=u(e);
        for (auto &item: W) item=0;


        // Make re-rolled weight array.
        for (auto &item: R)
            W[item]+=(w.empty()?1:w[item]);


        // Check sum weight after each re-roll.
        if (!w.empty()){
            SumWeight[Cnt]=0;
            for (int i=0;i<m;++i)
                SumWeight[Cnt]+=(W[i]>0?W[i]:-W[i]);
            if (SumWeight[Cnt]==0) continue;
        }

        // Calculate re-roll (weighted) average.
        std::vector<double> Tmp;
        for (int i=0;i<n;++i){
            double Sum=0;
            for (int j=0;j<m;++j) Sum+=W[j]*p[j][i];
            Tmp.push_back(Sum/SumWeight[Cnt]);
        }
        RerollStack.push_back(Tmp);
        SumWeight[Cnt]/=m;

        ++Cnt;
    }

    // Calculate bootstrap mean and std.

    return ShiftStack(RerollStack,{},SumWeight);
}

#endif
