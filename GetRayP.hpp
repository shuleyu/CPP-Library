#ifndef ASU_GETRAYP
#define ASU_GETRAYP

#include<iostream>
#include<string>

/*****************************************************
 * This C++ template use TauP toolkit to calculate the
 * ray parameter of given parameters.
 *
 * Using TauP toolkit. Model is PREM.
 * If there are triplications, return the rayp of the
 * first arrival.
 *
 * input(s):
 * const T1     &Dist   ----  gcp distance (in deg.)
 * const T2     &EVDP   ----  source depth (in km.)
 * const string &Phase  ----  seismic phase.
 *
 * return(s):
 * double ans  ----  ray parameter (in sec/rad.)
 *                   -1 if there's no such ray.
 *
 * Shule Yu
 * Feb 27 2018
 *
 * Key words: ray parameters.
*****************************************************/

template<class T1, class T2>
double GetRayP(const T1 &Dist,const T2 &EVDP,const std::string &Phase){

    std::string command="taup_time -mod prem -h "+std::to_string(EVDP)
                        + " -deg " + std::to_string(Dist) + " -ph " + Phase
                        + " --rayp | awk '{print $1}'";

    std::string res=ShellExec(command);

    if (res.empty()) {
        std::cerr <<  "Warning in " << __func__ << ": No such rays for given parameters ..." << std::endl;
        return -1;
    }

    return stod(res);
}

#endif
