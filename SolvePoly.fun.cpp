#include<iostream>
#include<vector>
#include<cmath>

/**************************************************************
 * This C function return the solution of f(x)=0 using Newton's
 * method.
 *
 * vector<double> C  ----  Constants of polynomial. The first one
 *                         represent the highest order.
 * double &ans       ----  The intial guess value. Also the root.
 * double tol        ----  Tolerance of error. (end criteria)
 *
 *
 * Shule Yu
 * Jul 20 2017
 *
 * Key words: Newton-Raphson iteratin.
 *
 * Note: This is a very simple program and failure conditions are not
 * examed. Use with caution!
****************************************************************/

bool SolvePoly(const std::vector<double> &C,double &ans, double tol){

	bool flag=true;
	int n=C.size();
	tol=fabs(tol);

	double prev_ans=ans+tol*2,err=0,prev_err=tol*2;
	for (int i=0;i<n;i++) err+=C[i]*pow(ans,n-i-1);
	
	while (fabs(prev_ans-ans)>tol){

		prev_ans=ans;
		prev_err=err;

		double A=0;
		for (int i=0;i<n-1;i++) A+=(n-i-1)*C[i]*pow(ans,n-i-2);
		if (A==0) flag=false;

		ans=prev_ans-prev_err/A;

		err=0;
		for (int i=0;i<n;i++) err+=C[i]*pow(ans,n-i-1);

		if (fabs(prev_err)<fabs(err)){
			flag=false;
			break;
		}
	}
	return flag;
}
