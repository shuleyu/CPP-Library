#include<iostream>
#include<fstream>
#include<iomanip>

#include<PREM.hpp>

using namespace std;

int main(){

    cout << Dvs(-1,false) << endl;

//     ofstream fpout;
//     double RE,Depth,dr;
//     double rho,vpv,vph,vsv,vsh,qu,qk,yita;
//
//     RE=6371.0;
//     dr=0.5;
//
//     fpout.open("data/PREM_iso_out");
//     fpout << "<Depth>    <Vp>    <Vs>    <Rho>    <Qu>    <Qk>    <Yita>\n";
//     Depth=0;
//     while (Depth<=RE){
//         PREM(Depth,1,0,rho,vpv,vph,vsv,vsh,qu,qk,yita);
//         fpout << setw(10) << Depth << setw(10) << vpv << setw(10)  << vsv << setw(10) << rho
//               << setw(10)  << qu << setw(10)  << qk << setw(10)  << yita << "\n";
//         Depth+=dr;
//     }
//     fpout.close();
//
//     fpout.open("data/PREM_out");
//     fpout << "<Depth>    <Vpv>    <Vph>    <Vsv>    <Vsh>    <Rho>    <Qu>    <Qk>    <Yita>\n";
//     Depth=0;
//     while (Depth<=RE){
//         PREM(Depth,0,0,rho,vpv,vph,vsv,vsh,qu,qk,yita);
//         fpout << setw(10) << Depth << setw(10) << vpv << setw(10) << vph
//               << setw(10) << vsv << setw(10) << vsh << setw(10) << rho
//               << setw(10)  << qu << setw(10)  << qk << setw(10)  << yita << "\n";
//         Depth+=dr;
//     }
//     fpout.close();
//
//     fpout.open("data/PREMSmoothed_out");
//     fpout << "<Depth>    <Vp>    <Vs>    <Rho>\n";
//     Depth=0;
//     while (Depth<=RE){
//         PREMSmoothed(Depth,rho,vpv,vph,vsv,vsh,qu,qk,yita,1,1,1,1);
//         fpout << setw(10) << Depth << setw(10) << vpv << setw(10)  << vsv << setw(10) << rho << "\n";
//         Depth+=dr;
//     }
//     fpout.close();
//
//     fpout.open("data/PREMX_out");
//     fpout << "<Depth>    <Vp>    <Vs>    <Rho>\n";
//     Depth=0;
//     while (Depth<=RE){
//         PREMX(Depth,rho,vpv,vph,vsv,vsh,qu,qk,yita);
//         fpout << setw(10) << Depth << setw(10) << vpv << setw(10)  << vsv << setw(10) << rho << "\n";
//         Depth+=dr;
//     }
//     fpout.close();

    return 0;
}
