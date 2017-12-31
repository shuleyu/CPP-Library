#ifndef ASU_RAYP2TAKEOFF
#define ASU_RAYP2TAKEOFF

#include<cmath>

#include<ASU_tools.hpp>

/***********************************************************
 * This C++ template convert ray parameter from Taup, event
 * depth, phase to take-off angle ( in rad. ).
 *
 * const T1   &rayp   ----  Ray parameter as the output of TauP.
 * const T2   &depth  ----  Event depth.
 * const char &phase  ----  'P' or 'S'.
 *
 * Shule Yu
 * Dec 29 2017
 *
 * Key words: ray parameter, take off angle.
***********************************************************/

template<class T1, class T2>
double RayP2TakeOff(const T1 &rayp, const T2 &depth,const char &phase){

    double V;

    if (phase=='P')      V=Dvp(depth);
    else if (phase=='S') V=Dvs(depth);
	else {
		std::cerr <<  __func__ << "; Error: input phase should be 'P' or 'S' ..." << std::endl; 
		return -1;
	}

    return asin(V*rayp*180/M_PI/(6371.0-depth));
}

#endif
