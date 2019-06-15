#include<iostream>

#include<CreateGlobeGrid.hpp>

using namespace std;

int main(){

    // Use function.
    double xinc=10,yinc=10;
    auto res=CreateGlobeGrid(xinc,yinc);

    cout << "Requested grid increment: " << xinc << " "  << yinc << endl;
    cout << "Actual grid increment   : " << res.second[2] << " "  << res.second[3] << endl;
    cout << "Grid X/Y size           : " << res.second[0] << " "  << res.second[1] << endl;
    cout << "Grid total size         : " << res.first.size() << " " << endl;

    cout << endl ;
    cout << res.first[0][0] << " " << res.first[0][1] << endl;
    cout << res.first[1][0] << " " << res.first[1][1] << endl;
    cout << "..." << endl;
    cout << res.first[res.first.size()-2][0] << " " << res.first[res.first.size()-2][1] << endl;
    cout << res.first[res.first.size()-1][0] << " " << res.first[res.first.size()-1][1] << endl;



    return 0;
}
