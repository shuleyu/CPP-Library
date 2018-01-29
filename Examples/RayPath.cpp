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

    // ScS at 0 degree, 0 km.
    // taup_time -mod prem -h 0 -ph ScS -deg 0
    double StartDepth=0,EndDepth=2891,TakeOff=0;
    double rayp=(RE-StartDepth)*sin(TakeOff*M_PI/180)/Dvs(StartDepth)/180*M_PI;

//     // S takeoff at 10 degree, 0 km.
//     // taup_time -mod prem -h 0 -ph S -deg 20.9205
//     double StartDepth=0,EndDepth=2891,TakeOff=30;
//     double rayp=(RE-StartDepth)*sin(TakeOff*M_PI/180)/Dvs(StartDepth)/180*M_PI;

    // Use function.
    vector<double> degree,radius;
    auto ans=RayPath(r,v,rayp,StartDepth,EndDepth,degree,radius);

    // outputs.
    ofstream fpout("data/RayPath_out");
    for (size_t i=0;i<degree.size();++i)
        fpout << degree[i] << " " << radius[i] << '\n';
    fpout.close();

    if (radius.back()-(RE-EndDepth)>1e-5) cout << "Ray turns !\n";

    cout << "Rayp      : " << rayp << " rad/sec." << endl;
    cout << "TravelTime: " << ans.first << " sec." << endl;
    cout << "Pursuit   : " << ans.second << " km." << endl;

    return 0;
}
