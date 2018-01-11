#include<iostream>
#include<vector>

#include<ASU_tools.hpp>

using namespace std;

int main(int argc, char **argv){

	auto ans=GrayCode(5);
	int n=ans.size();

	for (int i=0;i<n;++i) {
		for (auto item:ans[i]) 
			cout << (item?1:0);
		cout << " --> ";
		if (i%8==7) cout << '\n';
	}
	
	return 0;
}
