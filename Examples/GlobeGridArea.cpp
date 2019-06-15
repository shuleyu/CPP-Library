#include<iostream>
#include<fstream>

#include<GlobeGridArea.hpp>

using namespace std;

int main(){

    // A 1x1 gird at r=3480 km, center latitude is 0 deg.
    cout << GlobeGridArea(3480,0,1,1) << endl;

    return 0;
}
