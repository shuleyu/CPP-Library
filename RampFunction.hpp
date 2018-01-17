#ifndef ASU_RAMPFUNCTION
#define ASU_RAMPFUNCTION

#include<iostream>

/***********************************************************
 * This C++ template returns the value of a ramp function
 * defined by given parameters:
 *
 * f(x) =   0   x<=c1;
 *          1   x>=c2;
 *          linear rise from 0 to 1 while x from c1 to c2.
 *
 * const T1 &x   ----  x.
 * const T1 &c1  ----  left boundary.
 * const T1 &c2  ----  right boundary.
 *
 * Shule Yu
 * Dec 31 2017
 *
 * Key words: ramp function.
***********************************************************/

template<class T1, class T2, class T3>
double RampFunction(const T1 &x, const T2 &c1, const T3 &c2){

    if (c1>c2){ // check range.
		std::cerr <<  __func__ << "; Error: range c1 > c2 ..." << std::endl; 
		return 0;
    }
	else if (c1==c2){
		if (x>=c2) return 1.0;
		else return 0.0;
	}
	else{
		if (x>=c2) return 1.0;
		if (x<=c1) return 0.0;
		return 1.0*(x-c1)/(c2-c1);
	}
}

#endif
