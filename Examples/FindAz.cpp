#include<iostream>

#include<ASU_tools.hpp>

using namespace std;

int main(){

    double lo1,la1,lo2,la2;

    lo1=0;
    la1=0;

    lo2=0;
    la2=-35;

    // Use function.
    cout << FindAz(lo1,la1,lo2,la2) << endl;
    return 0;
}
