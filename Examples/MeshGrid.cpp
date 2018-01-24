#include<iostream>

#include<ASU_tools.hpp>

using namespace std;

int main(){

	vector<vector<double>> p={
        {1,5,5},
        {6,10,5},
        {0.1,0.2,3}
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
