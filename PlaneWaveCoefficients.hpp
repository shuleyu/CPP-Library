#ifndef ASU_PLANEWAVECOEFFICIENTS
#define ASU_PLANEWAVECOEFFICIENTS

#include<iostream>
#include<string>
#include<vector>
#include<cmath>
#include<complex>

/****************************************************************************************************
 * This C++ template returns plane wave reflection/refraction(transmission) coefficients. Reflection
 * related coefficients are denoted as "R", distinguishing from "T" for refraction(trasmission).
 *
 * A. For post-critical incident, reflection coefficients should be an imaginary number with
 *    amplitude 1. For the sake of consistant, will use complex<double> for all return values. Pre-
 *    critical incident will returns complex values with imaginary part 0.
 *
 * B. The Frequency Sign by default is negative to gives an evasive wave at post-critical conditions,
 *    whose amplitude decrease exponentially away from the interface.
 *
 * C. Ray always incident from media 1 (parameters related to index 1).
 *
 * D. In Aki & Richard, the meaning of angles (i,j) are related to wave types(P,S) for PSV. Here
 *    we use i for incident angle; j (with subscripts "_r", "_pp", "_ps", "_sp" and "_ss" for specific
 *    cases) to denote reflection or refraction(transmission) angles.
 *
 * E. Interfaces code names (SS,SA,SL,LS) (Media1 -> Media2).
 *
 *    Return (ans) order, for example, if cmp=="PSV" and mode=="SS":
 *    ans[0]=R_PP, ans[1]=R_PS, ...
 *
 *     Solid -> Solid  (SS):
 *
 *      PSV(8): R_PP, R_PS, R_SP, R_SS, T_PP, T_PS, T_SP, T_SS
 *      SH (2): R_SS, T_SS
 *
 *     Solid -> Liquid (SL):
 *
 *      PSV(6): R_PP, R_PS, R_SP, R_SS, T_PP, T_SP
 *      SH (1): R_SS (R_SS=1)
 *
 *     Solid -> Air    (SA):
 *
 *      PSV(2): R_PP, R_PS, R_SP, R_SS
 *      SH (1): R_SS (R_SS=1) [Notice: the phase shift introduced by internal caustic surface is not considered.]
 *
 *     Liquid -> Solid (LS):
 *
 *      PSV(3): R_PP, T_PP, T_PS
 *      SH (0):
 *
 *     Liquid -> Liquid (LL):
 *
 *      PSV(2): R_PP, T_PP
 *      SH (0):
 *
 *     Liquid -> Air (LA):
 *
 *      PSV(1): R_PP (R_PP=1)
 *      SH (0):
 *
 * F. input(s):
 *
 *     const double &rho1      ----  Density of media 1.
 *     const double &vp1       ----  P-wave velocity of media 1.
 *     const double &vs1       ----  S-wave velocity of media 1.
 *     const double &rho2      ----  Density of media 2.
 *     const double &vp2       ----  P-wave velocity of media 2.
 *     const double &vs2       ----  S-wave velocity of media 2.
 *     (If media2 is liquid or air, rho2, vp2, vs2 sometimes are ignored)
 *
 *     const double &inc       ----  Incident angle(in deg) in media 1.
 *     const string &cmp       ----  Component. "PSV" or "SH".
 *     const string &mode      ----  Structure mode. "SS", "SL", "SA", "LS", "LL", "LA".
 *     const int    &FreqSign  ----  (Optional, physics convention) Default value is -1, sign of frequencies.
 *
 * G. return(s):
 *
 *     vector<complex<double>> ans  ----  displacement coefficients. (see section E)
 *
 * Shule Yu
 * Apr 03 2018
 *
 * Key words: reflection, transmission, coefficient, plane wave, infinite frequency.
 *
 * Notes    : We are using negative freqeuncies so the imaginary part of vertical slowness is negative.
 *
 * Reference: Aki and Richards, Quantitative Seismology, 2nd ed., 2002.
 *            Page 140 (eq 5.34) - boundary conditions.
 *            Page 134,136,139,144,145 - results for "PSV-SA", "SH-SS", "PSV-SS"
****************************************************************************************************/

