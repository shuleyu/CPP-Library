#include<iostream>

#include<ASU_tools.hpp>

using namespace std;

int main(){

	vector<double> A{34.5,-6,7.3,-8,-173235.0051};
    double ans=8;

	SolvePoly(A,ans,1e-6);
	printf("%.9lf\n",ans);

	return 0;
}
