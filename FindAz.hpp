#ifndef ASU_FINDAZ
#define ASU_FINDAZ

#include<cmath>

/***********************************************************
 * This C++ template returns azimuth (in deg) of two
 * geographic locations, centered at the first point.
 *
 * const T1 &lo1  ----  longitude of point 1
 * const T2 &la1  ----  latitude  of point 1
 * const T3 &lo2  ----  longitude of point 2
 * const T4 &la2  ----  latitude  of point 2
 *
 * Shule Yu
 * Jan 23 2018
 *
 * Key words: azimuth.
 *
 * Reference:
 *     https://en.wikipedia.org/wiki/Spherical_trigonometry#Sine_rules
***********************************************************/

template<class T1,class T2, class T3, class T4>
double FindAz(const T1 &lo1,const T2 &la1,const T3 &lo2,const T4 &la2){

    double x,y;
    double a,b,C;
    a=la1*M_PI/180;
    b=la2*M_PI/180;
    C=(lo2-lo1)*M_PI/180;
    y=sin(C);
    x=cos(a)*tan(b)-sin(a)*cos(C);

    return atan2(y,x)*180/M_PI;
}

#endif
