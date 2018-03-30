#include<iostream>
#include<cmath>

#include<SphAngle.hpp>

using namespace std;

int main(){

    double t1,p1,t2,p2;

    p1=1*M_PI/180;
    t1=89*M_PI/180;

    p2=1*M_PI/180;
    t2=87*M_PI/180;

    // Use function.
    cout << SphAngle(t1,p1,t2,p2)*180/M_PI << endl;

    return 0;
}
