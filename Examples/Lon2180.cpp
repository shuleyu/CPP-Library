#include<iostream>

#include<Lon2180.hpp>

using namespace std;

int main(){

    double Lon=-180,dl=0.2;

    // Use function.
    cout << Lon << " " << Lon2180(Lon) << endl;

    return 0;
}
