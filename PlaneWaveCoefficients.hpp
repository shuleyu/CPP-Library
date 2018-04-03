#ifndef ASU_PLANEWAVECOEFFICIENTS
#define ASU_PLANEWAVECOEFFICIENTS

#include<iostream>
#include<string>
#include<vector>
#include<cmath>

/****************************************************************************************************
 * This C++ template returns plane wave reflection/refraction(transmission) coefficients. Reflection
 * related coefficients are denoted as "R", distinguishing from "T" for refraction(trasmission).
 *
 * A. Ray incident from media 1 (parameters related to index 1).
 *
 * B. Interfaces (SS,SA,SL,LS):
 *
 *     Solid -> Solid  (SS):
 *
 *      PSV: 2(incident as P/SV) * 2(reflection/refraction) * 2(out-going as P/SV) = 8 coefficients.
 *      SH : 2(reflection/refraction)                                              = 2 coefficients.
 *
 *     Solid -> Air    (SA):
 *
 *      PSV: 2(incident as P/SV) * 2(out-going as P/SV)                            = 4 coefficients.
 *      SH : always reflect to SH (value is 1)                                     = 1 coefficients.
 *
 *     Solid -> Liquid (SL):
 *
 *      PSV: 2(incident as P/SV) * 3(out-going as P(T),SV(R),P(R))                 = 6 coefficients.
 *      SH : always reflect to SH (value is 1)                                     = 1 coefficients.
 *
 *     Liquid -> Solid (LS):
 *
 *      PSV: 1(incident as P) * 3(out-going as P(R),SV(T),P(T))                    = 3 coefficients.
 *
 *
 * C. input(s):
 *
 *     const T1     &rho1  ----  Density of media 1.
 *     const T2     &vp1   ----  P-wave velocity of media 1.
 *     const T3     &vs1   ----  S-wave velocity of media 1.
 *     const T4     &rho2  ----  Density of media 2.
 *     const T5     &vp2   ----  P-wave velocity of media 2.
 *     const T6     &vs2   ----  S-wave velocity of media 2.
 *     const T7     &inc   ----  Incident angle(in deg) in media 1.
 *     const string &cmp   ----  Component. "PSV" or "SH".
 *     const string &mode  ----  Structure mode. "SS", "SA", "SL" or "LS".
 *
 * D. return(s):
 *
 *     vector<double> ans  ----  displacement coefficients. Value and length depend on cmp and mode:
 *
 *      cmp     mode     ans
 *                       
 *      PSV     SS       ans.size()=8. R_PP, R_PS, T_PP, T_PS, R_SP, R_SS, T_SP, T_SS.
 *      SH      SS       ans.size()=2. R_SS, T_SS.
 *      PSV     SA       ans.size()=4. R_PP, R_PS, R_SP, R_SS.
 *      SH      SA       ans.size()=1. ans[0]=R_SS=1.
 *      PSV     SL       ans.size()=6. R_PP, R_PS, T_PP, R_SP, R_SS, T_SP.
 *      SH      SL       ans.size()=1. ans[0]=R_SS=1.
 *      PSV     LS       ans.size()=3. R_PP, T_PP, T_PS.
 *
 * Shule Yu
 * Apr 03 2018
 *
 * Key words: reflection, transmission, coefficient, plane wave, infinite frequency.
 *
 * Reference: Aki and Richards, Quantitative Seismology, 2nd ed., 2002.
 *            Page 134,136,139,144,145.
****************************************************************************************************/

template <class T1, class T2, class T3, class T4, class T5, class T6, class T7>
std::vector<double> PlaneWaveCoefficients(const T1 &rho1,const T2 &vp1 ,const T3 &vs1,
                                          const T4 &rho2,const T5 &vp2 ,const T6 &vs2,
                                          const T7 &inc,const std::string &cmp, const std::string &mode){
    if (cmp=="PSV") {
        if (mode=="SS") {
            return {};
        }
        else if (mode=="SA") {
            return {};
        }
        else if (mode=="SL") {
            return {};
        }
        else if (mode=="LS") {
            return {};
        }
        else {
            std::cerr <<  "Error in " << __func__ << ": input cmp/mode("
                      << cmp << "/" << mode <<") err ..." << std::endl;
            return {};
        }
    }
    else if (cmp=="SH"){
        if (mode=="SS") {
            double px=sin(inc/180*M_PI)/vs1;
            double A=rho1*vs1*sqrt(1-pow(vs1*px,2)),B=rho2*vs2*sqrt(1-pow(vs2*px,2));
            return {(A-B)/(A+B),2*A/(A+B)};
        }
        else if (mode=="SA" || mode=="SL") return {1};
        else {
            std::cerr <<  "Error in " << __func__ << ": input cmp/mode("
                      << cmp << "/" << mode <<") err ..." << std::endl;
            return {};
        }
    }
    else {
        std::cerr <<  "Error in " << __func__ << ": input cmp/mode("
                  << cmp << "/" << mode <<") err ..." << std::endl;
        return {};
    }
}

#endif
