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

    // Velocity layers.
    vector<double> v;
    for (auto &item:r) v.push_back(RvsX(item));
//     for (auto &item:r) v.push_back(Rvs(item));

//     // ScS at 0 degree, 0 km.
//     // taup_time -mod prem -h 0 -ph ScS -deg 0
//     double StartDepth=0,EndDepth=2891,TakeOff=0;
//     double rayp=(RE-StartDepth)*sin(TakeOff*M_PI/180)/Dvs(StartDepth)/180*M_PI;

//     // S takeoff at 30 degree, 0 km, gcp distance at 20.9205 degree.
//     // taup_time -mod prem -h 0 -ph S -deg 20.9205
//     double StartDepth=0,EndDepth=2891,TakeOff=30;
//     double rayp=(RE-StartDepth)*sin(TakeOff*M_PI/180)/Dvs(StartDepth)/180*M_PI;

    // S takeoff at 80 degree, 100 km.
    double StartDepth=100,EndDepth=2891,TakeOff=89;
    double rayp=(RE-StartDepth)*sin(TakeOff*M_PI/180)/DvsX(StartDepth)/180*M_PI;

    // Use function.
    vector<double> degree1,radius1,degree2,radius2; // will output the ray path.
    auto ans1=RayPath(r,v,rayp,StartDepth,EndDepth,degree1,radius1);
    auto ans2=RayPath(r,v,rayp,0,EndDepth,degree2,radius2);

    // outputs.
    ofstream fpout("data/RayPath_out");
    for (size_t i=0;i<degree1.size();++i)
        fpout << degree1[i] << " " << radius1[i] << '\n';
    fpout << endl;
    for (size_t i=degree2.size();i>0;--i)
        fpout << degree1.back()+degree2.back()-degree2[i-1] << " " << radius2[i-1] << '\n';
    fpout.close();

    if (radius1.back()-(RE-EndDepth)>1e-5) cout << "Ray turns !\n";

    cout << "Rayp      : " << rayp << " sec/rad." << endl;
    cout << "TravelTime: " << ans1.first+ans2.first << " sec." << endl;
    cout << "Pursuit   : " << ans1.second+ans2.second << " km." << endl;

    return 0;
}
