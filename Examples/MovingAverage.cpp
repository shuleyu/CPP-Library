#include<iostream>

#include<MovingAverage.hpp>

using namespace std;

int main(){

    vector<double> A{1,2,3,4};
    auto ans=MovingAverage(A,5);

    for (auto item:ans) cout << item << ' ';
    cout << '\n';

    return 0;
}
