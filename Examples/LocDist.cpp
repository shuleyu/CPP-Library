#include<iostream>

#include<ASU_tools.hpp>

using namespace std;

int main(){

    // Use function.
    cout << LocDist(0,89,6371,0,90,6371) << endl;
    cout << LocDist(0,0,6371,0,1,6371) << endl;
    cout << endl;
    cout << LocDist(0,89,6371,1,89,6371) << endl;
    cout << LocDist(0,0,6371,1,0,6371) << endl;




    return 0;
}
