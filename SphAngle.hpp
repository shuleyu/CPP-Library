#ifndef ASU_SPHANGLE
#define ASU_SPHANGLE

#include<cmath>

/***********************************************************
 * This C++ template returns the angle ( in rad. ) between
 * two directions given in spherical coordinates ( in rad. ).
 *
 * Theta     : angle with z, 0 <= theta <= PI.
 * Phi       : angle with x, counter-clockwise.
 *
 * input(s):
 * const double &theta1  ----  theta1.
 * const double &phi1    ----  phi1.
 * const double &theta2  ----  theta2.
 * const double &phi2    ----  phi2.
 *
 * return(s):
 * double ans  ----  the angle between input directions.
 *
 * Shule Yu
 * Dec 31 2017
 *
 * Key words: geography, coordinates, angle.
***********************************************************/

double SphAngle(const double &theta1, const double &phi1, const double &theta2, const double &phi2){

    double a1,b1,c1,a2,b2,c2;

    // Find the unit vector of direction 1.
    c1=cos(theta1);
    a1=sin(theta1);
    b1=a1*sin(phi1);
    a1*=cos(phi1);

    // Find the unit vector of direction 2.
    c2=cos(theta2);
    a2=sin(theta2);
    b2=a2*sin(phi2);
    a2*=cos(phi2);

    // Make Angle.
    return acos(a1*a2+b1*b2+c1*c2);
}

#endif
