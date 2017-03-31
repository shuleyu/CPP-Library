#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

template <typename T> vector<size_t> sort_indexes(const vector<T> &v) {

	// initialize original index locations.
	vector<size_t> idx;
	for (size_t index=0;index<v.size();index++) idx.push_back(index);

	// sort indexes based on comparing values in v.
	auto f=[&v](size_t i1, size_t i2) {return v[i1] < v[i2];};
	sort(idx.begin(), idx.end(),f);

	return idx;
}

int main(){
	vector<double> A{6,5,4,4,3,3,2,1};
	auto Index=sort_indexes(A);
	for (auto item:Index)
		cout << item << endl;
	return 0;
}
