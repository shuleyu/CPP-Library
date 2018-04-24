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
 * const T1     &Dist   ----  gcp distance (in deg.)
 * const T2     &EVDP   ----  source depth (in km.)
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

template<class T1, class T2>
double BottomDepth(const T1 &Dist,const T2 &EVDP,const std::string &Phase){

    std::string command="taup_path -mod prem -h "+std::to_string(EVDP)+" -ph "+Phase
                        +" -evt 0 0 -sta 0 "+std::to_string(Dist)
                        +" -o stdout | grep -v '>' | sort -g -k2 |  head -n 1 | awk '{print $2}'";

    std::string res=ShellExec(command.c_str());

    if (res.empty()) {
        std::cerr <<  "Warning in " << __func__ << ": No such phase for given parameters ..." << std::endl;
        return 0;
    }

    return 6371.0-stod(res);
}

#endif
