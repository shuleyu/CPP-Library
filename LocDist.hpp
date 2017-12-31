#ifndef ASU_LOCDIST
#define ASU_ALOCDIST

#include<ASU_tools.hpp>

/***********************************************************
 * This C++ template caculate the straight line distance
 * between two geographic points.
 *
 * const T1 &lon1   ----  
 * const T2 &lat1   ----  
 * const T3 &r1   ----  
 * const T4 &lon2   ----  
 * const T5 &lat2   ----  
 * const T6 &r2   ----  
 *
 * Shule Yu
 * Dec 31 2017
 *
 * Key words: geography, coordinates, distance.
***********************************************************/

template <class T1,class T2,class T3,class T4,class T5,class T6>
double LocDist(const T1 &lon1, const T2 &lat1, const T3 &r1, const T4 &lon2, const T5 &lat2, const T6 &r2){


    auto res1=LL2TP(lon1,lat1);
    auto res2=LL2TP(lon2,lat2);

    double x1,y1,z1,x2,y2,z2;
	
    Sph2Cart(r1,res1.first,res1.second,x1,y1,z1);
    Sph2Cart(r2,res2.first,res2.second,x2,y2,z2);

    return DotDist(x1,y1,z1,x2,y2,z2);
}

#endif
