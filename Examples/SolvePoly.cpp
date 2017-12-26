#include<iostream>

#include<ASU_tools.hpp>

using namespace std;

int main(){

	vector<double> A{-93.897966421797,1306.221284532315,-6050.973091913391,9335.853350148595};
	double ans=4.4;
	double depth=400;
	A[3]-=(1-depth/6371);

	SolvePoly(A,ans,1e-8);
	printf("%.9lf\n",ans);

	return 0;
}
