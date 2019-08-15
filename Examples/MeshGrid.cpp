#include<iostream>

#include<MeshGrid.hpp>

using namespace std;

int main(){

    vector<vector<double>> p={
        {0,17,5.5},
        {4,2,1},
    };

    // Use function.
    auto ans=MeshGrid(p,1);

    for (auto &item:ans){
        for (auto &item2:item)
            cout << item2 << " " ;
        cout << '\n';
    }

    return 0;
}
