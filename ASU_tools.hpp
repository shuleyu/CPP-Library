#ifndef ASU_TOOLS_HPP
#define ASU_TOOLS_HPP

#include<iostream>
#include<vector>
#include<string>

int Combination(const int &n, const int &k);
std::string Decimal2Binary(int n);
void Kmeans(double **data, int nptsx, int nptsy, int CateN, int *res);
std::string ShellExec(const std::string &cmd);
std::vector<bool> PointsInPolygon(std::vector<std::pair<double,double>> &,std::vector<std::pair<double,double>> &);
std::vector<std::vector<std::pair<double,double>>> CombinePolygons(std::vector<std::vector<std::pair<double,double>>> );
std::pair<double,double> GridStretch(std::vector<std::pair<std::pair<double,double>,std::pair<double,double>>> &,std::pair<double,double>);
std::pair<double,double> StraightLineJunction(double,double,double,double,double,double);
std::vector<double> SH_Coeff(double rho1, double vs1, double rho2, double vs2, double px);
// void STDCC(std::vector<double> &s1, std::vector<double> &s2, int &shift, double &Diff);
// void STD2CC(std::vector<std::vector<double>> &s1, std::vector<std::vector<double>> &s2, int &m1, int &m2, double &Diff);
bool SolvePoly(std::vector<double> C, double &ans, double tol);
double GetEps1(double *y, int npts, double del);
double GetEps2(double *y, int npts, double *x);
void Wiginterp(double *x, double *y, int npts, double *xx, double *yy, int Newnpts, bool IsEven);

// Note: Some of the order of these header files can't change
// because of dependence issues.
#include<Amplitude.hpp>
#include<AvrStd.hpp>
#include<ShiftStack.hpp>
#include<Bootstrap.hpp>
#include<BottomDepth.hpp>                        // Need taup toolkit installed.
#include<BottomLocation.hpp>                     // Need taup toolkit installed.
#include<Butterworth.hpp>                        // Need SAC library.
#include<Cart2Sph.hpp>
#include<CMT2Sph.hpp>
#include<ComplexDivide.hpp>
// #include<ConfidenceInterval.hpp>                 // Need gsl library. (slow)
#include<DotDist.hpp>
#include<GcpDistance.hpp>
#include<CircleBin.hpp>
#include<Convolve.hpp>
#include<CrossCorrelation.hpp>
#include<CrossProduct.hpp>
#include<Diff.hpp>
#include<Interpolate.hpp>
#include<Lon2180.hpp>
#include<Lon2360.hpp>
#include<LL2TP.hpp>
#include<Mollifier.hpp>
#include<Normalize.hpp>
// #include<PolyFit.hpp>                            // Need boost library. (slow)
#include<PREM.hpp>
#include<RayP2TakeOff.hpp>
#include<SortWithIndex.hpp>
#include<Sph2Cart.hpp>
#include<SphAngle.hpp>
#include<LocDist.hpp>
#include<TP2LL.hpp>

#endif
