#include<iostream>
#include<vector>

#include<PlaneWaveCoefficients.hpp>
#include<PREM.hpp>

using namespace std;

int main(){

	/*

	 "taup_time -mod prem -h 500 -ph ScS -deg 40 -rayp"
	 gives              : 6.03993
     incident angle(deg): asin(6.03993*180/M_PI/3480*Dvs(2891))*180/M_PI

	 "taup_time -mod prem -h 500 -ph ScS -deg 55 -rayp"
	 gives              : 7.237396
     incident angle(deg): asin(7.237396*180/M_PI/3480*Dvs(2891))*180/M_PI

	 "taup_time -mod prem -h 500 -ph ScS -deg 60 -rayp"
	 gives              : 7.517779
     incident angle(deg): asin(7.517779*180/M_PI/3480*Dvs(2891))*180/M_PI

	*/

	// ULVZ with dVp=0%, dVs=-20%, dRho=0%. Top side reflection coefficients.
    double rho1,vp1,vs1,rho2,vp2,vs2,inc;
    rho1=rho2=Drho(2891);
    vp1=vp2=Dvp(2891);
    vs1=Dvs(2891);
    vs2=vs1*0.8;

	// at 40 deg. reflection coefficient is still positive.
    inc=asin(6.03993*180/M_PI/3480*Dvs(2891))*180/M_PI;
	auto ans=PlaneWaveCoefficients(rho1,vp1,vs1,rho2,vp2,vs2,inc,"SH","SS");
	printf("R: %.9lf\n",ans[0]);

	// at 55 deg. refletion coefficient is negative.
    inc=asin(7.237396*180/M_PI/3480*Dvs(2891))*180/M_PI;
	ans=PlaneWaveCoefficients(rho1,vp1,vs1,rho2,vp2,vs2,inc,"SH","SS");
	printf("R: %.9lf\n",ans[0]);

	// at 60 deg.
    inc=asin(7.517779*180/M_PI/3480*Dvs(2891))*180/M_PI;
	ans=PlaneWaveCoefficients(rho1,vp1,vs1,rho2,vp2,vs2,inc,"SH","SS");
	printf("R: %.9lf\n",ans[0]);

	return 0;
}
