#include<iostream>
#include<fstream>

#include<CurveJunction.hpp>

using namespace std;

int main(){

    vector<double> x1,x2,y1,y2;
    double x,y;

    ifstream fpin("data/CurveJunction_in1");
    while(fpin >> x >> y){
        x1.push_back(x);
        y1.push_back(y);
    }
    fpin.close();


    fpin.open("data/CurveJunction_in2");
    while(fpin >> x >> y){
        x2.push_back(x);
        y2.push_back(y);
    }
    fpin.close();

    // Use function.
    auto ans=CurveJunction(x1,y1,x2,y2,1e-5);
    cout << ans.first << " " << ans.second << endl;

    return 0;
}
