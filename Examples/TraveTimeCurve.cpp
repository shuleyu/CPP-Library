#include<iostream>
#include<string>
#include<vector>
#include<tuple>

#include<TravelTimeCurve.hpp>

using namespace std;

int main(){

    double evdp=500,dtheta=0.05;
    string phase="S";

    auto res=TravelTimeCurve(evdp,phase,dtheta,false);

    for (auto item:res) {
        for (size_t i=0;i<item.first.size();++i)
            cout << item.first[i] << " " << item.second[i] << endl;
        cout << "---" << endl;
    }

    return 0;
}

