#include<iostream>
#include<vector>

#include<AvrStd.hpp>
#include<ConfidenceInterval.hpp>

using namespace std;

int main(){

    vector<double> p{1.2944,-0.3362,1.7143,2.6236,0.3082,1.8580,2.2540,-0.5937,-0.4410,1.5711};

    // Use function.
    cout << ConfidenceInterval(p,95) << endl;
    cout << AvrStd(p).second << endl;

    return 0;    
}
