#include<iostream>

#include<WayPoint.hpp>

using namespace std;

int main(){

    // Use function.
    auto res=WayPoint(0,0,90,10);
    cout << res.first << " " << res.second << endl;
    return 0;
}
