#ifndef ASU_SOLVEPOLY
#define ASU_SOLVEPOLY

#include<iostream>
#include<vector>
#include<cmath>

/****************************************************************
 * This C++ template return the solution of f(x)=0 using Newton's
 * method.
 *
 * const vector<T> &C  ----  Constants of polynomial. The first one
 *                           represent the highest order.
 * double   &ans       ----  The intial guess value. Also the returning solution.
 * const T2 &tol       ----  Tolerance of error. (end criteria)
 *
 * Shule Yu
 * Jan 19 2018
 *
 * Key words: Newton-Raphson iteratin.
 *
 * Note: This is a very simple program and failure conditions are not
 * examed. Use with caution!
****************************************************************/

template<class T1, class  T2>
bool SolvePoly(const std::vector<T1> &C,double &ans, const T2 &tol){

	int n=C.size();
	double abstol=fabs(tol);

	double prev_ans=ans+abstol*2,err=0,prev_err=0,A;

    // get f(x0).
	for (int i=0;i<n;i++) err+=C[i]*pow(ans,n-i-1);

	while (fabs(prev_ans-ans)>abstol){

		prev_ans=ans;
		prev_err=err;

        // get f'(x0)
		A=0;
		for (int i=0;i<n-1;i++) A+=(n-i-1)*C[i]*pow(ans,n-i-2);

		if (A==0) {
            std::cerr <<  __func__ << "; Warning: reach a f'(x)=0 position ..." << std::endl;
            return false;
        }

        // Update by x = x0 - f(x0)/f'(x0)
		ans=prev_ans-prev_err/A;

		err=0;
		for (int i=0;i<n;i++) err+=C[i]*pow(ans,n-i-1);

		if (fabs(prev_err)<fabs(err)) {
            std::cerr <<  __func__ << "; Warning: result not converged ..." << std::endl;
            return false;
        }
	}
	return true;
}

#endif
