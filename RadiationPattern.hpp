#ifndef ASU_RADIATIONPATTERN
#define ASU_RADIATIONPATTERN

#include<iostream>
#include<cmath>
#include<string>

/*************************************************************
 * This C++ template calculate the radiation pattern amplitude
 * for given fault orientation and ray path orientation on
 * the lower hamisphere.
 *
 * input(s):
 * const T1 &strike    ----  Falut strike. (in deg, 0~360)
 * const T2 &dip       ----  Falut dip. (in deg, 0~90)
 * const T3 &rake      ----  Falut rake.  (in deg, -180~180)
 * const T4 &az        ----  Ray path azimuth. (in deg)
 * const T5 &take_off  ----  Ray path take-off angle. (in deg, 0~90)
 * const string &Cmp   ----  Chosen component. ("P","SV" or "SH").
 *
 * return(s):
 * double ans  ----  The radiation pattern intensity.
 *
 * Shule Yu
 * Jan 23 2018
 *
 * Key words: radiation pattern.
 *
 * Reference:
 *     Aki & Richard 2002, Page 108~109.
*************************************************************/

template<class T1, class T2, class T3, class T4, class T5>
double RadiationPattern(const T1 &strike, const T2 &dip, const T3 &rake, const T4 &az,
                        const T5 &take_off,const std::string &Cmp){

    // Convert parameters into rad.
    double s,d,r,a,t;
    s=strike*M_PI/180;
    d=dip*M_PI/180;
    r=rake*M_PI/180;
    a=az*M_PI/180;
    t=take_off*M_PI/180;

    // Calculate.
    double ans;
    if (Cmp=="P"){

        ans=cos(r)*sin(d)*pow(sin(t),2)*sin(2*(a-s))-
            cos(r)*cos(d)*sin(2*t)*cos(a-s)+
            sin(r)*sin(2*d)*(pow(cos(t),2)-pow(sin(t),2)*pow(sin(a-s),2))+
            sin(r)*cos(2*d)*sin(2*t)*sin(a-s);

        return ans;
    }

    if (Cmp=="SV"){

        ans=sin(r)*cos(2*d)*cos(2*t)*sin(a-s)-
            cos(r)*cos(d)*cos(2*t)*cos(a-s)+
            0.5*cos(r)*sin(d)*sin(2*t)*sin(2*(a-s))-
            0.5*sin(r)*sin(2*d)*sin(2*t)*(1+pow(sin(a-s),2));

        return ans;
    }

    if (Cmp=="SH"){

        ans=cos(r)*cos(d)*cos(t)*sin(a-s)+
            cos(r)*sin(d)*sin(t)*cos(2*(a-s))+
            sin(r)*cos(2*d)*cos(t)*cos(a-s)-
            0.5*sin(r)*sin(2*d)*sin(t)*sin(2*(a-s));

        return ans;
    }

    std::cerr <<  "Error in " << __func__ << ": Component error (must be \"P\",\"SV\" or \"SH\") ... " << std::endl;

    return -1;
}

#endif
