#include<iostream>

#include<CMT2Sph.hpp>

using namespace std;

int main(){

    double strike,dip,rake;
    strike=45;
    dip=90;
    rake=0;

    // Use function.
    auto res=CMT2Sph(strike,dip,rake);
    cout << "Azimuth: " << res.first << "\n";
	cout << "Theta: " << res.second << endl;

    return 0;
}
