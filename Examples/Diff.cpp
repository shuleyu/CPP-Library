#include<iostream>
#include<vector>

#include<Diff.hpp>

using namespace std;

int main(){

	vector<int> p{4,2,1};

    // Use function.
    auto ans=Diff(p);

	for (auto &item: ans)
		cout << item << " ";
	cout << endl;

    return 0;    
}
