#ifndef ASU_INTERPOLATE
#define ASU_INTERPOLATE

#include<iostream>
#include<vector>

#include<ASU_tools.hpp>

/****************************************************************
 * This c++ template is only a wrapper for the Wiginterp function
 * included in same library. I did some modification and put the
 * SAC source code in Wiginterp.fun.cpp.
 *
 * Shule Yu
 * Nov 14 2017
 *
 * Key words: interpolate, wiggins
****************************************************************/


template<class T1, class T2, class T3>
std::vector<double> Interpolate(const std::vector<T1> &x, const std::vector<T2> &y,
								const std::vector<T3> &xx, const bool &IsEven){

	// Check array size.
	if (x.size()!=y.size()) {
		std::cerr <<  __func__ << "; Error: input array size is zero ..." << std::endl;
		return {};
	}

	// Check x is sorted and no repeat.
	bool flag1=true,flag2=true;

	for (size_t i=0;i+1<x.size();++i) {
		if (x[i+1]<=x[i]) {
			flag1=false;
			break;
		}
	}
	if (!flag1) {
		for (size_t i=0;i+1<x.size();++i) {
			if (x[i+1]>=x[i]) {
				flag2=false;
				break;
			}
		}
	}
	if (!flag1 && !flag2) {
		std::cerr <<  __func__ << "; Error: input x is either not sorted or has repeating value ..." << std::endl;
		return {};
	}

	double *X=new double [x.size()];
	double *Y=new double [y.size()];
	double *XX=new double [xx.size()];
	double *YY=new double [xx.size()];
	std::vector<double> ans(xx.size());

	for (size_t i=0;i<x.size();++i) {
		X[i]=x[i];
		Y[i]=y[i];
	}

	for (size_t i=0;i<xx.size();++i) XX[i]=xx[i];

	Wiginterp(X,Y,x.size(),XX,YY,xx.size(),IsEven);

	for (size_t i=0;i<xx.size();++i) ans[i]=YY[i];

	delete [] X;
	delete [] Y;
	delete [] XX;
	delete [] YY;

	return ans;
}

#endif
