#ifndef ASU_SNR
#define ASU_SNR

#include<iostream>
#include<vector>
#include<cmath>
#include<numeric>

#include<Amplitude.hpp>
#include<Envelope.hpp>
#include<SimpsonRule.hpp>

/***********************************************************
 * This C++ template evaluate the signal/noise ratio of
 * input signal.
 *
 * method=0, default
 * 1. Calculate envelope trace. (See Envelope.hpp)
 * 2. Integrate between singal window and noise window.
 * 3. Calculate the ratio, averaged by window lengths.
 *
 * method=1
 * 1. Find the peak within the signal window.
 * 2. Find the peak within the noise window.
 * 3. Take the ratio.
 *
 * method=2
 * 1. Find the peak within the signal window.
 * 2. Find the averaged absolute amplitude through the noise window. (after taking envelope)
 * 3. Take the ratio.
 *
 * input(s):
 * const vector<T> &p       ----  Input array pointer.
 * const int       &nloc    ----  Noise window start position.
 * const int       &nlen    ----  Noise window length.
 * const int       &sloc    ----  Signal window start position.
 * const int       &slen    ----  Signal window length.
 * const int       &method  ----  Choose one method above, default is 0.
 *
 * return(s):
 * double ans  ----  SNR estimation.
 *
 * Shule Yu
 * Jan 20 2018
 *
 * Key words: signal to noise ratio, SNR
***********************************************************/

template<class T>
double SNR(const std::vector<T> &p,const int &nloc,const int &nlen,const int &sloc,const int &slen,const int &method=0){

    int npts=p.size();

    if (nloc<0 || nloc>npts || nlen<0 || nloc+nlen>npts){
        std::cerr <<  "Error in " << __func__ << ": Noise window error ..." << std::endl;
        return 0.0;
    }
    if (sloc<0 || sloc>npts || slen<0 || sloc+slen>npts){
        std::cerr <<  "Error in " << __func__ << ": Signal window error ..." << std::endl;
        return 0.0;
    }

    double Slevel,Nlevel;
    std::vector<double> res;

    switch (method){

        case 0:

            res=Envelope(p);

            // integrate the envelope using Simpson's rule. (ignore h/3 term).
            Slevel=SimpsonRule(std::vector<double> (res.begin()+sloc,res.begin()+sloc+slen));
            Nlevel=SimpsonRule(std::vector<double> (res.begin()+nloc,res.begin()+nloc+nlen));

            // average with respect to their length.
            return Slevel/Nlevel*(1.0*nlen/slen);

        case 1:

            Slevel=Amplitude(std::vector<T> (p.begin()+sloc,p.begin()+sloc+slen)).first;
            Nlevel=Amplitude(std::vector<T> (p.begin()+nloc,p.begin()+nloc+nlen)).first;

            return Slevel/Nlevel;

        case 2:

            res=Envelope(p);

            Slevel=Amplitude(std::vector<T> (p.begin()+sloc,p.begin()+sloc+slen)).first;
//             Nlevel=Amplitude(std::vector<double> (res.begin()+nloc,res.begin()+nloc+nlen)).first;
            Nlevel=std::accumulate(res.begin()+nloc,res.begin()+nloc+nlen,0.0)/nlen;

            return Slevel/Nlevel;

        default:

            std::cerr <<  "Error in " << __func__ << ": Method error ..." << std::endl;
            return -1;
    }
}

#endif
