#ifndef ASU_BOTTOMDEPTH 
#define ASU_BOTTOMDEPTH
// Need TauP Toolkit

#include<iostream>
#include<string>

#include<ShellExec.hpp>

/*****************************************************
 * This C++ template use TauP toolkit to calculate the
 * turning depth of seismic arrivals (for given
 * parameters).
 *
 * Using TauP toolkit. Model is PREM.
 *
 * input(s):
 * const double &Dist   ----  gcp distance (in deg.)
 * const double &EVDP   ----  source depth (in km.)
 * const string &Phase  ----  seismic phase.
 *
 * return(s):
 * double ans  ----  Bottom depth (in km.)
 *
 * Shule Yu
 * Dec 28 2017
 *
 * Key words: bottom depth.
*****************************************************/

double BottomDepth(const double &Dist,const double &EVDP,const std::string &Phase){

    std::string command="taup_path -mod prem -h "+std::to_string(EVDP)+" -ph "+Phase
                        +" -evt 0 0 -sta 0 "+std::to_string(Dist)
                        +" -o stdout | grep -v '>' | sort -g -k2 |  head -n 1 | awk '{print $2}'";

    std::string res=ShellExec(command);

    if (res.empty()) {
        std::cerr <<  "Warning in " << __func__ << ": No such phase for given parameters ..." << std::endl;
        return 0;
    }

    return 6371.0-stod(res);
}

#endif
