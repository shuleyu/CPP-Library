#ifndef ASU_SHIFTSTACK
#define ASU_SHIFTSTACK

#include<iostream>
#include<vector>

#include<ASU_tools.hpp>

/*********************************************************
 * This CPP template shift and stack the input 2D array.
 * Standard deviation is estimated, weighting is optional.
 *
 * const vector<vecotr<T1>> &p  ----  2D input array.
 * const vecotr<int>        &s  ----  Amount of shift for each row.
 * const vecotr<T2>         &w  ----  Weight for each row.
 *
 *   Negative s indicate this row is shifted to the left.
 *   Positive s indicate this row is shifted to the right.
 *
 *   Negative w indicate flip the sign of p for this row.
 *
 * Shule Yu
 * Dec 21 2017
 *
 * Key words: shift and stack, mean, standard deviation.
*********************************************************/

template <class T1, class T2=double>
std::pair<std::vector<double>,std::vector<double>> ShiftStack(const std::vector<std::vector<T1>> &p, const std::vector<int> &s=std::vector<int>(), const std::vector<T2> &w=std::vector<T2>()){

	// Check p.
	if (p.empty()) return {};

	size_t N=p[0].size();
	for (auto item:p)
		if (item.size()!=N) {
			std::cerr <<  __func__ << "; Error: input 2D array size error ..." << std::endl;
			return {};
		}

	// Check weight & shift length.
	if (!w.empty() && w.size()!=p.size()) {
		std::cerr <<  __func__ << "; Error: input weight size error ..." << std::endl;
		return {};
	}

	if (!s.empty() && s.size()!=p.size()) {
		std::cerr <<  __func__ << "; Error: input shift size error ..." << std::endl;
		return {};
	}

	// Check sum weight.

	int m=p.size(),n=p[0].size();
	if (!w.empty()){
		double SumWeight=0;
		for (int i=0;i<m;++i)
			SumWeight+=(w[i]>0?w[i]:-w[i]);
		if (SumWeight==0) return {};
	}

    // Calculate shift stack and std.

	std::vector<double> Stack(n,0),STD(n,0),Tmp(m,0);

	for (int i=0;i<n;++i){

		for (int j=0;j<m;++j){

			int Shift=(s.empty()?0:s[j]%n);
			int X=i-Shift;
			Tmp[j]=((X<0 || X>n)?0:p[j][X]);
		}

		auto res=AvrStd(Tmp,w);
		Stack[i]=res.first;
		STD[i]=res.second;

	}

	return {Stack,STD};
}

#endif
