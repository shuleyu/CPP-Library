#ifndef ASU_GCPDISTANCE
#define ASU_GCPDISTANCE

#include<cmath>

/*****************************************************
 * This C++ template returns distance (in deg) of two
 * geographic locations.
 *
 * input(s):
 * const double &lo1  ----  longitude of point 1
 * const double &la1  ----  latitude  of point 1
 * const double &lo2  ----  longitude of point 2
 * const double &la2  ----  latitude  of point 2
 *
 * return(s):
 * double ans  ----  distance.
 *
 * Shule Yu
 * Dec 26 2017
 *
 * Key words: great circle path, gcp, distance.
 *
 * Reference:
 *     http://en.wikipedia.org/wiki/Great-circle_distance
*****************************************************/

double GcpDistance(const double &lo1,const double &la1,const double &lo2,const double &la2){

    double a,b,C,x,y;

    a=la1*M_PI/180;
    b=la2*M_PI/180;
    C=(lo1-lo2)*M_PI/180;

    y=sqrt(pow((cos(a)*sin(b)-sin(a)*cos(b)*cos(C)),2)+pow(cos(b)*sin(C),2));
    x=sin(a)*sin(b)+cos(a)*cos(b)*cos(C);

    return 180/M_PI*atan2(y,x);
}

#endif
