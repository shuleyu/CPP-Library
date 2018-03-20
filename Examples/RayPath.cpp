#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>

#include<ASU_tools.hpp>

using namespace std;

int main(int argc, char **argv){

    // set up velocity and radius grid.

    double dr=0.05,RE=6371.0;
    auto r=CreateGrid(3479,6371,dr,2);
    reverse(r.begin(),r.end());
    vector<double> v;

    for (auto &item:r) v.push_back(RvsX(item));

    // Example, ScS arrival for 500km source.  =============
    double StartDepth=500,EndDepth=2891,TakeOff=54;
    double rayp=(RE-StartDepth)*sin(TakeOff*M_PI/180)/DvsX(StartDepth)/180*M_PI;

    // Use function.
    vector<double> degree1{-1e6},degree2{-1e6};
	size_t radius1,radius2;
    auto ans1=RayPath(r,v,rayp,StartDepth,EndDepth,degree1,radius1);
    auto ans2=RayPath(r,v,rayp,0,EndDepth,degree2,radius2);

    // outputs.
    ofstream fpout("data/RayPath_out");
    for (size_t i=0;i<degree1.size();++i)
        fpout << degree1[i] << " " << r[i+(radius1+1-degree1.size())] << '\n';
    fpout << endl;
    for (size_t i=degree2.size();i>0;--i)
        fpout << degree1.back()+degree2.back()-degree2[i-1] << " " << r[radius2+i-degree2.size()] << '\n';
    fpout.close();

    if (ans1.second || ans2.second) cout << "Ray turns !\n";

    cout << "Rayp      : " << rayp << " sec/deg. (" << rayp/111.195 << " sec/km.)"  << endl;
    cout << "X distance: " << (degree1.back()+degree2.back()) << " deg." << endl;
    cout << "Max Depth : " << RE-r[radius1] << " " << RE-r[radius2] << " km." << endl;
    cout << "TravelTime: " << ans1.first.first+ans2.first.first << " sec." << endl;
    cout << "Pursuit   : " << ans1.first.second+ans2.first.second << " km." << endl;
	// ====================================================
	
//     double StartDepth=0,EndDepth=2891,TakeOff=19.5;
//     double rayp=(RE-StartDepth)*sin(TakeOff*M_PI/180)/DvsX(StartDepth)/180*M_PI;
//     vector<double> degree{-1e6};
//     int radius;
//     auto ans=RayPath(r,v,rayp,StartDepth,EndDepth,degree,radius);
//     cout << "X distance: " << degree.back() << " deg." << endl;
//     cout << "Rayp      : " << rayp << " sec/deg. (" << rayp/111.195 << " sec/km.)"  << endl;

    return 0;
}
