#ifndef ASU_CREATEGRID
#define ASU_CREATEGRID

#include<iostream>
#include<vector>
#include<cmath>
#include<algorithm>

/***********************************************************
 * This C++ template returns the 1D grid meshing results.
 *
 * input(s):
 * const double &lowerbound   ----  Grid lower bound.
 * const double &upperbound   ----  Grid upper bound.
 * const double &Para         ----  meaning-changing parameter.
 *                                  it's meaning depends on which mode:
 *
 * const int    &mode         ----  select mode.
 *                                  Define the meaning of Para and return vector.
 *
 *                                  0: Para means number of gird points.
 *                                     Return a vector (ans) of size Para:
 *
 *                                     ans.size()=Para;
 *                                     ans[0]=lowerbound;
 *                                     ans[Para-1]=upperbound;
 *
 *                                  1: Para means grid increment.
 *                                     Last grid point is less equal to higherbound.
 *                                     Return a vector (ans) of calculated size:
 *
 *                                     ans[0]=lowerbound;
 *                                     ans[1]-ans[0]=Para;
 *                                     upperbound-Para<ans.back();
 *                                     ans.back()<=upperbound;
 *
 *                                 -1: Same as 1. Will only return the grid property:
 *
 *                                     ans.size()=2;
 *                                     ans[0] = calculated grid size.
 *                                     ans[1] = adjusted upperbound.
 *
 *
 *                                  2: Para means an estimation of grid increment.
 *                                     The calculated grid increment (Para*) is (possibly)
 *                                     sightly adjusted such that the higherbound is meet.
 *                                     Return a vector (ans) of calculated size:
 *
 *                                     ans[0]=lowerbound;
 *                                     ans[1]-ans[0]=Para* (<=Para);
 *                                     ans.back()=upperbound;
 *
 *                                 -2: Same as 2. Will only return the grid property:
 *
 *                                     ans.size()=2;
 *                                     ans[0] = calculated grid size.
 *                                     ans[1] = adjusted Para (Para*).
 *
 * return(s):
 * vector<double> ans  ----  Created grid or grid properties, depending on mode.
 *
 * Shule Yu
 * Jan 23 2018
 *
 * Key words: creating grid.
***********************************************************/

std::vector<double> CreateGrid(const double &lowerbound, const double &upperbound,
                               const double &Para, const int mode){

    double upperBound=upperbound,lowerBound=lowerbound;
    bool reverseAns=false;


    // check lower and upper bound.
    if (upperBound<lowerBound) {
        std::swap(upperBound,lowerBound);
        reverseAns=true;
    }

    if (mode==0){

        int N=Para;
        if (N<=0) throw std::runtime_error("NPTS < 0 ...\n");

        double Inc=1.0*(upperBound-lowerBound)/(N-1);

        std::vector<double> ans(N,lowerBound);
        for (int i=1;i<N;++i) ans[i]=ans[i-1]+Inc;
        if (reverseAns) std::reverse(ans.begin(),ans.end());
        return ans;
    }
    if (mode==1 || mode==-1){

        double Inc=fabs(Para);

        if (mode==-1) {
            double N=1+floor(1.0*(upperBound-lowerBound)/Inc);
            if (fabs((upperBound-lowerBound-N*Inc))<Inc*1e-6) // float number rounding error?
                N+=1;
            return {N,lowerBound+(N-1)*Inc};
        }

        std::vector<double> ans;

        double Cur=lowerBound;

        while (Cur<=upperBound || Cur-upperBound<Inc*1e-6) {
            ans.push_back(Cur);
            Cur+=Inc;
        }
        if (reverseAns) std::reverse(ans.begin(),ans.end());
        return ans;
    }
    if (mode==2 || mode==-2){

        double Inc=fabs(Para);

        int N=1+(int)round((upperBound-lowerBound)/Inc);
        Inc=1.0*(upperBound-lowerBound)/(N-1);

        if (mode==-2) {
            return {1.0*N,Inc};
        }

        std::vector<double> ans;
        double Cur=lowerBound;

        for (int i=0;i<N;++i) {
            ans.push_back(Cur);
            Cur+=Inc;
        }

        // Round-off errors eliminator!
        ans.back()=upperBound;

        if (reverseAns) std::reverse(ans.begin(),ans.end());
        return ans;
    }

    std::cerr <<  "Error in " << __func__ << ": mode error ..." << std::endl;
    return {};
}

#endif
