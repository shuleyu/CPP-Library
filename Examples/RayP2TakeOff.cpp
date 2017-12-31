#include<iostream>

#include<ASU_tools.hpp>

using namespace std;

int main(){

    double rayp,depth;

    rayp=14.640;
    depth=500;

	cout << "TakeOff: " << 180/M_PI*RayP2TakeOff(rayp,depth,'S');

    return 0;    
}
