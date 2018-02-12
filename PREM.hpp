#ifndef ASU_PREM
#define ASU_PREM

#include<iostream>
#include<cmath>

// PREM            // original PREM
//
// PREMSmoothed    // Replace first-order discontinuities.
//                     with straight line over +- 30km.
//
// PREMX           // Spline interpolate from surface to 400km.
//                 // Then degree2 from 600 km to the proper depth.

/***********************************************************
 * This C++ template returns PREM model values at certain depth.
 * There's some small template you can use for just one value.
 *
 * input(s):
 * const T   &Depth  ----  Given depth (in km)
 * const int &iso    ----  0/1: (set to 1 if want isotropy result)
 * const int &ocean  ----  0/1: (set to 1 if want ocean properties near surface)
 * double    &rho    ----  density (g/cm^3)
 * double    &vpv    ----  Vertical polarity P-wave speed (P-wave speed when traveling along the radius direction)
 * double    &vph    ----  Horizontal polarity P-wave speed (P-wave speed when traveling perpendicular to the radius direction)
 * double    &vsv    ----  Vertical polarity S-wave speed (SV-wave speed when traveling perpendicular to the radius direction)
 * double    &vsh    ----  Horizontal polarity S-wave speed (SH-wave speed when traveling perpendicular to the radius direction)
 *                                                          (also, SV- and SH-wave speed when traveling along the radius direction)
 * double    &qu     ----  Isotropic dissipation of shear energy.
 * double    &qk     ----  Isotropic dissipation of compressional energy.
 * double    &yita   ----  Anisotropic parameter.
 *
 * output(s):
 * double    &rho  (in-place)
 * double    &vpv  (in-place)
 * double    &vph  (in-place)
 * double    &vsv  (in-place)
 * double    &vsh  (in-place)
 * double    &qu   (in-place)
 * double    &qk   (in-place)
 * double    &yita (in-place)
 *
 * Shule Yu
 * Dec 29 2017
 *
 * Key words: PREM, prem
 *
 * Reference :
 *     Physics_of_the_earth_and_planetary_interiors_1981_Dziewonski
***********************************************************/

