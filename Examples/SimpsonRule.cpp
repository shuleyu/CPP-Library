#include<iostream>
#include<cmath>

#include<SimpsonRule.hpp>
#include<CreateGrid.hpp>

using namespace std;

inline double g(const double &x){
    return exp(x);
}

int main(){

    double h=0.01;
    vector<double> A{1};
    for (int i=0;i<10001;++i) A.push_back(A.back()+h);

    auto f=[](const double &s1){return (s1>0?s1:-s1);};
    cout << SimpsonRule(A.begin(),A.end(),h,f) << endl;

    for (double h=1e-5;h<=0.1;h*=10){
        cout << "Interval: " << h << endl;
        double res=SimpsonRule(0,10.0,h,g); // integral exp(x).
                                            // from x=0 to x=10 using h as interval.
        cout << "Error   : " << (1-(res+exp(0))/exp(10))*100 << "%\n";
    }

    return 0;
}
