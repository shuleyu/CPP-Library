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
    // ULVZ with dVp=0%, dVs=-20%, dRho=0%. Top side reflection coefficients.
//     double rho1,vp1,vs1,rho2,vp2,vs2,inc;
//     rho1=rho2=Drho(2891);
//     vp1=vp2=Dvp(2891);
//     vs1=Dvs(2891);
//     vs2=vs1*0.8;
// 
//     // at 40 deg. reflection coefficient is still positive.
//     inc=asin(6.03993*180/M_PI/3480*Dvs(2891))*180/M_PI;
//     auto ans=PlaneWaveCoefficients(rho1,vp1,vs1,rho2,vp2,vs2,inc,"SH","SS");
//     printf("R: %.9lf + %.9lfi\n",ans[0].real(),ans[0].imag());
// 
//     // at 55 deg. refletion coefficient becomes negative.
//     inc=asin(7.237396*180/M_PI/3480*Dvs(2891))*180/M_PI;
//     ans=PlaneWaveCoefficients(rho1,vp1,vs1,rho2,vp2,vs2,inc,"SH","SS");
//     printf("R: %.9lf + %.9lfi\n",ans[0].real(),ans[0].imag());
// 
//     // at 60 deg.
//     inc=asin(7.517779*180/M_PI/3480*Dvs(2891))*180/M_PI;
//     ans=PlaneWaveCoefficients(rho1,vp1,vs1,rho2,vp2,vs2,inc,"SH","SS");
//     printf("R: %.9lf + %.9lfi\n",ans[0].real(),ans[0].imag());

    /* Experiment 2.

     Plot figure 5.6 in Page 137 of Aki and Richard, 2nd ed, 2002

    */
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
    double inc=41.090,rho1=2.6,vp1=5.8,vs1=3.2,rho2=1.02,vp2=1.45,vs2=0.01;
    auto ans=PlaneWaveCoefficients(rho1,vp1,vs1,rho2,vp2,vs2,inc,"PSV","SL");
    cout << "Vals   : ";
    for (auto &item:ans) cout << item << ", " ;
    cout << "\nABS    : ";
    for (auto &item:ans) cout << abs(item) << ", " ;
    cout << "\nPhases : ";
    for (auto &item:ans) cout << 180*arg(item)/M_PI << ", " ;
    cout << endl;

//     for (double inc=0;inc<=1;inc+=0.01) {
//         double rho1=2.1,vs1=2.02,vp1=3.5,rho2=2.5,vs2=3.7,vp2=5.5;
//         auto ans=PlaneWaveCoefficients(rho1,vp1,vs1,rho2,vp2,vs2,inc,"PSV","SS");
//         cout << inc << " " <<  abs(ans[1]) << " " << arg(ans[1]) << '\n';
//     }

    return 0;
}
