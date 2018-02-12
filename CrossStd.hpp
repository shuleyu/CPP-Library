#ifndef ASU_CROSSSTD
#define ASU_CROSSSTD

#include<iostream>
#include<vector>
#include<limits>

#include<AvrStd.hpp>

/**************************************************************
 * This C++ template calculate the "cross standard deviation"
 * of two given signals s1 and s2.
 *
 * s2 should be shorter than s1.
 *
 * s2 is the sliding signal, and its movement is restricted to
 * s1's length (the overlapping part length is always s2's length).
 *
 * For each shift position, for the overlapping part:
 * the difference between s1 and s2 are calculated as a residual
 * signal, then the standard deviation of the residual signal
 * is calculated, as an indicator whether this shift position is
 * optimal in terms of whether two signals match each other.
 *
 * input(s):
 * const vector<T1> &s1  ----  1D signal 1.
 * const vector<T2> &s2  ----  1D signal 2.
 *
 * return(s):
 * pair<int,double> ans  ----  {position of best match, standard deviation of the residual}
 *
 *
 * Shule Yu
 * Jan 18 2018
 *
 * Key words: modified cross-correlation, moving standard deviaiton
****************************************************************/

template<class T1, class T2>
std::pair<int,double> CrossStd(const std::vector<T1> &s1, const std::vector<T2> &s2){

    int xlen=s1.size(),ylen=s2.size();

    // If signals are too short...
    if (xlen<=1 || ylen<=1){
        std::cerr <<  "Error in " << __func__ << ": Input sequences length error ..." << std::endl;
        return {};
    }

    // Finding s2 in s1, therefore xlen should be larger than ylen.
    if (xlen<ylen){
        std::cerr <<  "Error in " << __func__ << ": s2 length larger than s1 length ..." << std::endl;
        return {};
    }

    // Shifting and evaluating STD at each step.
    std::vector<double> A(ylen,0);
    int shift=0;
    double STD=std::numeric_limits<double>::max();

    for (int delay=0;delay<xlen-ylen+1;++delay){

        // produce the residual signal.
        for (int i=0;i<ylen;++i) A[i]=s1[i+delay]-s2[i];

        // calculate std of the residual signal.
        auto res=AvrStd(A);

        if (STD>res.second){
            STD=res.second;
            shift=delay;
        }
    }
    return {shift,STD};
}

#endif
