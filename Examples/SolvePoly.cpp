#include<iostream>

#include<SolvePoly.hpp>

using namespace std;

int main(){

    vector<double> A{34.5,-6,7.3,-8,-173235.0051};
    auto res=SolvePoly(A,8,1e-6);

    if (res.first) printf("%.9lf\n",res.second);

    return 0;
}
