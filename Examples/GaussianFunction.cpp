#include<iostream>
#include<cmath>

#include<ASU_tools.hpp>

using namespace std;

int main(){

    double x,sigma=1,mu=0;

    x=sqrt(2*log(2)*sigma);

    // Use function.
    cout << GaussianFunction(x,sigma,mu)/GaussianFunction(0,sigma,mu) << endl;
    cout << GaussianFunction(0,sigma,mu) << endl;
    cout << 1/sqrt(2*M_PI)/sigma << endl;
    cout << GaussianFunction(0.5,sigma,mu) << endl;

    return 0;
}
// Matlab Benchmark:
// x=-9.1247;
// gaussmf(x,[10,0]);
