#include<iostream>

#include<UnionIntervals.hpp>

using namespace std;

int main(){

    vector<pair<double,double>> x{{-1.3,-1.0},{-1.2,-0.8},{-0.9,0.5}};

    for (auto &item:x) 
        cout << item.first << " " << item.second << endl;
    cout << endl;

    // Use function.
    auto ans=UnionIntervals(x);

    for (auto &item:ans) 
        cout << item.first << " " << item.second << endl;
    
    return 0;    
}
