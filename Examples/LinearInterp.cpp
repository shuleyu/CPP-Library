#include<iostream>

#include<ASU_tools.hpp>

using namespace std;

int main(){

    vector<int> p{1,2,3,4,5,6,7,8};
    vector<double> pp{0.485375648722841,0.957166948242946,0.800280468888800};

    // Use function.
    printf("%.15lf\n",LinearInterp(p,pp));

    return 0;
}
// Matlab Benchmark:
// x=[0,1];
// a=linspace(1,8,8);
// V=reshape(a,2,2,2);
// interp3(x,x,x,V,0.485375648722841,0.957166948242946,0.800280468888800,'linear')
