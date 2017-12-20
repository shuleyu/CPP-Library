#ifndef ASU_AVRSTD_HPP
#define ASU_AVRSTD_HPP

#include<iostream>
#include<cmath>
#include<vector>
#include<numeric>

#include<ASU_tools.hpp>

/***********************************************************
 * This C function returns mean and unbiased estimation of
 * standard deviation for input array. The std formula is:
 *
 * sigma=sqrt{ sum( (x-avr(x))^2 ) ) /(NPTS-1) }
 *
 * vector<T1> V  ----  Array of data of T1 type.
 * vector<T2> W  ----  Array of weight of T2 type.
 *                     we define negative weight as taking
 *                     the negative value of V beforehand.
 *
 *
 * Shule Yu
 * Dec 19 2017
 *
 * Note: when npts<=1, we return 0 as estimation of sigma.
 *
 * Key words: standard deviation, unbiased, weighted.
 *
 * Reference:
 *     http://en.wikipedia.org/wiki/Unbiased_estimation_of_standard_deviation
 *     http://en.wikipedia.org/wiki/Standard_error
***********************************************************/

template <class T1,class T2=double>
std::pair<double,double> AvrStd(const std::vector<T1> &V,const std::vector<T2> &W=std::vector<T2>()){

	if (W.empty()){ // no weighting.

		if (V.empty()) return {};
		if (V.size()==1) return {V[0],0};

		// Calculate x average.
		double avr=accumulate(V.begin(),V.end(),0.0)/V.size();

		// Calculate summation.
		double SUM=0;
		for (auto item:V) SUM+=pow((item-avr),2);

		// Return average and standard deviation.
		return {avr,sqrt(SUM/(V.size()-1))};
	}
	else { // with weighting.

		if (V.size()!=W.size()) {
			std::cerr <<  __func__ << ": data array and weight array length doesn't match ..." << std::endl;
			return {};
		}

		// Count number of weight!=0.
		// Calculate x average.
		// Sum weights.
		int N=0;
		double SumW=0,avr=0;
		for (size_t i=0;i<V.size();++i){
			SumW+=fabs(W[i]);
			avr+=V[i]*W[i];
			if (W[i]!=0) ++N;
		}
		avr/=SumW;

		if (N==0) return {};
		if (N==1) return {avr,0};

		// Calculate summation.
		double SUM=0;
		for (size_t i=0;i<V.size();++i)
			SUM+=fabs(W[i])*pow((W[i]>0?1:-1)*V[i]-avr,2);

		// Return standard deviation.
		return {avr,sqrt(SUM*N/(N-1)/SumW)};
	}
}

#endif
