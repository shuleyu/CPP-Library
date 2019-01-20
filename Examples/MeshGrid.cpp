#include<iostream>

#include<MeshGrid.hpp>

using namespace std;

int main(){

    vector<vector<double>> p={
        {0,10,5},
        {2,4,5},
        {1,5,2}
    };

    // Use function.
    auto ans=MeshGrid(p,0);

    for (auto &item:ans){
        for (auto &item2:item)
            cout << item2 << " " ;
        cout << '\n';
    }

    return 0;
}
