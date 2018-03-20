#ifndef ASU_FINDRAYPATH
#define ASU_FINDRAYPATH

#include<iostream>
#include<vector>
#include<cmath>

#include<RayPath.hpp>

/*************************************************************
 * This C++ template finds the ray path connecting two points.
 *
 * input(s):
 * const vector<T1> &r  ----  layer radius array. r[0] is the shallowest layer.
 * const vector<T2> &v  ----  velocity at each radius.
 * const T3 &r1         ----  Radius of the first point.
 * const T4 &r2         ----  Radius of the second point.
 * const T5 &t          ----  Distance (in deg) between the two points.
 * const T6 &tol        ----  (Optional) Takeoff angle error tolerance (in deg, default is 0.001).
 *
 * return(s):
 * double rayp  ----  Ray parameter connection the two points. (in sec/deg, p=Rsin/c/180*PI)
 *
 * Shule Yu
 * Jan 29 2018
 *
 * Key words: rayp, ray parameter, ray tracing.
***********************************************************/

template<class T1,class T2,class T3,class T4,class T5,class T6=double>
double FindRayPath(const std::vector<T1> &r, const std::vector<T2> &v,
                   const T3 &r1, const T4 &r2, const T5 &t, const T6 &tol=1e-3){

    // check inputs.
    auto cmp=[](const T1 &a, const T1 &b){
        return a>=b;
    };
    if (!std::is_sorted(r.begin(),r.end(),cmp)) {
        std::cerr <<  "Error in " << __func__ << ": layers (radius) is not strict monotonic decreasing..." << std::endl;
        return -1;
    }

    double R1=std::max(r1,r2),R2=std::min(r1,r2);
    if (R2<r.back() || R1>r[0]){
        std::cerr <<  "Error in " << __func__ << ": Start/End point depth error ..." << std::endl;
        return -1;
    }

    // locate our start Layer and end Layer.
    // because the grid could be unevenly spaced, this is a bit lengthy;
    size_t P1;
    double CurMin=std::numeric_limits<double>::max();
    for (P1=0;P1<r.size();++P1) {
        double NewMin=fabs(R1-r[P1]);
        if (CurMin<NewMin) {--P1;break;}
        CurMin=NewMin;
    }

    // Start Calculating.
    double RE=6371.0,theta=fabs(t),ll=0,rr=90,mid,rayp=0;
    std::vector<double> degree{0},radius{0};

    while (rr-ll>tol){
        mid=ll+(rr-ll)/2;

        degree[0]=radius[0]=-1e6;
        rayp=R1*sin(mid*M_PI/180)/v[P1]/180*M_PI;
        RayPath(r,v,rayp,RE-R1,RE-R2,degree,radius);

        if (degree[0]>theta || radius[0]>R2+1e-6) rr=mid;
        else ll=mid;
    }

    return rayp;
}

#endif
