#ifndef ASU_LINEJUNCTION
#define ASU_LINEJUNCTION

#include<iostream>
#include<cmath>
#include<limits>

/**************************************************************
 * This C++ template return the after-restoration position of a
 * point inside the unit square {(0,0), (1,0), (1,1), (0,1)}.
 *
 * const T1 &x1  ----  x1.
 * const T2 &y1  ----  y1.
 * const T3 &s1  ----  slope 1.
 * const T4 &x2  ----  x2.
 * const T5 &y2  ----  y2.
 * const T6 &s2  ----  slope 2.
 *
 * return value:
 * pair<double,double>  ans  ----  The position of the junction point.
 *
 * Shule Yu
 * Jan 19 2018
 *
 * Key words: line junction.
****************************************************************/

template<class T1,class T2,class T3,class T4,class T5,class T6>
std::pair<double,double> LineJunction(const T1 &x1,const T2 &y1,const T3 &s1,const T4 &x2,const T5 &y2,const T6 &s2){

	std::pair<double,double> ans{std::numeric_limits<double>::max(),std::numeric_limits<double>::max()};

	if (s1==s2 || (std::isinf(s1) && std::isinf(s2)) ){
        std::cerr <<  __func__ << "; Warning: parallel lines ..." << std::endl;
		return ans;
	}

	double b1=y1-s1*x1,b2=y2-s2*x2;
	if (std::isinf(s1)){
		ans.first=x1;
		ans.second=s2*x1+b2;
	}
	else if (std::isinf(s2)){
		ans.first=x2;
		ans.second=s1*x2+b1;
	}
	else{
		ans.second=((s2*b1-s1*b2)/(s2-s1));
		if (s1!=0) ans.first=(ans.second-b1)/s1;
		else ans.first=(ans.second-b2)/s2;
	}

	return ans;
}

#endif
