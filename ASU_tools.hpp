#ifndef ASU_TOOLS_HPP
#define ASU_TOOLS_HPP

#include<vector>
#include<string>

int Combination(const int &n, const int &k);
std::string Decimal2Binary(int n);
std::vector<std::vector<bool>> GrayCode(const int &n);
std::string ShellExec(const std::string &cmd);
void Wiginterp(double *x, double *y, int npts, double *xx, double *yy, int Newnpts, bool IsEven);

#endif
