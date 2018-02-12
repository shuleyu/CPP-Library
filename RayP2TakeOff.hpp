#ifndef ASU_RAYP2TAKEOFF
#define ASU_RAYP2TAKEOFF

#include<cmath>

#include<PREM.hpp>

/***********************************************************
 * This C++ template convert ray parameter (Taup output),
 * event depth, component name to take-off angle (in deg).
 *
 * input(s):
 * const T1   &rayp   ----  Ray parameter as the output of TauP.
 * const T2   &depth  ----  Event depth.
 * const char &cmp    ----  'P' or 'S'.
 *
 * return(s):
 * double ans  ----  Take-off angle (0~90)
 *
 * Shule Yu
 * Dec 29 2017
 *
 * Key words: ray parameter, take off angle.
***********************************************************/

template<class T1, class T2>
double RayP2TakeOff(const T1 &rayp, const T2 &depth,const char &cmp){

    double V;

    if (cmp=='P')      V=Dvp(depth);
    else if (cmp=='S') V=Dvs(depth);
    else {
        std::cerr <<  "Error in " << __func__ << ":  input component should be 'P' or 'S' ..." << std::endl;
        return -1;
    }

    return asin(V*rayp*180/M_PI/(6371.0-depth))*180/M_PI;
}

#endif