template<class T>
void PREM(const T &Depth,const int &iso, const int &ocean,
          double &rho, double &vpv, double &vph, double &vsv,
          double &vsh, double &qu, double &qk, double &yita){

    double R=6371.0;
    double r=R-Depth;
    double x=r/R;

    yita=sqrt(-1);
    qk=57823;

    if ( 0<=r && r<1221.5 ) {
    // inner core

        rho= -8.8381*x*x + 13.0885;
        vpv= -6.3640*x*x + 11.2622;
        vph=  vpv;
        vsv= -4.4475*x*x + 3.6678;
        vsh=  vsv;
        qu =  84.6;
        qk =  1327.7;
    }

    else if ( 1221.5<=r && r<3480.0 ) {
    // 2891 ~ 5149.5 km, outter core.

        rho=  -5.5281*x*x*x - 3.6426*x*x - 1.2638*x + 12.5815;
        vpv= -13.5732*x*x*x + 4.8023*x*x - 4.0362*x + 11.0487;
        vph=  vpv;
        vsv=  0;
        vsh=  0;
        qu =  INFINITY;
    }

    else if ( 3480.0<=r && r<3630.0 ) {
    // 2741 ~ 2891 km, D''

        rho= -3.0807*x*x*x + 5.5283*x*x - 6.4761*x + 7.9565;
        vpv= -2.5514*x*x*x + 5.5242*x*x - 5.3181*x + 15.3891;
        vph=  vpv;
        vsv=  0.9783*x*x*x - 2.0834*x*x + 1.4672*x + 6.9254;
        vsh=  vsv;
        qu =  312;
    }

    else if ( 3630.0<=r && r<5600.0 ) {
    // 771 ~ 2741 km

        rho=  -3.0807*x*x*x +  5.5283*x*x -  6.4761*x +  7.9565;
        vpv= -26.6419*x*x*x + 51.4832*x*x - 40.4673*x + 24.9520;
        vph=  vpv;
        vsv=  -9.2777*x*x*x + 17.4575*x*x - 13.7818*x + 11.1671;
        vsh=  vsv;
        qu =  312;
    }

    else if ( 5600.0<=r && r<5701.0 ) {
    // 670 ~ 771 km

        rho= -3.0807*x*x*x + 5.5283*x*x -  6.4761*x +  7.9565;
        vpv= -2.5514*x*x*x + 5.5242*x*x - 23.6027*x + 29.2766;
        vph=  vpv;
        vsv=  0.9783*x*x*x - 2.0834*x*x - 17.2473*x + 22.3459;
        vsh=  vsv;
        qu =  312;
    }

    else if ( 5701.0<=r && r<5771.0 ) {
    // 600 ~ 670 km

        rho= -1.4836*x +  5.3197;
        vpv= -9.8672*x + 19.0957;
        vph=  vpv;
        vsv= -4.9324*x +  9.9839;
        vsh=  vsv;
        qu =  143;
    }

    else if ( 5771.0<=r && r<5971.0 ) {
    // 400 ~ 600 km

        rho=  -8.0298*x + 11.2494;
        vpv= -32.6166*x + 39.7027;
        vph=  vpv;
        vsv= -18.5856*x + 22.3512;
        vsh=  vsv;
        qu =  143;
    }

    else if ( 5971.0<=r && r<6151.0 ) {
    // 220 ~ 400 km

        rho=  -3.8045*x +  7.1089;
        vpv= -12.2569*x + 20.3926;
        vph=  vpv;
        vsv=  -4.4597*x +  8.9496;
        vsh=  vsv;
        qu =  143;
    }

    else if ( 6151.0<=r && r<6291.0 ) {
    // 80 ~ 220 km, LVZ

        rho=  0.6924*x + 2.6910;
        vpv=  7.2180*x + 0.8317;
        vph=  4.6172*x + 3.5908;
        vsv= -1.4678*x + 5.8582;
        vsh=  5.7176*x - 1.0839;
        qu =  80;
        yita= -2.4778*x + 3.3687;
    }

    else if ( 6291.0<=r && r<6346.6 ) {
    // 24.4 ~ 80 km, LID

        rho=  0.6924*x + 2.6910;
        vpv=  7.2180*x + 0.8317;
        vph=  4.6172*x + 3.5908;
        vsv= -1.4678*x + 5.8582;
        vsh=  5.7176*x - 1.0839;
        qu =  600;
        yita= -2.4778*x + 3.3687;
    }

    else if ( 6346.6<=r && r<6356.0 ) {
    // crust 1
        rho=  2.900;
        vpv=  6.800;
        vph=  vpv;
        vsv=  3.900;
        vsh=  vsv;
        qu =  600;
    }

    else if ( 6356.0<=r && r<=R+1e-5 ) {
    // crust 2

        rho=  2.600;
        vpv=  5.800;
        vph=  vpv;
        vsv=  3.200;
        vsh=  vsv;
        qu =  600;
    }

    else { // space

        std::cerr <<  "Error in " << __func__ << ": negative depth value: " << R-r << " ..." << std::endl;

        rho=0;
        vpv=0;
        vph=0;
        vsv=0;
        vsh=0;
        qu =0;
        yita=0;

    }

    if ( ocean==1 && 6368.0<=r && r<=R+1e-5 ) {
    // ocean

        rho=  1.020;
        vpv=  1.450;
        vph=  vpv;
        vsv=  0;
        vsh=  vsv;
        qu =  INFINITY;
    }

    if ( iso==1 && 6151.0<=r && r<6291.0 ) {
    // LVZ iso

        rho=  0.6924*x + 2.6910;
        vpv=  3.9382*x + 4.1875;
        vph=  vpv;
        vsv=  2.3481*x + 2.1519;
        vsh=  vsv;
        qu =  80;
        yita= -2.4778*x + 3.3687;
    }

    if ( iso==1 && 6291.0<=r && r<6346.6 ) {
    // LID iso

        rho=  0.6924*x + 2.6910;
        vpv=  3.9382*x + 4.1875;
        vph=  vpv;
        vsv=  2.3481*x + 2.1519;
        vsh=  vsv;
        qu =  600;
        yita= -2.4778*x + 3.3687;
    }

    return ;
}

