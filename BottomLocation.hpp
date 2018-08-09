#ifndef ASU_BOTTOMLOCATION
#define ASU_BOTTOMLOCATION
// Need TauP Toolkit

#include<iostream>
#include<vector>
#include<string>

#include<ShellExec.hpp>

/**************************************************
 * This C++ template use TauP toolkit to return the
 * geographic location of the turnning point of the
 * seismic arrival (for the given parameters).
 *
 * Using TauP toolkit. Model is PREM.
 *
 * input(s):
 * const T1     &evtlon  ----  event lon
 * const T2     &evtlat  ----  event lat
 * const T3     &EVDP    ----  event depth (in km.)
 * const T4     &stalon  ----  station lon
 * const T5     &stalat  ----  station lat
 * const string &Phase   ----  seismic phase
 *
 * return(s):
 * vector<double> ans  ----  Turnning piont {depth,lon,lat}.
 *
 * Shule Yu
 * Dec 28 2017
 *
 * Key words: bottom location.
**************************************************/

template<class T1, class T2, class T3, class T4, class T5>
std::vector<double> BottomLocation(const T1 &evtlon, const T2 &evtlat, const T3 &EVDP,
                                   const T4 &stalon, const T5 &stalat,const std::string &Phase){

    std::string command="taup_path -mod prem -h "+std::to_string(EVDP)+" -ph "+Phase
                        +" -evt "+std::to_string(evtlat)+" "+std::to_string(evtlon)+
                        +" -sta "+std::to_string(stalat)+" "+std::to_string(stalon)+
                        +" -o stdout | grep -v '>' | sort -g -k2 |  head -n 1 | awk '{print $2,$3,$4}'";

    std::string res=ShellExec(command);
    if (res.empty()) {
        std::cerr <<  "Warning in " << __func__ << ": No such phase for given parameters ..." << std::endl;
        return {};
    }

    // Parse the string.
    size_t pb=0,p=0;
    while (std::isspace(res[p])) ++p;
    pb=p;
    while (!std::isspace(res[p])) ++p;
    double depth=6371.0-stod(res.substr(pb,p));

    while (std::isspace(res[p])) ++p;
    pb=p;
    while (!std::isspace(res[p])) ++p;
    double plat=stod(res.substr(pb,p));

    while (std::isspace(res[p])) ++p;
    pb=p;
    while (p<res.size() && !std::isspace(res[p])) ++p;
    double plon=stod(res.substr(pb,p));

    return {depth,plon,plat};
}

#endif
