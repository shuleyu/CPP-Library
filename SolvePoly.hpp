#ifndef ASU_SOLVEPOLY
#define ASU_SOLVEPOLY

#include<iostream>
#include<vector>
#include<cmath>

/****************************************************************
 * This C++ template return the solution of f(x)=0 using Newton's
 * iterative method.
 *
 * input(s):
 * const vector<T> &C        ----  Coefficients of the polynomial. C[0] is the highest order.
 * const double    &Initial  ----  Initial guess value. Also the returning solution.
 * const double    &tol      ----  Tolerance of error. (ending criteria)
 *
 * return(s):
 * pair<bool,double> ans  ----  {flag,x0}.
 *                              If the result is reliable, flag=true, f(x0)=0.
 *
 * Shule Yu
 * Jan 19 2018
 *
 * Key words: Newton-Raphson iteratin.
 *
 * Note: This is a very simple program and failure conditions are not
 *       carefully examed!
****************************************************************/

template<typename T>
std::pair<bool,double> SolvePoly(const std::vector<T> &C,const double &Initial, const double &tol){

    int n=C.size();
    double abstol=fabs(tol),ans=Initial;

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
            std::cerr <<  "Warning in " << __func__ << ": reaching a f'(x)=0 position ..." << std::endl;
            return {false,ans};
        }

        // Update by x = x0 - f(x0)/f'(x0)
        ans=prev_ans-prev_err/A;

        err=0;
        for (int i=0;i<n;i++) err+=C[i]*pow(ans,n-i-1);

        if (fabs(prev_err)<fabs(err)) {
            std::cerr <<  "Warning in " << __func__ << ": result is not converging ..." << std::endl;
            return {false,0};
        }
    }
    return {true,ans};
}

#endif