template<class T>
double Dvs(const T &Depth){
    double rho,vpv,vph,vsv,vsh,qu,qk,yita;
    PREM(Depth,1,0,rho,vpv,vph,vsv,vsh,qu,qk,yita);
    return vsv;
}

template<class T>
double Dvp(const T &Depth){
    double rho,vpv,vph,vsv,vsh,qu,qk,yita;
    PREM(Depth,1,0,rho,vpv,vph,vsv,vsh,qu,qk,yita);
    return vpv;
}

template<class T>
double Drho(const T &Depth){
    double rho,vpv,vph,vsv,vsh,qu,qk,yita;
    PREM(Depth,1,0,rho,vpv,vph,vsv,vsh,qu,qk,yita);
    return rho;
}

template<class T>
double Rvs(const T &Radius){
    return Dvs(6371.0-Radius);
}

template<class T>
double Rvp(const T &Radius){
    return Dvp(6371.0-Radius);
}

template<class T>
double Rrho(const T &Radius){
    return Drho(6371.0-Radius);
}

// PREMSmoothed

template<class T>
void PREMSmoothed(const T &Depth, double &rho,double &vpv,double &vph,
                  double &vsv,double &vsh,double &qu,double &qk,double &yita,
                  const int &RemoveCrust,const int &Remove220,
                  const int &Remove400, const int &Remove670){

// Inspired from Mike Thorne.
// By default, no ocean (0), no anisotropy (1).
// Linear interpolate between +- 30km around each discontinuity.

    double r=6371.0-Depth;
    PREM(Depth,1,0,rho,vpv,vph,vsv,vsh,qu,qk,yita);

    // Remove Crust.
    if ( RemoveCrust==1 && 6346.6<=r ) {
        vsv=4.49100712;
        vpv=8.11061727;
        rho=3.38074821;
    }

    // Remove 220km.
    if ( Remove220==1 && 6121.0<=r && r<6181.0 ){
        vsv=Rvs(6181.0)+(Rvs(6121.0)-Rvs(6181.0))/(6121.0-6181.0)*(r-6181.0);
        vpv=Rvp(6181.0)+(Rvp(6121.0)-Rvp(6181.0))/(6121.0-6181.0)*(r-6181.0);
        rho=Rrho(6181.0)+(Rrho(6121.0)-Rrho(6181.0))/(6121.0-6181.0)*(r-6181.0);
    }

    // Remove 400km.
    if ( Remove400==1 && 5941.0<=r && r<6001.0 ){
        vsv=Rvs(6001.0)+(Rvs(5941.0)-Rvs(6001.0))/(5941.0-6001.0)*(r-6001.0);
        vpv=Rvp(6001.0)+(Rvp(5941.0)-Rvp(6001.0))/(5941.0-6001.0)*(r-6001.0);
        rho=Rrho(6001.0)+(Rrho(5941.0)-Rrho(6001.0))/(5941.0-6001.0)*(r-6001.0);
    }

    // Remove 670km.
    if ( Remove670==1 && 5671.0<=r && r<5731.0 ){
        vsv=Rvs(5731.0)+(Rvs(5671.0)-Rvs(5731.0))/(5671.0-5731.0)*(r-5731.0);
        vpv=Rvp(5731.0)+(Rvp(5671.0)-Rvp(5731.0))/(5671.0-5731.0)*(r-5731.0);
        rho=Rrho(5731.0)+(Rrho(5671.0)-Rrho(5731.0))/(5671.0-5731.0)*(r-5731.0);
    }

    vsh=vsv;
    vph=vpv;

    return;
}

template<class T>
double DvsS(const T &Depth,const int &RemoveCrust, const int &Remove220,
            const int &Remove400, const int &Remove670){
    double rho,vpv,vph,vsv,vsh,qu,qk,yita;
    PREMSmoothed(Depth,rho,vpv,vph,vsv,vsh,qu,qk,yita,RemoveCrust,Remove220,Remove400,Remove670);
    return vsv;
}

