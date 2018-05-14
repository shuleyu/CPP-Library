#include<iostream>

#include<CreateGrid.hpp>

using namespace std;

int main(){

    int NPTS=10,mode=1,Lowerbound=0,Upperbound=10;
    double Inc=0.05,U;

    // Use function mode 0.
//     mode=0;
//     auto res=CreateGrid(Lowerbound,Upperbound,NPTS,mode);
//     if (!res.empty()) Inc=res[1]-res[0];
//     NPTS=res.size();
//     U=res.back();


    // Use function mode 1.
    mode=1;
    auto res=CreateGrid(Lowerbound,Upperbound,Inc,mode);
    if (!res.empty()) Inc=res[1]-res[0];
    NPTS=res.size();
    U=res.back();

    // Use function mode -1.
//     mode=-1;
//     auto res=CreateGrid(Lowerbound,Upperbound,Inc,mode);
//     NPTS=res[0];
//     U=res[1];

    // Use function mode 2.
//     mode=2;
//     auto res=CreateGrid(Lowerbound,Upperbound,Inc,mode);
//     if (!res.empty()) Inc=res[1]-res[0];
//     NPTS=res.size();
//     U=res.back();

//     // Use function mode -1.
//     mode=-2;
//     auto res=CreateGrid(Lowerbound,Upperbound,Inc,mode);
//     NPTS=res[0];
//     Inc=res[1];
//     U=Upperbound;


    for (auto &item: res) cout << item << '\n';
    cout << "Lowerbound: " << Lowerbound << '\n';
    cout << "UpperBound: " << U << '\n';
    cout << "Increment : " << Inc << '\n';
    cout << "NPTS      : " << NPTS << '\n';

    return 0;
}
