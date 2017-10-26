#include<vector>

#ifndef ASU_TOOLS_HPP
#define ASU_TOOLS_HPP

using namespace std;

string Decimal2Binary(int);
void Kmeans(double **, int, int, int, int *);
unsigned RenderSeed();
string ShellExec(const char *);
vector<bool> PointsInPolygon(vector<pair<double,double>> &,vector<pair<double,double>> &);
vector<vector<pair<double,double>>> CombinePolygons(vector<vector<pair<double,double>>> );
pair<double,double> GridStretch(vector<pair<pair<double,double>,pair<double,double>>> &,pair<double,double>);
pair<double,double> StraightLineJunction(double,double,double,double,double,double);
double DotDist(double x1,double y1,double x2,double y2);
vector<double> SH_Coeff(double rho1, double vs1, double rho2, double vs2, double px);
void STDCC(vector<double> &s1, vector<double> &s2, int &shift, double &Diff);
void STD2CC(vector<vector<double>> &s1, vector<vector<double>> &s2, int &m1, int &m2, double &Diff);
bool SolvePoly(vector<double> C, double &ans, double tol);

#endif
