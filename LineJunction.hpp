#ifndef ASU_LINEJUNCTION
#define ASU_LINEJUNCTION

#include<iostream>
#include<cmath>
#include<limits>

/**************************************************************
 * This C++ template return the junction point of two straigh
 * line.
 *
 * const pair<T1,T2> &p1  ----  first point.
 * const T3          &s1  ----  slope through the first point.
 * const pair<T4,T5> &p2  ----  second point.
 * const T6          &s2  ----  slope through the second point.
 *
 * return value:
 * std::pair<bool,pair<double,double>>  ans
 *                       ----  Result flag and
 *                             the position of the junction point.
 *
 * Shule Yu
 * Jan 19 2018
 *
 * Key words: line junction.
****************************************************************/

template<class T1,class T2,class T3,class T4,class T5,class T6>
std::pair<bool,std::pair<double,double>> LineJunction(const std::pair<T1,T2> &p1,const T3 &s1,
                                                      const std::pair<T4,T5> &p2,const T6 &s2){

	std::pair<double,double> ans{std::numeric_limits<double>::max(),std::numeric_limits<double>::max()};

	if (s1==s2 || (std::isinf(s1) && std::isinf(s2))) return {false,ans};

	double b1=p1.second-s1*p1.first,b2=p2.second-s2*p2.first;
	if (std::isinf(s1)){
		ans.first=p1.first;
		ans.second=s2*p1.first+b2;
	}
	else if (std::isinf(s2)){
		ans.first=p2.first;
		ans.second=s1*p2.first+b1;
	}
	else{
		ans.second=((s2*b1-s1*b2)/(s2-s1));
		if (s1!=0) ans.first=(ans.second-b1)/s1;
		else ans.first=(ans.second-b2)/s2;
	}

	return {true,ans};
}

#endif
