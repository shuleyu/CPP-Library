#include<iostream>
#include<vector>
#include<ASU_tools.hpp>

using namespace std;

int main(){

	/*

	 "taup_time -mod prem -h 500 -ph ScS -deg 40 -rayp"
	 gives 6.03993
	 6.03993*180/PI/3480=0.099443246

	 "taup_time -mod prem -h 500 -ph ScS -deg 55 -rayp"
	 gives 7.237396
	 7.237396*180/PI/3480=0.119158691

	 "taup_time -mod prem -h 500 -ph ScS -deg 60 -rayp"
	 gives 7.517779
	 7.517779*180/PI/3480=0.123775


	*/

	// ULVZ of 20% S-velocity decrease, 0% density decrease.

	// at 40 deg. reflection coefficient is still positive.
	auto ans=SHCoefficients(Drho(2891),Dvs(2891),Drho(2891),Dvs(2891)*0.8,0.099443246);
	printf("%.9lf\t%.9lf\n",ans.first,ans.second);

	// at 55 deg. refletion coefficient is negative.
	ans=SHCoefficients(Drho(2891),Dvs(2891),Drho(2891),Dvs(2891)*0.8,0.119158691);
	printf("%.9lf\t%.9lf\n",ans.first,ans.second);

	// at 60 deg.
	ans=SHCoefficients(Drho(2891),Dvs(2891),Drho(2891),Dvs(2891)*0.8,0.123775);
	printf("%.9lf\t%.9lf\n",ans.first,ans.second);

	return 0;
}
