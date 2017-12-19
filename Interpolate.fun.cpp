#include<cstdio>
#include<iostream>
#include<vector>
#include<cmath>
#include<algorithm>
#include<numeric>
extern "C" {
#include<ASU_tools.h>
}
#include<ASU_tools.hpp>

using namespace std;

/**************************************************************
 * This cpp function is a shell for the wiginterp c function.
 *
 * Shule Yu
 * Nov 14 2017
 *
 * Key words: interpolate, wiggins
****************************************************************/

void Interpolate(const vector<pair<double,double>> &input, vector<pair<double,double>> &output,int flag){

	double *x=new double [input.size()];
	double *y=new double [input.size()];
	double *xx=new double [output.size()];
	double *yy=new double [output.size()];

	for (size_t i=0;i<input.size();++i) {
		x[i]=input[i].first;
		y[i]=input[i].second;
	}

	for (size_t i=0;i<output.size();++i) xx[i]=output[i].first;
	wiginterpd(x,y,input.size(),xx,yy,output.size(),flag);
	for (size_t i=0;i<output.size();++i) output[i].second=yy[i];

	delete [] x;
	delete [] y;
	delete [] xx;
	delete [] yy;
	
	return;
}
