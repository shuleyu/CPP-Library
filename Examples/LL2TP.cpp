#include<iostream>

#include<ASU_tools.hpp>

using namespace std;

int main(){

    double lon,lat;

	lat=40;

    // Use function.

    for (int i=0;i<7222;++i){
        lon=-361+i*722.0/7220;
        auto res=LL2TP(lon,lat);
		cout << lon << " " << res.second*180/M_PI << " " << lat << " " << res.first*180/M_PI << "\n";
    }

    return 0;
}
