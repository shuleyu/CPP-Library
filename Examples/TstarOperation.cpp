#include<iostream>
#include<fstream>

#include<Normalize.hpp>
#include<TstarOperation.hpp>

using namespace std;

int main(){

    int n=8000;
	double delta=0.025,ts=0.9;

    vector<double> ans=TstarOperation(ts,delta,n);

    // Use function.
	Normalize(ans);

	// output.
	for (int i=0;i<n;++i) printf("%.12e\n",ans[i]);

    return 0;
}
