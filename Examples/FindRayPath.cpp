#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>

#include<ASU_tools.hpp>

using namespace std;

int main(int argc, char **argv){


    double dr=0.05,RE=6371.0;

    // set up grids.
    auto r=CreateGrid(3479,6371,dr,2);
    reverse(r.begin(),r.end());
    vector<double> v;
    for (auto &item:r) v.push_back(Rvs(item));

    // Finding ScS at 64 degree, 350 km.
    // taup_time -mod prem -h 350 -ph ScS -deg 64
    // where is the source side bouncing point distance?
    double StartDepth=350,EndDepth=2891,ll=0,rr=64,mid,Dist=64;

    // Use function.
    double rayp1=0,rayp2=1;
    while (fabs(ll-rr)>1e-4) {
        mid=ll+(rr-ll)/2;
        rayp1=FindRayPath(r,v,RE-StartDepth,RE-EndDepth,mid);
        rayp2=FindRayPath(r,v,RE-0,RE-EndDepth,Dist-mid);

        if (rayp1>rayp2) rr=mid;
        else ll=mid;
    }

    // outputs.
    cout << "Rayp      : " << (rayp1+rayp2)/2 << " rad/sec." << endl;
    cout << "SourceDist: " << mid << " deg." << endl;

    return 0;
}
