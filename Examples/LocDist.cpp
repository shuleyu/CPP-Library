#include<iostream>

#include<ASU_tools.hpp>

using namespace std;

int main(){

    double lon1,lat1,r1,lon2,lat2,r2;

    lon1=0;   lat1=0;
    lon2=0;   lat2=0;
    r1=6370.0;r2=6371.0;

    // Use function.
    cout << LocDist(lon1,lat1,r1,lon2,lat2,r2) << endl;

    return 0;
}