template<class T>
double DvpS(const T &Depth,const int &RemoveCrust, const int &Remove220,
            const int &Remove400, const int &Remove670){
    double rho,vpv,vph,vsv,vsh,qu,qk,yita;
    PREMSmoothed(Depth,rho,vpv,vph,vsv,vsh,qu,qk,yita,RemoveCrust,Remove220,Remove400,Remove670);
    return vpv;
}

template<class T>
double DrhoS(const T &Depth,const int &RemoveCrust, const int &Remove220,
            const int &Remove400, const int &Remove670){
    double rho,vpv,vph,vsv,vsh,qu,qk,yita;
    PREMSmoothed(Depth,rho,vpv,vph,vsv,vsh,qu,qk,yita,RemoveCrust,Remove220,Remove400,Remove670);
    return rho;
}

template<class T>
double RvsS(const T &Radius,const int &RemoveCrust, const int &Remove220,
            const int &Remove400, const int &Remove670){
    return DvsS(6371.0-Radius,RemoveCrust,Remove220,Remove400,Remove670);
}

template<class T>
double RvpS(const T &Radius,const int &RemoveCrust, const int &Remove220,
            const int &Remove400, const int &Remove670){
    return DvpS(6371.0-Radius,RemoveCrust,Remove220,Remove400,Remove670);
}

template<class T>
double RrhoS(const T &Radius,const int &RemoveCrust, const int &Remove220,
            const int &Remove400, const int &Remove670){
    return DrhoS(6371.0-Radius,RemoveCrust,Remove220,Remove400,Remove670);
}

// PREMX

template<class T>
void PREMX(const T &Depth, double &rho,double &vpv,double &vph,
           double &vsv,double &vsh,double &qu,double &qk,double &yita){

    PREM(Depth,1,0,rho,vpv,vph,vsv,vsh,qu,qk,yita);

    double R=6371.0;
    double r=R-Depth;
    double x=r/R;


    if (Depth<=400) {
        vsh= -1147.226569441673*x*x*x + 3481.648723871561*x*x - 3521.617739418105*x + 1191.686592108216;
        vsv= vsh;
        rho=   734.780041069559*x*x*x - 2071.193615789281*x*x + 1938.047108369886*x -  598.252785440164;
        vph=   -4.9208884249226*x*x*x + 274.0496803031463*x*x - 533.3366953315248*x + 272.3185207233010;
        vpv= vph;
    }

    if (600<=Depth && Depth<=1155.674) {
        vsh= -84.46554942*x*x + 134.4361189*x - 46.95411628;
        vsv= vsh;
    }
    if (600<=Depth && Depth<=1726.323) {
        rho= -13.29902926*x*x + 16.06334012*x + 0.3373366175;
    }
    if (600<=Depth && Depth<=970.426) {
        vph= -189.1928756*x*x + 310.1340153*x - 115.5330402;
        vpv= vph;
    }

    return;
}

template<class T>
double DvsX(const T &Depth){
    double rho,vpv,vph,vsv,vsh,qu,qk,yita;
    PREMX(Depth,rho,vpv,vph,vsv,vsh,qu,qk,yita);
    return vsv;
}

template<class T>
double DvpX(const T &Depth){
    double rho,vpv,vph,vsv,vsh,qu,qk,yita;
    PREMX(Depth,rho,vpv,vph,vsv,vsh,qu,qk,yita);
    return vpv;
}

template<class T>
double DrhoX(const T &Depth){
    double rho,vpv,vph,vsv,vsh,qu,qk,yita;
    PREMX(Depth,rho,vpv,vph,vsv,vsh,qu,qk,yita);
    return rho;
}

template<class T>
double RvsX(const T &Radius){
    return DvsX(6371.0-Radius);
}

template<class T>
double RvpX(const T &Radius){
    return DvpX(6371.0-Radius);
}

template<class T>
double RrhoX(const T &Radius){
    return DrhoX(6371.0-Radius);
}

#endif
