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

#endif
