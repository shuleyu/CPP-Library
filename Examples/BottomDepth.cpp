#include<iostream>
#include<string>

#include<BottomDepth.hpp>

using namespace std;

int main(){

    double dist=80,evdp=500;
    string phase="S";
    cout << BottomDepth(dist,evdp,phase) << endl;

    return 0;
}

