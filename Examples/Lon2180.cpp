#include<iostream>

#include<Lon2180.hpp>

using namespace std;

int main(){

    double Lon=-361.2,dl=0.2;

    // Use function.
    while (Lon<361.2){
        cout << Lon << " " << Lon2180(Lon) << endl;
        Lon+=dl;
    }

    return 0;
}
