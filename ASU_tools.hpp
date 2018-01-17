#ifndef ASU_TOOLS_HPP
#define ASU_TOOLS_HPP

#include<vector>
#include<string>

int Combination(const int &n, const int &k);
std::string Decimal2Binary(int n);
void Kmeans(double **data, int nptsx, int nptsy, int CateN, int *res);
std::vector<std::vector<bool>> GrayCode(const int &n);
std::string ShellExec(const std::string &cmd);
std::vector<bool> PointsInPolygon(const std::vector<std::pair<double,double>> &Polygon,const std::vector<std::pair<double,double>> &Points);
std::vector<std::vector<std::pair<double,double>>> CombinePolygons(const std::vector<std::vector<std::pair<double,double>>> &p);
std::pair<double,double> GridStretch(std::vector<std::pair<std::pair<double,double>,std::pair<double,double>>> &,std::pair<double,double>);
std::pair<double,double> StraightLineJunction(double,double,double,double,double,double);
// void STDCC(std::vector<double> &s1, std::vector<double> &s2, int &shift, double &Diff);
// void STD2CC(std::vector<std::vector<double>> &s1, std::vector<std::vector<double>> &s2, int &m1, int &m2, double &Diff);
bool SolvePoly(std::vector<double> C, double &ans, double tol);
double GetEps1(double *y, int npts, double del);
double GetEps2(double *y, int npts, double *x);
void Wiginterp(double *x, double *y, int npts, double *xx, double *yy, int Newnpts, bool IsEven);

#include<Amplitude.hpp>
#include<AvrStd.hpp>
#include<Bootstrap.hpp>
#include<BottomDepth.hpp>                        // Need TauP toolkit
#include<BottomLocation.hpp>                     // Need TauP toolkit
#include<Butterworth.hpp>                        // Need SAC
#include<Cart2Sph.hpp>
#include<CircleBin.hpp>
#include<CMT2Sph.hpp>
#include<ComplexDivide.hpp>
#include<ConfidenceInterval.hpp>                 // Need sci-libs/gsl
#include<Convolve.hpp>
#include<CrossCorrelation.hpp>
#include<CrossProduct.hpp>
#include<Diff.hpp>
#include<DotDist.hpp>
#include<FindPeak.hpp>
#include<GaussianBlur.hpp>
#include<GaussianFunction.hpp>
#include<GaussianSignal.hpp>
#include<GcpDistance.hpp>
#include<Interpolate.hpp>
#include<LL2TP.hpp>
#include<LocDist.hpp>
#include<Lon2180.hpp>
#include<Lon2360.hpp>
#include<Mollifier.hpp>
#include<Normalize.hpp>
#include<PNormErr.hpp>
#include<PNorm.hpp>
#include<PolyFit.hpp>                            // Need dev-libs/boost
#include<PREM.hpp>
#include<RampFunction.hpp>
#include<RayP2TakeOff.hpp>
#include<SHCoefficients.hpp>
#include<ShiftStack.hpp>
#include<SortWithIndex.hpp>
#include<Sph2Cart.hpp>
#include<SphAngle.hpp>
#include<TP2LL.hpp>
#include<TriangleSignal.hpp>

#endif
