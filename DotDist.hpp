#ifndef ASU_DOTDIST
#define ASU_DOTDIST

#include<cmath>

#include<ASU_tools.hpp>

/*********************************************************
 * This C++ template return the cartesian distance between
 * two input pionts.
 *
 * Shule Yu
 * Mar 30 2017
 *
 * Key words: distance.
*********************************************************/

template<class T1, class T2, class T3, class T4>
double DotDist(const T1 &x1, const T2 &y1,const T3 &x2,const T4 &y2){
	return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

#endif
