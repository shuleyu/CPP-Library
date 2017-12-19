#include<vector>
#include<string>

#ifndef ASU_TOOLS_HPP
#define ASU_TOOLS_HPP

// using namespace std;

std::string Decimal2Binary(int);
void Kmeans(double **, int, int, int, int *);
unsigned RenderSeed();
std::string ShellExec(const char *);
std::vector<bool> PointsInPolygon(std::vector<std::pair<double,double>> &,std::vector<std::pair<double,double>> &);
std::vector<std::vector<std::pair<double,double>>> CombinePolygons(std::vector<std::vector<std::pair<double,double>>> );
std::pair<double,double> GridStretch(std::vector<std::pair<std::pair<double,double>,std::pair<double,double>>> &,std::pair<double,double>);
std::pair<double,double> StraightLineJunction(double,double,double,double,double,double);
double DotDist(double x1,double y1,double x2,double y2);
std::vector<double> SH_Coeff(double rho1, double vs1, double rho2, double vs2, double px);
void STDCC(std::vector<double> &s1, std::vector<double> &s2, int &shift, double &Diff);
void STD2CC(std::vector<std::vector<double>> &s1, std::vector<std::vector<double>> &s2, int &m1, int &m2, double &Diff);
bool SolvePoly(std::vector<double> C, double &ans, double tol);
void Interpolate(const std::vector<std::pair<double,double>> &input, std::vector<std::pair<double,double>> &output,int flag);
std::pair<double,double> AvrStd(const std::vector<double> &V,const std::vector<double> &W={});
std::vector<double> PolyFit(const std::vector<std::pair<double,double>> &data,int Degree,const std::vector<double> &weight={});

#endif
