#include<iostream>
#include<string>

#include<BottomLocation.hpp>

using namespace std;

int main(){

    double evdp=496.8;
    string phase="S";

    auto ans=BottomLocation(179.919,-24.799,evdp,-135.329,59.4601,phase);
    cout << "Depth: " << ans[0] << endl;
    cout << ans[1] << " " << ans[2] << endl;

    return 0;
}

