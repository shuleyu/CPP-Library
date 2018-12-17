#include<iostream>
#include<vector>

#include<ConvexPolygon.hpp>

using namespace std;

int main(){

    vector<pair<int,double>> Polygons{{0,0},{1,0},{0.3,0.3},{0,1}};

    // Use function.
    auto res=ConvexPolygon(Polygons);
    cout << (res?"Yes":"No") << endl;

    return 0;
}
