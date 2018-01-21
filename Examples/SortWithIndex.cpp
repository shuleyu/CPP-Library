#include<iostream>
#include<functional>

#include<ASU_tools.hpp>

using namespace std;

int main(){

	// Example 1.

// 	vector<pair<int,int>> A{
// 		{1,9},
// 		{2,8},
// 		{3,7},
// 		{4,6}
// 	};
// 
// 	for (size_t i=0;i<A.size();++i)
// 		cout << A[i].first << " " << A[i].second << " Original Index: "  << i << "\n";
// 	cout << endl;
// 
// 	// Use template.
// 	// Define comparison: ascending of the second element in pairs.
// 	auto cmp=[](const pair<int,int> &p1,const pair<int,int> &p2){
// 		return p1.second<p2.second;
// 	};
// 	auto res=SortWithIndex(A.begin(),A.end(),cmp);
// 
// 	for (size_t i=0;i<A.size();++i)
// 		cout << A[i].first << " " << A[i].second << " Original Index: "  << res[i] << "\n";
// 	cout << endl;

	// Example 2.
	
	vector<double> A{1.3,2.4,1.2,3.1,5.1,6.3,4.5,6.4,5.6};

	for (size_t i=0;i<A.size();++i)
		cout << A[i] << " Original Index: "  << i << "\n";
	cout << endl;

	auto res=SortWithIndex(A.begin(),A.end(),std::greater<double>());

	for (size_t i=0;i<A.size();++i)
		cout << A[i] << " Original Index: "  << res[i] << "\n";
	cout << endl;

	return 0;
}

