#include<iostream>

#include<DotDist.hpp>

using namespace std;

int main(){

    pair<double,double> p1{0,0},p2{1,1};
    cout << DotDist(p1.first,p1.second,0,p2.first,p2.second,0) << endl;

    return 0;
}
