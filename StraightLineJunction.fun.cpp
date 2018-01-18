#include<iostream>
#include<cmath>
#include<algorithm>

/**************************************************************
 * This C function return the after-restoration position of a
 * point inside the unit square {(0,0), (1,0), (1,1), (0,1)}.
 *
 * double x1                 ----  x1.
 * double y1                 ----  y1.
 * double s1                 ----  slope 1.
 * double x2                 ----  x2.
 * double y2                 ----  y2.
 * double s2                 ----  slope 2.
 *
 * return value:
 * pair<double,double>  ans  ----  The position of the junction point.
 *
 * Shule Yu
 * Mar 30 2017
 *
 * Key words: stretching.
****************************************************************/

std::pair<double,double> StraightLineJunction(double x1,double y1,double s1,double x2,double y2,double s2){

	std::pair<double,double> ans{std::numeric_limits<double>::max(),std::numeric_limits<double>::max()};

	if (s1==s2 || (std::isinf(s1) && std::isinf(s2)) ){
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
