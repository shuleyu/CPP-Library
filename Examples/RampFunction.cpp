#include<iostream>
#include<cmath>

#include<ASU_tools.hpp>

using namespace std;

int main(){

    double x,c1,c2;

    c1=-M_PI;
    c2=M_PI;

    // Use function.
    for (x=c1-1;x<c2+1;x=x+0.1)
		cout << x << " " << RampFunction(x,c1,c2) << endl;

    return 0;
}
