#ifndef ASU_SHCOEFFICIENTS
#define ASU_SHCOEFFICIENTS

#include<cmath>

#include<PREM.hpp>

/****************************************************
 * This C++ template returns SH component solid-solid
 * reflection and transmission coefficients.
 *
 * const T1 &rho1  ----  Density of first layer.
 * const T2 &vs1   ----  S-wave velocity of first layer.
 * const T3 &rho2  ----  Density of second layer.
 * const T4 &vs2   ----  S-wave velocity of second layer.
 * const T5 &px    ----  Horizontal slowness defined as sin(i)/vs1;
 *                       i is the incident angle.
 *
 * Shule Yu
 * Jan 16 2018
 *
 * Key words: reflection, transmission, coefficient.
 *
 * Reference: Thorne Lay and Terry C. Wallace
 *            Modern Global Seismology, P98. Table 3.1
****************************************************/

template <class T1, class T2, class T3, class T4, class T5>
std::pair<double,double> SHCoefficients(const T1 &rho1,const T2 &vs1 ,const T3 &rho2,const T4 &vs2,const T5 &px){

	double A=rho1*vs1*sqrt(1-pow(vs1*px,2)),B=rho2*vs2*sqrt(1-pow(vs2*px,2));
	return {(A-B)/(A+B),2*A/(A+B)};
}

#endif
