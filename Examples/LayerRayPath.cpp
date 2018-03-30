#include<iostream>
#include<fstream>
#include<vector>
#include<cmath>
#include<algorithm>

#include<CreateGrid.hpp>
#include<LayerRayPath.hpp>
#include<PREM.hpp>

using namespace std;

int main(int argc, char **argv){

    // set up velocity and depth grid.

    double dz=0.05,RE=6371.0;
    auto z=CreateGrid(0,RE-1,dz,2);
    vector<double> v;
    for (auto &depth:z) {

		// apply earth-flattening here.
        double x=RE/(RE-depth);
        v.push_back(DvsX(depth)*x);
        depth=RE*log(x);
    }

    // Example, S arrival for 500km source, slowness of 1/7.0 (phase velocity: 7km/sec)
    double SourceDepth=500,EndDepth=2891,rayp=1/7.0;
    EndDepth=RE*log(RE/(RE-EndDepth));
    SourceDepth=RE*log(RE/(RE-SourceDepth));

    // Use function, first half: from source to turning point.
    vector<double> X;
	size_t D;
    auto ans1=LayerRayPath(z,v,rayp,SourceDepth,EndDepth,X,D);

    // second half: from turning point ot surface.
    vector<double> XX;
	size_t DD;
    auto ans2=LayerRayPath(z,v,rayp,0,EndDepth,XX,DD);

    // output.
    ofstream fpout("data/LayerRayPath_out");
    for (size_t i=0;i<X.size();++i)
        fpout << X[i] << " " << z[i+D-X.size()+1] << '\n';
    fpout << endl;
    for (size_t i=XX.size();i>0;--i)
        fpout << X.back()+XX.back()-XX[i-1] << " " << z[DD+i-XX.size()] << '\n';
    fpout.close();

    if (ans1.second || ans2.second) cout << "Ray turns !\n";
    cout << "Rayp      : " << rayp << " sec/km." << endl;
    cout << "X distance: " << X.back()+XX.back() << " km. (" << (X.back()+XX.back())/111.195 << " deg.)" << endl;
    cout << "Max Depth : " << z[D] << " " << z[DD] << " km." << endl;
    cout << "TravelTime: " << ans1.first.first+ans2.first.first << " sec." << endl;
    cout << "Pursuit   : " << ans1.first.second+ans2.first.second << " km." << endl;

    return 0;
}
