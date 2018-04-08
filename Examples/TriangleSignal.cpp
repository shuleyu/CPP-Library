#include<iostream>
#include<vector>

#include<TriangleSignal.hpp>

using namespace std;

int main(){

    
    // Use function.
//     auto A=TriangleSignal(20,0.2);
    auto A=TriangleSignal(20,2.0);

    for (auto &item:A) cout << item << "  ";
    cout << "\n";

    return 0;
}
