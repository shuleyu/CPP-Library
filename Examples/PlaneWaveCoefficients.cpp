#include<iostream>
#include<vector>

#include<PlaneWaveCoefficients.hpp>
#include<PREM.hpp>

using namespace std;

int main(){

//     /*  Experiment 1.
//
//      "taup_time -mod prem -h 500 -ph ScS -deg 40 -rayp"
//      gives              : 6.03993
//      incident angle(deg): asin(6.03993*180/M_PI/3480*Dvs(2891))*180/M_PI
//
//      "taup_time -mod prem -h 500 -ph ScS -deg 55 -rayp"
//      gives              : 7.237396
//      incident angle(deg): asin(7.237396*180/M_PI/3480*Dvs(2891))*180/M_PI
//
//      "taup_time -mod prem -h 500 -ph ScS -deg 60 -rayp"
//      gives              : 7.517779
//      incident angle(deg): asin(7.517779*180/M_PI/3480*Dvs(2891))*180/M_PI
//
//     */
//
//     // ULVZ with dVp=0%, dVs=-20%, dRho=0%. Top side reflection coefficients.
//     double rho1,vp1,vs1,rho2,vp2,vs2,inc;
//     rho1=rho2=Drho(2891);
//     vp1=vp2=Dvp(2891);
//     vs1=Dvs(2891);
//     vs2=vs1*0.8;
//
//     // at 40 deg. reflection coefficient is still positive.
//     inc=asin(6.03993*180/M_PI/3480*Dvs(2891))*180/M_PI;
//     auto ans=PlaneWaveCoefficients(rho1,vp1,vs1,rho2,vp2,vs2,inc,"SH","SS");
//     printf("R: %.9lf\n",ans[0]);
//
//     // at 55 deg. refletion coefficient becomes negative.
//     inc=asin(7.237396*180/M_PI/3480*Dvs(2891))*180/M_PI;
//     ans=PlaneWaveCoefficients(rho1,vp1,vs1,rho2,vp2,vs2,inc,"SH","SS");
//     printf("R: %.9lf\n",ans[0]);
//
//     // at 60 deg.
//     inc=asin(7.517779*180/M_PI/3480*Dvs(2891))*180/M_PI;
//     ans=PlaneWaveCoefficients(rho1,vp1,vs1,rho2,vp2,vs2,inc,"SH","SS");
//     printf("R: %.9lf\n",ans[0]);

//     /* Experiment 2.
//
//      Plot figure 5.6 in Page 137 of Aki and Richard, 2nd ed, 2002
//
//     */
//     for (double inc=0;inc<=90;inc+=0.25) {
//         double rho=1,vs=3,vp=5,ps=sin(inc/180*M_PI)/vs,pp=sin(inc/180*M_PI)/vp;
//         auto ans=PlaneWaveCoefficients(rho,vp,vs,rho,vp,vs,inc,"PSV","SA");
//         cout << pp << " " <<  ans[0] << " ";
//         cout << pp << " " <<  ans[1] << " ";
//         cout << ps << " " <<  ans[2] << " ";
//         cout << ps << " " <<  ans[3] << " ";
//         cout << endl;
//     }


    // Other examples.
    double inc=62.8924,rho1=5.55142,vp1=13.7093,vs1=7.26495,rho2=5.55147,vp2=13.7093,vs2=5.08547;
    auto ans=PlaneWaveCoefficients(rho1,vp1,vs1,rho2,vp2,vs2,inc,"PSV","SS");
    for (auto &item:ans) cout << item << " " ;
    cout << endl;

    return 0;
}
