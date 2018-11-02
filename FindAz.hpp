#ifndef ASU_FINDAZ
#define ASU_FINDAZ

#include<cmath>

/***********************************************************
 * This C++ template returns azimuth (in deg) of two
 * geographic locations, centered at the first point.
 *
 * input(s):
 * const double &lo1  ----  longitude of point 1
 * const double &la1  ----  latitude  of point 1
 * const double &lo2  ----  longitude of point 2
 * const double &la2  ----  latitude  of point 2
 *
 * return(s):
 * double ans  ----  azimuth from p1 to p2 centered on p1.
 *
 * Shule Yu
 * Jan 23 2018
 *
 * Key words: azimuth.
 *
 * Reference:
 *     https://en.wikipedia.org/wiki/Spherical_trigonometry#Sine_rules
***********************************************************/

double FindAz(const double &lo1,const double &la1,const double &lo2,const double &la2){

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
