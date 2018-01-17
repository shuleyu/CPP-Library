#ifndef ASU_CMT2SPH
#define ASU_CMT2SPH

#include<cmath>

/************************************************************
 * This C function convert the direction of slip from
 * Strike, Dip, Rake ( in deg. ) to theta and azimuth (the
 * spherical coordinates center at the center of the focal
 * mechanism sphere, with North phi=0, East phi=pi/2,
 * South phi=pi; Up theta=0, Down theta=pi. in deg.)
 *
 * Return values:
 * Azimuth   : clockwise angle with the North
 *                                          0 < Phi <= 2*pi.
 * Theta     : angle with upward direction. 0 <= Theta <= pi.
 *
 *
 * T1 strike  ----  Strike.
 * T2 dip     ----  Dip.
 * T3 rake    ----  Rake.
 *
 * Shule Yu
 * Dec 27 2017
 *
 * Key words: CMT, coordinates, convertion.
************************************************************/

template<class T1, class T2, class T3>
std::pair<double,double> CMT2Sph(const T1 &strike, const T2 &dip, const T3 &rake){

    // Convert cmt to rad.
    double s=strike*M_PI/180;
    double d=dip*M_PI/180;
    double r=rake*M_PI/180;

    // Phi direction.
    double a=s-atan2(tan(r),sqrt(1+pow(tan(d),2)));

    // Theta direction.
    double t=M_PI/2-asin(tan(d)*sin(r)/sqrt(1+pow(tan(d),2)));

    return {a*180/M_PI,t*180/M_PI};
}

#endif
