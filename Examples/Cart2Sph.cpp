#include<iostream>

#include<ASU_tools.hpp>

int main(){

    double r,theta,phi;

//     double x,y,z;
//     x=19.12;
//     y=12.3;
//     z=23.34;

	int x,y,z;
    x=0;
    y=0;
    z=-1;

    // Use function.
    Cart2Sph(x,y,z,r,phi,theta);
    printf("%.10lf\t%.10lf\t%.10lf\n",phi,theta,r);
    return 0;

}
// Matlab benchmark:
// [phi,theta,r]=cart2sph(19.12,12.3,23.34)
// theta = pi/2-theta
