#ifndef ASU_TP2LL
#define ASU_TP2LL

#include<iostream>
#include<cmath>

#include<Lon2180.hpp>

/*****************************************************
 * This C++ template convert Theta, Phi ( in rad. ) to
 * Longitude, Latitude ( in deg. ).
 *
 * The original coordinate x,y,z axis is as follows:
 * x axis: lon=0 ,lat=0
 * y axis: lon=90,lat=0.
 * z axis: lon=  ,lat=90.
 *
 * Theta : angle with z, 0 <= theta <= PI.
 * Phi   : angle with x, counter-clockwise.
 *
 * Longitude: -180 < lon <= 180. (Lon=0 at poles.)
 * Latitude : -90 <= lat <= 90.
 *
 * input(s):
 * const double &theta  ----  theta.
 * const double &phi    ----  phi.
 *
 * return(s):
 * pair<double,double> ans  ----  {lon,lat}
 *
 * Shule Yu
 * Dec 29 2017
 *
 * Key words: geography, coordinates, convertion.
*****************************************************/

std::pair<double,double> TP2LL(const double &theta,const double &phi){

    // Deal with latitude, theta.
    // Check theta.
    if ( theta > M_PI || theta < 0.0 ) {
        std::cerr <<  "Error in " << __func__ << ": input theta need to be 0 ~ pi ..." << std::endl;
        return {};
    }

    if (theta==0.0) return {0.0,90.0};
    else if (theta==M_PI) return {0.0,-90.0};

    return {Lon2180(phi*180/M_PI),(M_PI/2-theta)*180/M_PI};
}

#endif
