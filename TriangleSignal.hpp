#ifndef ASU_TRIANGLESIGNAL
#define ASU_TRIANGLESIGNAL

#include<vector>
#include<ASU_tools.hpp>

/***********************************************************
 * This C++ template returns a triangle signal.
 * The signal has peak at its center, amplitude is 1.
 *
 * vector<T> &p        ----  the output signal.
 * const int &N        ----  request signal length.
 * double    frac  ----  request fraction of the non-zero part
 *                           (explaned below)
 *
 *                      *
 *                     * *
 *                    *   *
 *                   *     *
 *        00000000000       00000000000
 *
 *		  frac = 7/N = 7/29 = 0.24137931
 *
 *
 * Shule Yu
 * Dec 31 2017
 *
 * Key words: triangle signal.
***********************************************************/

template<class T>
void TriangleSignal(std::vector<T> &p,const int &N,double frac){

	p.clear();

	// Check requested size.
	if (N<=0) return;
	p.resize(N,0);

	// Check signal width.
	if (frac<0) frac=0; // gives an impulse if frac<=0.

	int L=N*frac;
	T Inc=-2.0/(L+1);
    p[N/2]=1;

    for (int i=N/2+1;i<N;i++){
        p[i]=p[i-1]+Inc;
		if (p[i]<0) p[i]=0;
        p[N/2-(i-N/2)]=p[i];
    }
	p[0]=p[1]+Inc;
	if (p[0]<0) p[0]=0;

	return;
}

#endif
