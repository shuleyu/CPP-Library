#include<iostream>
#include<vector>
#include<algorithm>
#include<tuple>
#include<cmath>

#include<ASU_tools.hpp>

using namespace std;

/**************************************************************
 * This C function return 
 * and transmission coefficients.
 *
 * Inputs:
 * 
 * double rho1,vs1,rho2,vs2.  ----  Properties of top(1) and bottom(2) layer.
 * double px.                 ----  Horizontal slowness defined as sin(i)/vs1;
 *
 * Outputs:
 *
 * vector<double> ans(2).        ----  ans[0] is the reflection coefficient.
 *                                     ans[1] is the transmission coefficient.
 * 
 * Shule Yu
 * Sept 14 2017
 *
 * Key words: reflection, transmission, coefficient.
 *
 * Reference: Thorne Lay and Terry C. Wallace
 *            Modern Global Seismology, P98. Table 3.1
****************************************************************/

vector<double> SH_Coeff(double rho1, double vs1, double rho2, double vs2, double px){
	double A=rho1*vs1*sqrt(1-pow(vs1*px,2)),B=rho2*vs2*sqrt(1-pow(vs2*px,2));
	vector<double> ans(2);
	ans[0]=(A-B)/(A+B);
	ans[1]=2*A/(A+B);
	return ans;
}
