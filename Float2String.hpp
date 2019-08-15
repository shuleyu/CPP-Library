#ifndef ASU_FLOAT2STRING
#define ASU_FLOAT2STRING

#include<sstream>

std::string Float2String(const double &d, const int &n=0){
    std::ostringstream os;
    os << std::fixed << std::setprecision(n) << d;
    return os.str();
}

#endif