std::vector<std::complex<double>>
PlaneWaveCoefficients(const double &rho1,const double &vp1 ,const double &vs1,
                      const double &rho2,const double &vp2 ,const double &vs2,
                      const double &inc,const std::string &cmp,
                      const std::string &mode, const int &FreqSign=-1){

    double sini=sin(inc/180*M_PI),cosi=sqrt(1-sini*sini);

    if (cmp=="PSV") {

        if (mode=="SS") {

            // P as incident.
            double p=sini/vp1,sinj_r=p*vs1,sinj_pp=p*vp2,sinj_ps=p*vs2;

            std::complex<double> yp1=cosi/vp1,yp2(0),ys1(0),ys2(0);
            if (sinj_pp<=1) yp2=sqrt(1-sinj_pp*sinj_pp)/vp2;
            else yp2.imag(FreqSign*sqrt(p*p-1.0/vp2/vp2));

            if (sinj_r<=1) ys1=sqrt(1-sinj_r*sinj_r)/vs1;
            else ys1.imag(FreqSign*sqrt(p*p-1.0/vs1/vs1));

            if (sinj_ps<=1) ys2=sqrt(1-sinj_ps*sinj_ps)/vs2;
            else ys2.imag(FreqSign*sqrt(p*p-1.0/vs2/vs2));

            std::complex<double> a,b,c,d,E,F,G,H,D;
            a=rho2*(1-2*vs2*vs2*p*p)-rho1*(1-2*vs1*vs1*p*p);
            b=rho2*(1-2*vs2*vs2*p*p)+2*rho1*vs1*vs1*p*p;
            c=rho1*(1-2*vs1*vs1*p*p)+2*rho2*vs2*vs2*p*p;
            d=2*(rho2*vs2*vs2-rho1*vs1*vs1);

            E=b*yp1+c*yp2;
            F=b*ys1+c*ys2;
            G=a-d*yp1*ys2;
            H=a-d*yp2*ys1;

            D=E*F+G*H*p*p;

            std::complex<double> R_PP,R_PS,T_PP,T_PS;
            R_PP=(b*yp1-c*yp2)*F-(a+d*yp1*ys2)*H*p*p;
            R_PS=-2.0*yp1*(a*b+c*d*yp2*ys2)*p*vp1/vs1;
            T_PP=2.0*rho1*yp1*F*vp1/vp2;
            T_PS=2.0*rho1*yp1*H*p*vp1/vs2;

            R_PP/=D;
            R_PS/=D;
            T_PP/=D;
            T_PS/=D;


            // SV as incident.
            p=sini/vs1,sinj_r=p*vp1;
            double sinj_ss=p*vs2,sinj_sp=p*vp2;

            ys1=cosi/vs1,ys2=0,yp1=0,yp2=0;

            if (sinj_r<=1) yp1=sqrt(1-sinj_r*sinj_r)/vp1;
            else yp1.imag(FreqSign*sqrt(p*p-1.0/vp1/vp1));

            if (sinj_ss<=1) ys2=sqrt(1-sinj_ss*sinj_ss)/vs2;
            else ys2.imag(FreqSign*sqrt(p*p-1.0/vs2/vs2));

            if (sinj_sp<=1) yp2=sqrt(1-sinj_sp*sinj_sp)/vp2;
            else yp2.imag(FreqSign*sqrt(p*p-1.0/vp2/vp2));


            a=rho2*(1-2*vs2*vs2*p*p)-rho1*(1-2*vs1*vs1*p*p);
            b=rho2*(1-2*vs2*vs2*p*p)+2*rho1*vs1*vs1*p*p;
            c=rho1*(1-2*vs1*vs1*p*p)+2*rho2*vs2*vs2*p*p;
            d=2*(rho2*vs2*vs2-rho1*vs1*vs1);

            E=b*yp1+c*yp2;
            F=b*ys1+c*ys2;
            G=a-d*yp1*ys2;
            H=a-d*yp2*ys1;

            D=E*F+G*H*p*p;

            std::complex<double> R_SP,R_SS,T_SP,T_SS;
            R_SP=-2.0*ys1*(a*b+c*d*yp2*ys2)*p*vs1/vp1;
            R_SS=-(b*ys1-c*ys2)*E+(a+d*yp2*ys1)*G*p*p;
            T_SP=-2.0*rho1*ys1*G*p*vs1/vp2;
            T_SS=2.0*rho1*ys1*E*vs1/vs2;

            R_SP/=D;
            R_SS/=D;
            T_SP/=D;
            T_SS/=D;

            return {R_PP,R_PS,R_SP,R_SS,T_PP,T_PS,T_SP,T_SS};
        }
        else if (mode=="SL") {

            // P as incident.
            double p=sini/vp1,sinj_r=p*vs1,sinj_pp=p*vp2;

            std::complex<double> yp1=cosi/vp1,yp2(0),ys1(0);
            if (sinj_pp<=1) yp2=sqrt(1-sinj_pp*sinj_pp)/vp2;
            else yp2.imag(FreqSign*sqrt(p*p-1.0/vp2/vp2));

            if (sinj_r<=1) ys1=sqrt(1-sinj_r*sinj_r)/vs1;
            else ys1.imag(FreqSign*sqrt(p*p-1.0/vs1/vs1));

            std::complex<double> a,b,D;
            a=4*rho1*rho1*pow(vs1,4)*p*p*yp1*ys1;
            b=rho1*rho1*pow((1-2*vs1*vs1*p*p),2);
            D=(a+b)*yp2+rho1*rho2*yp1;

            std::complex<double> R_PP,R_PS,T_PP;
            R_PP=(a-b)*yp2+rho1*rho2*yp1;
            R_PS=2.0*rho1*rho1*vp1*vs1*(1-2*vs1*vs1*p*p)*p*yp1*yp2;
            T_PP=2.0*rho1*rho1*vp1/vp2*(1-2*vs1*vs1*p*p)*yp1;

            R_PP/=D;
            R_PS/=D;
            T_PP/=D;

            // S as incident.
            p=sini/vs1;sinj_r=p*vp1;
            double sinj_sp=p*vp2;

            ys1=cosi/vs1;yp1=0;yp2=0;
            if (sinj_r<=1) yp1=sqrt(1-sinj_r*sinj_r)/vp1;
            else yp1.imag(FreqSign*sqrt(p*p-1.0/vp1/vp1));

            if (sinj_sp<=1) yp2=sqrt(1-sinj_sp*sinj_sp)/vp2;
            else yp2.imag(FreqSign*sqrt(p*p-1.0/vp2/vp2));

            a=4*rho1*rho1*pow(vs1,4)*p*p*yp1*ys1;
            b=rho1*rho1*pow((1-2*vs1*vs1*p*p),2);
            D=(a+b)*yp2+rho1*rho2*yp1;

            std::complex<double> R_SP,R_SS,T_SP;
            R_SP=4*rho1*rho1*pow(vs1,3)/vp1*(1-2*vs1*vs1*p*p)*p*ys1*yp2;
            R_SS=(b-a)*yp2+rho1*rho2*yp1;
            T_SP=-4*rho1*rho1*pow(vs1,3)/vp2*p*ys1*yp1;

            R_SP/=D;
            R_SS/=D;
            T_SP/=D;

            return {R_PP,R_PS,R_SP,R_SS,T_PP,T_SP};
        }
        else if (mode=="SA") {

            // P as incident.
            double p=sini/vp1,sinj=p*vs1;

            std::complex<double> yp1=cosi/vp1,ys1(0);
            if (sinj<=1) ys1=sqrt(1-sinj*sinj)/vs1;
            else ys1.imag(FreqSign*sqrt(p*p-1.0/vs1/vs1));

            std::complex<double> A;
            A=pow(1.0/vs1/vs1-2*p*p,2)+4.0*p*p*yp1*ys1;

            std::complex<double> R_PP,R_PS;
            R_PP=-pow(1/vs1/vs1-2*p*p,2)+4*p*p*yp1*ys1;
            R_PS=4*vp1/vs1*p*yp1*(1.0/vs1/vs1-2*p*p);

            R_PP/=A;
            R_PS/=A;

            // SV as incident.
            p=sini/vs1;sinj=p*vp1;

            ys1=cosi/vs1;yp1=0;
            if (sinj<=1) yp1=sqrt(1-sinj*sinj)/vp1;
            else yp1.imag(FreqSign*sqrt(p*p-1.0/vp1/vp1));

            A=pow(1.0/vs1/vs1-2*p*p,2)+4.0*p*p*ys1*yp1;

            std::complex<double> R_SP,R_SS;
            R_SP=4*vs1/vp1*p*ys1*(1.0/vs1/vs1-2*p*p);
            R_SS=pow(1/vs1/vs1-2*p*p,2)-4*p*p*yp1*ys1;

            R_SP/=A;
            R_SS/=A;

            return {R_PP,R_PS,R_SP,R_SS};
        }
        else if (mode=="LS") {

            // P as incident.
            double p=sini/vp1,sinj_pp=p*vp2,sinj_ps=p*vs2;

            std::complex<double> yp1=cosi/vp1,yp2(0),ys2(0);
            if (sinj_pp<=1) yp2=sqrt(1-sinj_pp*sinj_pp)/vp2;
            else yp2=sqrt(p*p-1.0/vp2/vp2);

            if (sinj_ps<=1) ys2=sqrt(1-sinj_ps*sinj_ps)/vs2;
            else ys2=sqrt(p*p-1.0/vs2/vs2);

            std::complex<double> a,b,D;
            a=4*rho2*rho2*pow(vs2,4)*p*p*yp2*ys2;
            b=rho2*rho2*pow((1-2*vs2*vs2*p*p),2);
            D=(a+b)*yp1+rho1*rho2*yp2;

            std::complex<double> R_PP,T_PP,T_PS;
            R_PP=(a+b)*yp1-rho1*rho2*yp2;
            T_PP=2*rho1*rho2*(1-2*vs2*vs2*p*p)*vp1/vp2*yp1;
            T_PS=-4*rho1*rho2*vp1*vs2*p*yp1*yp2;

            R_PP/=D;
            T_PP/=D;
            T_PS/=D;

            return {R_PP,T_PP,T_PS};
        }
        else if (mode=="LL"){

            double p=sini/vp1,sinj=p*vp2;

            std::complex<double> yp1=cosi/vp1,yp2(0);

            if (sinj<=1) yp2=sqrt(1-sinj*sinj)/vp2;
            else yp2.imag(FreqSign*sqrt(p*p-1.0/vp2/vp2));

            std::complex<double> D(0);
            D=rho2*yp1+rho1*yp2;

            std::complex<double> R_PP,T_PP;
            R_PP=(rho2*yp1-rho1*yp2)/D;
            T_PP=2*rho1*vp1/vp2*yp1/D;

            return {R_PP,T_PP};
        }
        else if (mode=="LA") return {1};
    }
    else if (cmp=="SH"){

        if (mode=="SS") {

            double p=sini/vs1,sinj=p*vs2;

            std::complex<double> ys1=cosi/vs1,ys2(0);
            if (sinj<=1) ys2=sqrt(1.0-sinj*sinj)/vs2;
            else ys2.imag(FreqSign*sqrt(p*p-1.0/vs2/vs2));

            std::complex<double> A,B;
            A=rho1*vs1*vs1*ys1;
            B=rho2*vs2*vs2*ys2;

            std::complex<double> R_SS,T_SS;
            R_SS=(A-B)/(A+B);
            T_SS=2.0*A/(A+B);

            return {R_SS,T_SS};
        }
        else if (mode=="SA" || mode=="SL") return {1};
    }

    std::cerr <<  "Error in " << __func__ << ": input cmp/mode("
              << cmp << "/" << mode <<") error ..." << std::endl;
    return {};
}

#endif
