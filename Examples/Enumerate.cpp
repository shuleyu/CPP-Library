#include<iostream>
#include<vector>

#include<Enumerate.hpp>

using namespace std;

int main(){

//     vector<size_t> p{2,4,2,5,0};

    vector<double> a{1.1,2.1},b{1.2,2.2,3.2,4.2};
    vector<vector<double>> p{a,b};

    // Use function.
    auto res=Enumerate(p);
    for (auto &item:res) {
        for (auto &item2:item)
            cout << item2 << " ";
        cout << endl;
    }

    return 0;
}
