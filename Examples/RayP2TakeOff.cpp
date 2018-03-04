#include<iostream>
#include<cmath>

#include<ASU_tools.hpp>

using namespace std;

int main(){

    double rayp,depth;

    rayp=834.298;
    depth=500;

	cout << "TakeOff: " << RayP2TakeOff(rayp,depth,'S') << endl;

    return 0;    
}
