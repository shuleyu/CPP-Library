#ifndef ASU_CIRCLEBIN
#define ASU_CIRCLEBIN

#include<iostream>
#include<vector>

#include<GcpDistance.hpp>

/*********************************************************
 * This C++ template compute circle geographic binning
 * results. Update the bin center to the average position.
 *
 * input(s):
 * const vector<pair<T1,T2>> &p    ----  Sampling lon/lat.
 * const vector<pair<T3,T4>> &b    ----  Bin center lon/lat.
 * const vector<T5>          &r    ----  Bin radius (in deg).
 *
 * return(s):
 * pair<vector<pair<double,double>>,vector<vector<bool>>> ans  ----  make_pair(B,ans)
 *                           B  : Updated bin centers. (b.size())
 *                           ans: Bin result matrix. (p.size()*b.size())
 *
 * Shule Yu
 * Dec 26 2017
 *
 * Note: original bin center locations destroyed.
 * Key words: geographic bin, circle, update.
***********************************************************/

template <typename T1, typename T2, typename T3, typename T4, typename T5>
std::pair<std::vector<std::pair<double,double>>,std::vector<std::vector<bool>>> CircleBin(
    const std::vector<std::pair<T1,T2>> &p,
    const std::vector<std::pair<T3,T4>> &b,
    const std::vector<T5> &r){

    // Check array size.
    if (b.size()!=r.size()) {
        std::cerr <<  "Error in " << __func__ << ": bins and radius size don't match ..." << std::endl;
        return {};
    }

    // Prepare output.
    std::size_t Np=p.size(),Nb=b.size();
    std::vector<std::pair<double,double>> B(Nb);
    std::vector<std::vector<bool>> ans(Np,std::vector<bool>(Nb,false));

    // For each bin, search which points are inside them.
    // Then update the bin center.
    for (std::size_t i=0;i<Nb;++i){

        T2 NewLon=0,NewLat=0;
        int Cnt=0;

        for (std::size_t j=0;j<Np;++j){
            if (GcpDistance(b[i].first,b[i].second,p[j].first,p[j].second)<=r[i]){
                ans[j][i]=true;
                NewLon+=p[j].first;
                NewLat+=p[j].second;
                ++Cnt;
            }
        }
        B[i].first=NewLon/Cnt;
        B[i].second=NewLat/Cnt;
    }

    return {B,ans};
}

#endif
