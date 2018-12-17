#include<iostream>

#include<Lon2360.hpp>

using namespace std;

int main(){

    double Lon=-361.2,dl=0.2;

    // Use function.
    while (Lon<361.2){
        printf("%.12lf\t%.12lf\n",Lon,Lon2360(Lon));
        Lon+=dl;
    }

    return 0;
}
