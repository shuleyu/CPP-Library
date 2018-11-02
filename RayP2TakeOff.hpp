#ifndef ASU_RAYP2TAKEOFF
#define ASU_RAYP2TAKEOFF

#include<cmath>

#include<PREM.hpp>

/***********************************************************
 * This C++ template convert ray parameter (Taup output),
 * event depth, component name to take-off angle (in deg).
 *
 * input(s):
 * const double &rayp   ----  Ray parameter as the output of TauP.
 * const double &depth  ----  Event depth.
 * const char   &cmp    ----  'P' or 'S'.
 *
 * return(s):
 * double ans  ----  Take-off angle (0~90)
 *
 * Shule Yu
 * Dec 29 2017
 *
 * Key words: ray parameter, take off angle.
***********************************************************/

double RayP2TakeOff(const double &rayp, const double &depth, const char &cmp){

    double V;

    if (cmp=='P')      V=Dvp(depth);
    else if (cmp=='S') V=Dvs(depth);
    else {
        std::cerr <<  "Error in " << __func__ << ":  input component should be 'P' or 'S' ..." << std::endl;
        return -1;
    }

    if (rayp<0) {
        std::cerr <<  "Warning in " << __func__ << ":  input ray parameter is negative ..." << std::endl;
        return -1;
    }

    return asin(V*rayp*180/M_PI/(6371.0-depth))*180/M_PI;
}

#endif
