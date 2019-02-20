#include<iostream>

#include<CreateGlobeGrid.hpp>

using namespace std;

int main(){

    // Use function.
    double xinc=10,yinc=10;
    auto res=CreateGlobeGrid(xinc,yinc);

    cout << "Requested grid increment: " << xinc << " "  << yinc << endl;
    cout << "Actual grid increment   : " << res.second.first << " "  << res.second.second << endl;
    cout << "Grid size               : " << res.first.size() << " " << endl;


    return 0;
}
