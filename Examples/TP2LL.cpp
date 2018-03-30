#include<iostream>
#include<cmath>

#include<TP2LL.hpp>

using namespace std;

int main(){

    double t,p;

    p=181*M_PI/180;
    t=91*M_PI/180;

    // Use function.
    auto res=TP2LL(t,p);

	cout << res.first << " " << res.second << endl;

    return 0;
}
