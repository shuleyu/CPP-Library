#ifndef ASU_GETRAYP
#define ASU_GETRAYP
// Need TauP Toolkit

#include<iostream>
#include<string>

#include<ShellExec.hpp>

/*****************************************************
 * This C++ template use TauP toolkit to calculate the
 * ray parameter of given parameters.
 *
 * Using TauP toolkit. Model is PREM.
 * If there are triplications, return the rayp of the
 * first arrival.
 *
 * input(s):
 * const double &Dist   ----  gcp distance (in deg.)
 * const double &EVDP   ----  source depth (in km.)
 * const string &Phase  ----  seismic phase.
 *
 * return(s):
 * double ans  ----  ray parameter (same unit as
 *                   current version of taup, usually
 *                   in sec/deg, p=Rsin/c/180*PI)
 *                   return -1 if there's no such ray.
 *
 * Shule Yu
 * Feb 27 2018
 *
 * Key words: ray parameters.
*****************************************************/

double GetRayP(const double &Dist,const double &EVDP,const std::string &Phase){

    std::string command="taup_time -mod prem -h "+std::to_string(EVDP)
                        + " -deg " + std::to_string(Dist) + " -ph " + Phase
                        + " --rayp | awk '{print $1}'";

    std::string res=ShellExec(command);

    if (res.empty()) {
        std::cerr <<  "Warning in " << __func__ << ": No such rays for the following parameters: " << "Deg: " << Dist << ". Depth: " << EVDP << ". Phase: " << Phase << ". " << std::endl;

        return -1;
    }

    return stod(res);
}

#endif
