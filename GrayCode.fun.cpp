#include<iostream>
#include<vector>

/***************************************************
 * This C++ function returns gray code sequence for
 * certain number of bits.
 *
 * input(s):
 * const int &n  ----  Number of bits.
 *
 * return(s):
 * vector<vector<bool>> ans  ----  a 2D 0/1 array of gray code sequence.
 *
 * Shule Yu
 * Jan 10 2018
 *
 * Key words: gray code.
***************************************************/

std::vector<std::vector<bool>> GrayCode(const int &n){

    // Check array size.
    if (n<=0) {
        std::cerr <<  "Warning in " << __func__ << ": input size <= 0 ..." << std::endl;
        return {};
    }

    if (n==1) {
        std::vector<std::vector<bool>> ans{{false},{true}};
        return ans;
    }
    else {
        auto ans1=GrayCode(n-1);
        int N=ans1.size();

        std::vector<std::vector<bool>> ans(N,{false});
        ans.insert(ans.end(),N,{true});

        for (int i=0;i<N;++i) ans[i].insert(ans[i].end(),ans1[i].begin(),ans1[i].end());
        for (int i=N;i<2*N;++i) ans[i].insert(ans[i].end(),ans1[2*N-1-i].begin(),ans1[2*N-1-i].end());
        return ans;
    }
}
