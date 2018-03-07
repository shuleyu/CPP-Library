#ifndef ASU_LAYERRAYPATH
#define ASU_LAYERRAYPATH

#include<iostream>
#include<vector>
#include<cmath>
#include<algorithm>

/********************************************************************
 * This C++ template calculate the ray path for the given ray
 * parameters. Difference from "RayPath.hpp", here given layers are
 * in Cartesian Coordinates.
 *
 * input(s):
 * const vector<T1> &z             ----  layer depth array. z[0] is the depth of the shallowest layer. z>=0.
 * const vector<T2> &v             ----  velocity array at each depth.
 * const T3         &rayp          ----  ray parameter. (sec/km, p=sin/c)
 * const T4         &MinDepth      ----  depth to start the ray tracing.
 * const T5         &MaxDepth      ----  depth to stop the ray tracing.
 * vector<double>   &X             ----  Output ray path, horizontal distance from source. If X[0]<-1e5, will only output X.back();
 * vector<double>   &D             ----  Output ray path, vertical distance from z=0. If D[0]<-1e5, will only output D.back();
 * const T6         &TurningAngle  ----  (Optional) the critical angle for turning assessment.
 *                                       default value is 89.9 deg.
 * return(s):
 * pair<double,double>  ans  ----  {travel time (s) / pursuit distance (km)}
 * vector<double> &X (in-place)
 * vector<double> &D (in-place)
 *
 * Shule Yu
 * Mar 06 2018
 *
 * Key words: ray path, ray tracing, cartesian.
********************************************************************/

template<class T1, class T2, class T3, class T4, class T5, class T6=double>
std::pair<double,double> LayerRayPath(const std::vector<T1> &z, const std::vector<T2> &v,
                                 const T3 &rayp, const T4 &MinDepth, const T5 &MaxDepth,
                                 std::vector<double> &X,std::vector<double> &D,
                                 const T6 &TurningAngle=89.9){

    // check inputs.
    if (MaxDepth<=MinDepth || MaxDepth>z.back() || MinDepth<z[0]){
        std::cerr <<  "Error in " << __func__ << ":  MinDepth/MaxDepth input error ..." << std::endl;
        return {-1,-1};
    }

    auto cmp=[](const T1 &a, const T1 &b){
        return a<=b;
    };
    if (!std::is_sorted(z.begin(),z.end(),cmp)) {
        std::cerr <<  "Error in " << __func__ << ":  layers (depth) is not strict monotonic increasing..." << std::endl;
        return {-1,-1};
    }

    // locate our start layer and end layer.
    // because the grid could be unevenly spaced, this is a bit lengthy;
    size_t P1;
    double CurMin=std::numeric_limits<double>::max();
    for (P1=0;P1<z.size();++P1) {
        double NewMin=fabs(MinDepth-z[P1]);
        if (CurMin<NewMin) {--P1;break;}
        CurMin=NewMin;
    }

    size_t P2;
    CurMin=std::numeric_limits<double>::max();
    for (P2=0;P2<z.size();++P2) {
        double NewMin=fabs(MaxDepth-z[P2]);
        if (CurMin<NewMin) {--P2;break;}
        CurMin=NewMin;
    }

    // prepare output.
    bool OutPutX=(X.empty() || X[0]>=-1e5);
    bool OutPutD=(D.empty() || D[0]>=-1e5);
    X.clear(); D.clear();

    // start ray tracing.
    //   A=sin(takeoff);

    double x=0,d=z[P1],MaxAngle=sin(TurningAngle*M_PI/180);
    std::pair<double,double> ans{0,0};
    for (size_t i=P1;i<P2-1;++i){

        double A=rayp*v[i];

        // Judge turning.
        if (A>=MaxAngle) break;

        double dist=(z[i+1]-z[i])/sqrt(1-A*A);

        // store travel time and distance of this step.
        ans.first+=dist/v[i];
        ans.second+=dist;

        // store the path of this step.
        if (OutPutX) X.push_back(x);
        if (OutPutD) D.push_back(d);
        x+=A*dist;
        d=z[i+1];
    }
    X.push_back(x);
    D.push_back(d);

    return ans;
}

#endif
