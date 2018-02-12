#include<iostream>
#include<vector>

#include<ASU_tools.hpp>

using namespace std;

int main(){

    vector<vector<int>> data;

    data.push_back({1,1});
    data.push_back({2,3});
    data.push_back({3,2});
    data.push_back({2,5});
    data.push_back({-1,4});
    data.push_back({-3,2});
    data.push_back({-4,1});
    data.push_back({-3,4});
    data.push_back({-4,-5});
    data.push_back({-2,-4});
    data.push_back({-3,-2});
    data.push_back({-1,-1});

	auto res=Kmeans(data,3);

	for (size_t i=0;i<data.size();++i) {
        cout << "Catergory: " << res[i] << ": ";
        for (size_t j=0;j<data[j].size();++j) 
            cout << data[i][j] << " ";
        cout << endl;
    }

	return 0;
}
