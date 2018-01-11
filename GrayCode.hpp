#ifndef ASU_GRAYCODE
#define ASU_GRAYCODE

#include<iostream>
#include<vector>

#include<ASU_tools.hpp>

/***************************************************
 * This C++ function returns gray code sequence for
 * certain number of bits.
 *
 * const int &n  ----  Number of bits.
 *
 * Shule Yu
 * Jan 10 2018
 *
 * Key words: gray code.
***************************************************/

std::vector<std::vector<bool>> GrayCode(const int &n){

	// Check array size.
	if (n<=0) {
		std::cerr <<  __func__ << "; Warning: input size <= 0 ..." << std::endl;
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

#endif
