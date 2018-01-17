#ifndef ASU_AVRSTD
#define ASU_AVRSTD

#include<iostream>
#include<cmath>
#include<vector>
#include<numeric>

/***********************************************************
 * This C++ template returns mean and unbiased estimation of
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
 * Note: when there's not enough record, we return 0 as
 *       estimation of sigma and gives a warning.
 *
 * Key words: standard deviation, unbiased, weighted.
 *
 * Reference:
 * 	   Use SPSS approach for weighted standard deviation:
 *     http://www.analyticalgroup.com/download/WEIGHTED_MEAN.pdf
***********************************************************/

template <class T1,class T2=double>
std::pair<double,double> AvrStd(const std::vector<T1> &V,const std::vector<T2> &W=std::vector<T2>()){

	// Check weight size.
	if (!W.empty() && V.size()!=W.size()) {
		std::cerr <<  __func__ << "; Error: data and weight size don't match ..." << std::endl;
		return {};
	}

	// Calculate (weighted) mean.
	double SumW=(W.empty()?V.size():0),avr=0;
	if (W.empty()) {
		for (size_t i=0;i<V.size();++i){
			avr+=V[i];
		}
	}
	else {
		for (size_t i=0;i<V.size();++i){
			SumW+=(W[i]>0?W[i]:-W[i]);
			avr+=W[i]*V[i];
		}
	}

	// Check weight sum.
	if (SumW<=0) {
		std::cerr << __func__ << "; Error: weight sum <= 0 ..." << std::endl;
		return {};
	}

	avr/=SumW;

	if (SumW<=1) {
		std::cerr << __func__ << "; Warning: weight sum <= 1 ..." << std::endl;
		return {avr,0};
	}

	// Calculate square sum.
	double Sum=0;

	if (W.empty()){
		for (size_t i=0;i<V.size();++i){
			Sum+=(V[i]-avr)*(V[i]-avr);
		}
	}
	else {
		for (size_t i=0;i<V.size();++i){
			double diff=(W[i]>0?1:-1)*V[i]-avr;
			Sum+=(W[i]>0?W[i]:-W[i])*diff*diff;
		}
	}

	return {avr,sqrt(Sum/(SumW-1))};
}

#endif
