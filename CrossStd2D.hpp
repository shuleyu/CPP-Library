#ifndef ASU_CROSSSTD2D
#define ASU_CROSSSTD2D

#include<iostream>
#include<vector>
#include<limits>

#include<AvrStd.hpp>

/**************************************************************
 * This C++ template has the same concept as "CrossStd". The
 * usage is extended to 2D to be used in image pattern finding.
 *
 * input(s):
 * const vector<vector<T1>> &s1  ----  2D signal 1.
 * const vector<vector<T2>> &s2  ----  2D signal 2.
 *
 * return(s):
 * pair<pair<int,int>,double> ans  ----  {position of best match({x,y}), standard deviation of the residual}
 *
 * Shule Yu
 * Jan 18 2018
 *
 * Key words: modified cross-correlation, moving standard deviaiton
****************************************************************/

template<class T1, class T2>
std::pair<std::pair<int,int>,double> CrossStd2D(const std::vector<std::vector<T1>> &s1, const std::vector<std::vector<T2>> &s2){

    int m1=s1.size(),m2=s2.size();

    // If signals are too short...
    if (m1<=1 || m2<=1){
        std::cerr <<  "Error in " << __func__ << ": Input sequences length error ..." << std::endl;
        return {};
    }

    int n1=s1[0].size(),n2=s2[0].size();

    if (n1<=1 || n2<=1){
        std::cerr <<  "Error in " << __func__ << ": Input sequences length error ..." << std::endl;
        return {};
    }

    // Finding s2 in s1, therefore s1 size should be larger than s2 size.
    if (m1<m2 || n1<n2){
        std::cerr <<  "Error in " << __func__ << ": s2 size larger than s1 size ..." << std::endl;
        return {};
    }

    // Shifting and evaluating STD at every shift pair.
    double STD=std::numeric_limits<double>::max();
    int sm=0,sn=0;
    std::vector<double> A(m2*n2,0);

    for (int di=0;di<m1-m2+1;++di){

        for (int dj=0;dj<n1-n2+1;++dj){

            // produce the residual signal.
            for (int i=0;i<m2;++i)
                for (int j=0;j<n2;++j)
                    A[n2*i+j]=s1[i+di][j+dj]-s2[i][j];

            // calculate std.
            auto res=AvrStd(A);

            if (STD>res.second){
                STD=res.second;
                sm=di;
                sn=dj;
            }
        }
    }
    return {{sm,sn},STD};
}

#endif
