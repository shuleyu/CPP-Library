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
    double rho1,vp1,vs1,rho2,vp2,vs2,inc;
    rho1=rho2=Drho(2861);
    vp1=Dvp(2861);
    vp2=vp1*1.1;
    vs1=Dvs(2861);
    vs2=vs1*1.1;

    // at 40 deg. reflection coefficient is still positive.
//     inc=asin(6.03993*180/M_PI/3480*Dvs(2891))*180/M_PI;
//     auto ans=PlaneWaveCoefficients(rho1,vp1,vs1,rho2,vp2,vs2,inc,"SH","SS");
//     printf("R: %.9lf + %.9lfi\n",ans[0].real(),ans[0].imag());

    // at 55 deg. refletion coefficient becomes negative.
//     inc=asin(7.237396*180/M_PI/3480*Dvs(2891))*180/M_PI;
//     ans=PlaneWaveCoefficients(rho1,vp1,vs1,rho2,vp2,vs2,inc,"SH","SS");
//     printf("R: %.9lf + %.9lfi\n",ans[0].real(),ans[0].imag());

    // at 60 deg.
//     inc=asin(7.517779*180/M_PI/3480*Dvs(2891))*180/M_PI;
//     ans=PlaneWaveCoefficients(rho1,vp1,vs1,rho2,vp2,vs2,inc,"SH","SS");
//     printf("R: %.9lf + %.9lfi\n",ans[0].real(),ans[0].imag());

    inc=asin(4.3519*180/M_PI/3510*Dvs(2861))*180/M_PI;
    auto ans=PlaneWaveCoefficients(rho1,vp1,vs1,rho2,vp2,vs2,inc,"PSV","SS");
    printf("R_SS: %.9lf + %.9lfi    Amp: %.9lf, Phase: %.9lf\n",ans[3].real(),ans[3].imag(),abs(ans[3]),180/M_PI*arg(ans[3]));
    printf("R_SP: %.9lf + %.9lfi    Amp: %.9lf, Phase: %.9lf\n",ans[2].real(),ans[2].imag(),abs(ans[2]),180/M_PI*arg(ans[2]));
//     auto c=ans[6]*ans[0]*ans[4];
//     printf("T_SP*R_PP*T_PP: %.9lf + %.9lfi    Amp: %.9lf, Phase: %.9lf\n",c.real(),c.imag(),abs(c),180/M_PI*arg(c));

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
//     double inc=74.8,rho1=5.566,vp1=13.7164,vs1=7.26,rho2=rho1,vp2=vp1,vs2=vs1*1.1;
//     auto ans=PlaneWaveCoefficients(rho1,vp1,vs1,rho2,vp2,vs2,inc,"SH","SS");
//     cout << "Vals   : ";
//     for (auto &item:ans) cout << item << ", " ;
//     cout << "\nABS    : ";
//     for (auto &item:ans) cout << abs(item) << ", " ;
//     cout << "\nPhases : ";
//     for (auto &item:ans) cout << 180*arg(item)/M_PI << ", " ;
//     cout << endl;

//     for (double inc=0;inc<=1;inc+=0.01) {
//         double rho1=2.1,vs1=2.02,vp1=3.5,rho2=2.5,vs2=3.7,vp2=5.5;
//         auto ans=PlaneWaveCoefficients(rho1,vp1,vs1,rho2,vp2,vs2,inc,"PSV","SS");
//         cout << inc << " " <<  abs(ans[1]) << " " << arg(ans[1]) << '\n';
//     }

    return 0;
}
