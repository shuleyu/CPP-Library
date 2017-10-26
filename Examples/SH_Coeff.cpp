#include<iostream>
#include<vector>
#include<ASU_tools.hpp>
extern "C"{
#include<ASU_tools.h>
}

using namespace std;

int main(){

	/*
	 "taup_time -mod prem -h 500 -ph ScS -deg 60 -rayp"
	 gives 7.517779
	 7.517779*180/PI/3480=0.123775


	*/
// 	vector<double> ans=SH_Coeff(d_rho(2891),d_vs(2891),d_rho(2891),d_vs(2891)*0.8,0.123775);

	// at 55 deg. refletion coefficient is negative.
	vector<double> ans=SH_Coeff(d_rho(2891),d_vs(2891),d_rho(2891),d_vs(2891)*0.8,0.119158691);
	printf("%.9lf\t%.9lf\n",ans[0],ans[1]);

	// at 40 deg. reflection coefficient still positive.
	ans=SH_Coeff(d_rho(2891),d_vs(2891),d_rho(2891),d_vs(2891)*0.8,0.099443246);
	printf("%.9lf\t%.9lf\n",ans[0],ans[1]);

	return 0;
}
