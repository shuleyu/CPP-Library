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
 * size_t           &D             ----  Output ray path end index in "z".
 * const T6         &TurningAngle  ----  (Optional) the critical angle for turning assessment.
 *                                       default value is 89.9 deg.
 * return(s):
 * pair<pair<double,double>,bool>  ans  ----  {{travel time (s) / pursuit distance (km)}, RayTurns?}
 * vector<double> &X (in-place)
 * size_t &D (in-place)
 *
 * Shule Yu
 * Mar 06 2018
 *
 * Key words: ray path, ray tracing, cartesian.
********************************************************************/

template<class T1, class T2, class T3, class T4, class T5, class T6=double>
std::pair<std::pair<double,double>,bool> LayerRayPath(const std::vector<T1> &z, const std::vector<T2> &v,
                                         const T3 &rayp, const T4 &MinDepth, const T5 &MaxDepth,
                                         std::vector<double> &X,size_t &D,
                                         const T6 &TurningAngle=89.9){

    // check inputs.
    if (MaxDepth<=MinDepth || MaxDepth>z.back() || MinDepth<z[0]){
        std::cerr <<  "Error in " << __func__ << ":  MinDepth/MaxDepth input error ..." << std::endl;
        return {{-1,-1},false};
    }

    auto cmp=[](const T1 &a, const T1 &b){
        return a<=b;
    };
    if (!std::is_sorted(z.begin(),z.end(),cmp)) {
        std::cerr <<  "Error in " << __func__ << ":  layers (depth) is not strict monotonic increasing..." << std::endl;
        return {{-1,-1},false};
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
    X.clear();

    // start ray tracing.
    //   A=sin(takeoff);

    double x=0,MaxAngle=sin(TurningAngle*M_PI/180);
    std::pair<std::pair<double,double>,bool> ans{{0,0},false};
    for (size_t i=P1;i<P2;++i){

        double A=rayp*v[i];

        // Judge turning.
        if (A>=MaxAngle) {
            D=i;
            X.push_back(x);
            ans.second=true;
            return ans;
        }

        double dist=(z[i+1]-z[i])/sqrt(1-A*A);

        // store travel time and distance of this step.
        ans.first.first+=dist/v[i];
        ans.first.second+=dist;

        // store the path of this step.
        if (OutPutX) X.push_back(x);
        x+=A*dist;
    }
    X.push_back(x);
    D=P2;

    return ans;
}

#endif
