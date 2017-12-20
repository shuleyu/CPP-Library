#include<cmath>

#include<ASU_tools.hpp>

/**************************************************************
 * This C++ function return the cartesian distance between two input pionts.
 *
 * Shule Yu
 * Mar 30 2017
 *
 * Key words: distance.
****************************************************************/

inline double DotDist(double x1,double y1,double x2,double y2){
	return sqrt(pow((x1-x2),2)+pow((y1-y2),2));
}
