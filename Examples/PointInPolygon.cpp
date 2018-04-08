#include<iostream>
#include<fstream>
#include<vector>

#include<PointInPolygon.hpp>

using namespace std;

int main(){

    vector<pair<double,double>> Polygon;
    pair<double,double> p;

    ifstream fpin("data/PointInPolygon_polygon");
    while(fpin >> p.first >> p.second) Polygon.push_back(p);
    fpin.close();

    fpin.open("data/PointInPolygon_points");
    ofstream fpout("data/PointInPolygon_result");
    ofstream fpout1("data/PointInPolygon_result1");
    ofstream fpout2("data/PointInPolygon_result-1");
    bool res;
    int i=0;
    while(fpin >> p.first >> p.second) {

        // Use function.
        // By default, left and upper boundaries are excluded.
        res=PointInPolygon(Polygon,p);
        fpout << (res?"*":"o");

        res=PointInPolygon(Polygon,p,1);
        fpout1 << (res?"*":"o");

        res=PointInPolygon(Polygon,p,-1);
        fpout2 << (res?"*":"o");

        if (i%51==50) {
            fpout << '\n';
            fpout1 << '\n';
            fpout2 << '\n';
        }
        ++i;
    }
    fpin.close();
    fpout.close();
    fpout1.close();
    fpout2.close();

    pair<double,double> Point{1,1};
    Polygon={{0,0},{0,2}};
    res=PointInPolygon(Polygon,Point);
    cout << Point.first << " " <<  Point.second << (res?" In":" Out") << endl;

    return 0;    
}
