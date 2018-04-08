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
 * A. For clearance, I didn't ommit any terms (formula is not shortened for cancelling terms).
 *
 * B. Ray always incident from media 1 (parameters related to index 1).
 *
 * C. In Aki & Richard, the meaning of angles (i,j) are related to (P,S) waves in PSV discussion.
 *    Here we use i means incident angle, j (with subscripts, if necessary) to denote takeoff or
 *    reflection angles.
 *
 * D. Interfaces (SS,SA,SL,LS):
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
 * E. input(s):
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
 * F. return(s):
 *
 *     vector<double> ans  ----  displacement coefficients. Value and length depend on cmp and mode:
 *
 *      cmp     mode     ans
 *
 *      PSV     SS       ans.size()=8. R_PP, R_PS, T_PP, T_PS, R_SP, R_SS, T_SP, T_SS.
 *      PSV     SA       ans.size()=4. R_PP, R_PS, R_SP, R_SS.
 *      PSV     SL       ans.size()=6. R_PP, R_PS, T_PP, R_SP, R_SS, T_SP.
 *      PSV     LS       ans.size()=3. R_PP, T_PP, T_PS.
 *      PSV     LL       ans.size()=2. R_PP, T_PP.
 *      PSV     LA       ans.size()=1. ans[0]=R_PP=1.
 *      SH      SS       ans.size()=2. R_SS, T_SS.
 *      SH      SA       ans.size()=1. ans[0]=R_SS=1.
 *      SH      SL       ans.size()=1. ans[0]=R_SS=1.
 *
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

    double sini=sin(inc/180*M_PI),cosi=sqrt(1-sini*sini);

    if (cmp=="PSV") {
        if (mode=="SS") {
            double R_PP,R_PS,T_PP,T_PS,R_SP,R_SS,T_SP,T_SS,p,sinj_r,sinj_pp,sinj_ps,sinj_sp,sinj_ss,cosj_r,cosj_pp,cosj_ps,cosj_sp,cosj_ss;
            double a,b,c,d,E,F,G,H,D,ys1,ys2,yp1,yp2;

            // P as incident.
            p=sini/vp1;
            sinj_r=p*vs1;cosj_r=sqrt(1-sinj_r*sinj_r);
            sinj_pp=p*vp2;cosj_pp=sqrt(1-sinj_pp*sinj_pp);
            sinj_ps=p*vs2;cosj_ps=sqrt(1-sinj_ps*sinj_ps);

            yp1=cosi/vp1;yp2=cosj_pp/vp2;
            ys1=cosj_r/vs1;ys2=cosj_ps/vs2;

            a=rho2*(1-2*vs2*vs2*p*p)-rho1*(1-2*vs1*vs1*p*p);
            b=rho2*(1-2*vs2*vs2*p*p)+2*rho1*vs1*vs1*p*p;
            c=rho1*(1-2*vs1*vs1*p*p)+2*rho2*vs2*vs2*p*p;
            d=2*(rho2*vs2*vs2-rho1*vs1*vs1);

            E=b*yp1+c*yp2;
            F=b*ys1+c*ys2;
            G=a-d*yp1*ys2;
            H=a-d*yp2*ys1;

            D=E*F+G*H*p*p;

            R_PP=(b*yp1-c*yp2)*F-(a+d*yp1*ys2)*H*p*p;
            R_PS=-2*yp1*(a*b+c*d*yp2*ys2)*p*vp1/vs1;
            T_PP=2*rho1*yp1*F*vp1/vp2;
            T_PS=2*rho1*yp1*H*p*vp1/vs2;

            R_PP/=D;
            R_PS/=D;
            T_PP/=D;
            T_PS/=D;

            // SV as incident wave.
            p=sini/vs1;
            sinj_r=p*vp1;cosj_r=sqrt(1-sinj_r*sinj_r);
            sinj_sp=p*vp2;cosj_sp=sqrt(1-sinj_sp*sinj_sp);
            sinj_ss=p*vs2;cosj_ss=sqrt(1-sinj_ss*sinj_ss);

            ys1=cosi/vs1;ys2=cosj_ss/vs2;
            yp1=cosj_r/vp1;yp2=cosj_sp/vp2;

            a=rho2*(1-2*vs2*vs2*p*p)-rho1*(1-2*vs1*vs1*p*p);
            b=rho2*(1-2*vs2*vs2*p*p)+2*rho1*vs1*vs1*p*p;
            c=rho1*(1-2*vs1*vs1*p*p)+2*rho2*vs2*vs2*p*p;
            d=2*(rho2*vs2*vs2-rho1*vs1*vs1);

            E=b*yp1+c*yp2;
            F=b*ys1+c*ys2;
            G=a-d*yp1*ys2;
            H=a-d*yp2*ys1;

            D=E*F+G*H*p*p;

            R_SP=-2*ys1*(a*b+c*d*yp2*ys2)*p*vs1/vp1;
            R_SS=-(b*ys1-c*ys2)*E+(a+d*yp2*ys1)*G*p*p;
            T_SP=-2*rho1*ys1*G*p*vs1/vp2;
            T_SS=2*rho1*ys1*E*vs1/vs2;

            R_SP/=D;
            R_SS/=D;
            T_SP/=D;
            T_SS/=D;

            return {R_PP,R_PS,T_PP,T_PS,R_SP,R_SS,T_SP,T_SS};
        }
        else if (mode=="SA") {

            double R_PP,R_PS,R_SP,R_SS,p,A,sinj,cosj;

            // P as incident wave.
            p=sini/vp1;
            sinj=p*vs1,cosj=sqrt(1-sinj*sinj);
            A=pow(1/vs1/vs1-2*p*p,2)+4*p*p*cosi/vp1*cosj/vs1;

            R_PP=-pow(1/vs1/vs1-2*p*p,2)+4*p*p*cosi/vp1*cosj/vs1;
            R_PS=4*vp1/vs1*p*cosi/vp1*(1/vs1/vs1-2*p*p);
            R_PP/=A;
            R_PS/=A;

            // SV as incident wave.
            p=sini/vs1;
            sinj=p*vp1,cosj=sqrt(1-sinj*sinj);
            A=pow(1/vs1/vs1-2*p*p,2)+4*p*p*cosi/vs1*cosj/vp1;
            R_SP=4*vs1/vp1*p*cosi/vs1*(1/vs1/vs1-2*p*p);
            R_SS=pow(1/vs1/vs1-2*p*p,2)-4*p*p*cosj/vp1*cosi/vs1;
            R_SP/=A;
            R_SS/=A;

            return {R_PP,R_PS,R_SP,R_SS};
        }
        else if (mode=="SL") {

            double R_PP,R_PS,T_PP,R_SP,R_SS,T_SP,p,sinj_r,sinj_pp,sinj_sp,cosj_r,cosj_pp,cosj_sp;
            double a,b,D,ys1,yp1,yp2;

            // P as incident.
            p=sini/vp1;
            sinj_r=p*vs1;cosj_r=sqrt(1-sinj_r*sinj_r);
            sinj_pp=p*vp2;cosj_pp=sqrt(1-sinj_pp*sinj_pp);

            ys1=cosj_r/vs1;
            yp1=cosi/vp1;yp2=cosj_pp/vp2;

            a=4*rho1*rho1*pow(vs1,4)*p*p*yp1*ys1;
            b=rho1*rho1*pow((1-2*vs1*vs1*p*p),2);
            D=(a+b)*yp2+rho1*rho2*yp1;

            R_PP=(a-b)*yp2+rho1*rho2*yp1;
            R_PS=2*rho1*rho1*vp1*vs1*(1-2*vs1*vs1*p*p)*p*yp1*yp2;
            T_PP=2*rho1*rho1*vp1/vp2*(1-2*vs1*vs1*p*p)*yp1;

            R_PP/=D;
            R_PS/=D;
            T_PP/=D;

            // S as incident.
            p=sini/vs1;
            sinj_r=p*vp1;cosj_r=sqrt(1-sinj_r*sinj_r);
            sinj_sp=p*vp2;cosj_sp=sqrt(1-sinj_sp*sinj_sp);
            
            ys1=cosi/vs1;
            yp1=cosj_r/vp1;yp2=cosj_sp/vp2;

            a=4*rho1*rho1*pow(vs1,4)*p*p*yp1*ys1;
            b=rho1*rho1*pow((1-2*vs1*vs1*p*p),2);
            D=(a+b)*yp2+rho1*rho2*yp1;

            R_SP=4*rho1*rho1*pow(vs1,3)/vp1*(1-2*vs1*vs1*p*p)*p*ys1*yp2;
            R_SS=(b-a)*yp2+rho1*rho2*yp1;
            T_SP=-4*rho1*rho1*pow(vs1,3)/vp2*p*ys1*yp1;


            return {R_PP,R_PS,T_PP,R_SP,R_SS,T_SP};
        }
        else if (mode=="LS") {

            // Liquid to solid: only P could be incident wave.
            double R_PP,T_PP,T_PS,p,sinj_pp,sinj_ps,cosj_pp,cosj_ps;
            double a,b,D,ys2,yp1,yp2;

            p=sini/vp1;
            sinj_pp=p*vp2;cosj_pp=sqrt(1-sinj_pp*sinj_pp);
            sinj_ps=p*vs2;cosj_ps=sqrt(1-sinj_ps*sinj_ps);

            ys2=cosj_ps/vs2;
            yp1=cosi/vp1;yp2=cosj_pp/vp2;

            a=4*rho2*rho2*pow(vs2,4)*p*p*yp2*ys2;
            b=rho2*rho2*pow((1-2*vs2*vs2*p*p),2);
            D=(a+b)*yp1+rho1*rho2*yp2;

            R_PP=(a+b)*yp1-rho1*rho2*yp2;
            T_PP=2*rho1*rho2*(1-2*vs2*vs2*p*p)*vp1/vp2*yp1;
            T_PS=-4*rho1*rho2*vp1*vs2*p*yp1*yp2;

            R_PP/=D;
            T_PP/=D;
            T_PS/=D;

            return {R_PP,T_PP,T_PS};
        }
        else if (mode=="LL"){

            double R_PP,T_PP,p,D,sinj,cosj,yp1,yp2;

            p=sini/vp1;
            sinj=p*vp2;cosj=sqrt(1-sinj*sinj);
            yp1=cosi/vp1;yp2=cosj/vp2;

            D=rho2*yp1+rho1*yp2;
            R_PP=(rho2*yp1-rho1*yp2)/D;
            T_PP=2*rho1*vp1/vp2*yp1/D;

            return {R_PP,T_PP};
        }
        else if (mode=="LA") return {1};
        else {
            std::cerr <<  "Error in " << __func__ << ": input cmp/mode("
                      << cmp << "/" << mode <<") error ..." << std::endl;
            return {};
        }
    }
    else if (cmp=="SH"){
        if (mode=="SS") {
            double sinj=sini/vs1*vs2,cosj=sqrt(1-sinj*sinj);
            double A=rho1*vs1*cosi,B=rho2*vs2*cosj;
            return {(A-B)/(A+B),2*A/(A+B)};
        }
        else if (mode=="SA" || mode=="SL") return {1};
        else {
            std::cerr <<  "Error in " << __func__ << ": input cmp/mode("
                      << cmp << "/" << mode <<") error ..." << std::endl;
            return {};
        }
    }
    else {
        std::cerr <<  "Error in " << __func__ << ": input cmp/mode("
                  << cmp << "/" << mode <<") error ..." << std::endl;
        return {};
    }
}

#endif
