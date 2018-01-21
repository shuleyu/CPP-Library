#include<iostream>
#include<vector>

#include<ASU_tools.hpp>

using namespace std;

int main(){

    vector<vector<int>> data;

    data.push_back({1});
    data.push_back({2});
    data.push_back({3});
    data.push_back({4});
    data.push_back({11});
    data.push_back({12});
    data.push_back({13});
    data.push_back({14});
    data.push_back({21});
    data.push_back({22});
    data.push_back({23});
    data.push_back({24});

	auto res=Kmeans(data,3);

	for (size_t i=0;i<data.size();++i)
            cout << data[i][0] << " ";

	cout << endl <<  "=========="  << endl;

	for (auto &item:res) cout << item << " ";

	cout << endl <<  "=========="  << endl;

	return 0;
}
