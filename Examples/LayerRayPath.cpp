#include<iostream>
#include<fstream>
#include<vector>
#include<cmath>
#include<algorithm>

#include<ASU_tools.hpp>

using namespace std;

int main(int argc, char **argv){


    double dz=0.25,RE=6371.0;

    // set up grids.
    auto z=CreateGrid(0,RE,dz,2);
    vector<double> v;
    for (auto &item:z) {
        double x=RE/(RE-item);
        v.push_back(DvsX(item)*x);
        item=RE*log(x);
    }

//     // S @ source=500 km.
//     double SourceDepth=500,EndDepth=2891,TakeOff=47.95;
    double SourceDepth=500,EndDepth=2891,TakeOff=46.57;
    double x=RE/(RE-SourceDepth);
    vector<double> X{-1e6},D{-1e6};
    double rayp=sin(TakeOff*M_PI/180)/Dvs(SourceDepth)/x;
    SourceDepth=RE*log(x);

    // Use function, first half.
    auto ans1=LayerRayPath(z,v,rayp,SourceDepth,EndDepth,X,D);

    // second half.
    vector<double> XX{-1e6},DD{-1e6};
    auto ans2=LayerRayPath(z,v,rayp,0,EndDepth,XX,DD);

    // outputs.
    ofstream fpout("data/LayerRayPath_out");
    for (size_t i=0;i<X.size();++i)
        fpout << X[i] << " " << D[i] << '\n';
    fpout << endl;
    for (size_t i=XX.size();i>0;--i)
        fpout << XX[i-1] << " " << DD[i-1] << '\n';
    fpout.close();

    if (D.back()<EndDepth) cout << "Ray turns !\n";
    cout << "Rayp      : " << rayp << " sec/km." << endl;
    cout << "X distance: " << (X.back()+XX.back())/111.195 << " deg." << endl;
    cout << "Max Depth : " << D.back() << " " << DD.back() << " km." << endl;
    cout << "TravelTime: " << ans1.first+ans2.first << " sec." << endl;
    cout << "Pursuit   : " << ans1.second+ans2.second << " km." << endl;

    cout << endl << RE*log(RE/(RE-1022)) << endl;

    return 0;
}
