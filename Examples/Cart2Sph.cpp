#include<iostream>

#include<Cart2Sph.hpp>

using namespace std;

int main(){

    double x,y,z;
    x=1;
    y=1;
    z=0;

    // Use function.
    auto ans=Cart2Sph(x,y,z);
    cout << ans[0] << " " << ans[1] << " " << ans[2] << endl;
    return 0;
}
// Matlab benchmark:
// [phi,theta,r]=cart2sph(19.12,12.3,23.34)
// theta = pi/2-theta
