#include<iostream>
#include<vector>

#include<CrossStd2D.hpp>

using namespace std;

int main(){

	vector<vector<double>> s1,s2;

    s1.push_back({0,0,0,0,0});
    s1.push_back({0,1,1,2,0});
    s1.push_back({0,1,2,4,0});
    s1.push_back({0,1,5,2,0});
    s1.push_back({0,0,0,0,0});

    s2.push_back({1,2});
    s2.push_back({2,4});

    // Use function.
	auto res=CrossStd2D(s1,s2);
	cout << "Shift: " << res.first.first << " " << res.first.second << "\n";
	cout << "STD: " << res.second << "\n";

    return 0;
}
