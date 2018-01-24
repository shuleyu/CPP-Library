#include<iostream>

#include<ASU_tools.hpp>

using namespace std;

int main(){

    double strike=0,dip=90,rake=0;
    double azimuth=0,take_off;

    for (take_off=1;take_off<180;take_off+=15)
        cout << RadiationPattern(strike,dip,rake,azimuth,take_off,"SH") << '\n';

    return 0;
}

