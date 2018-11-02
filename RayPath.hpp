#ifndef ASU_RAYPATH
#define ASU_RAYPATH

#include<iostream>
#include<vector>
#include<cmath>
#include<algorithm>

#include<LocDist.hpp>

/********************************************************************
 * This C++ template calculate the ray path for the given ray
 * parameters.
 *
 * input(s):
 * const vector<T1> &r             ----  layer radius array. r[0] is the shallowest layer.
 * const vector<T2> &v             ----  velocity array at each radius.
 * const double     &rayp          ----  ray parameter. (in sec/deg, p=Rsin/c/180*PI)
 * const double     &MinDepth      ----  depth to start the ray tracing.
 * const double     &MaxDepth      ----  depth to stop the ray tracing.
 * vector<double>   &degree        ----  Output ray path, angles. If degree[0]<-1e5, will only output degree.back();
 * std::size_t      &radius        ----  Output ray path end index in "r".
 * const double     &TurningAngle  ----  (Optional) the critical angle for turning assessment.
 *                                       default value is 89.9 deg.
 * return(s):
 * pair<pair<double,double>,bool>  ans  ----  {{travel time (s) / pursuit distance (km)}, RayTurns?}
 * vector<double> &degree (in-place)
 * std::size_t    &radius (in-place)
 *
 * Shule Yu
 * Jan 28 2018
 *
 * Key words: ray path, ray tracing, layers.
********************************************************************/

template<typename T1, typename T2>
std::pair<std::pair<double,double>,bool>
RayPath(const std::vector<T1> &r, const std::vector<T2> &v,
        const double &rayp, const double &MinDepth, const double &MaxDepth,
        std::vector<double> &degree, std::size_t &radius, const double &TurningAngle=89.9){

    // check inputs.
    double RE=6371.0;
    if (MaxDepth<=MinDepth || MaxDepth>RE-r.back() || MinDepth<RE-r[0]){
        std::cerr <<  "Error in " << __func__ << ":  MinDepth/MaxDepth input error ..." << std::endl;
        return {{-1,-1},false};
    }

    auto cmp=[](const T1 &a, const T1 &b){
        return a>=b;
    };
    if (!std::is_sorted(r.begin(),r.end(),cmp)) {
        std::cerr <<  "Error in " << __func__ << ":  layers (radius) is not strict monotonic decreasing..." << std::endl;
        return {{-1,-1},false};
    }

    // locate our start Layer and end Layer.
    // because the grid could be unevenly spaced, this is a bit lengthy;
    std::size_t P1;
    double CurMin=std::numeric_limits<double>::max();
    for (P1=0;P1<r.size();++P1) {
        double NewMin=fabs(RE-MinDepth-r[P1]);
        if (CurMin<NewMin) {--P1;break;}
        CurMin=NewMin;
    }

    std::size_t P2;
    CurMin=std::numeric_limits<double>::max();
    for (P2=0;P2<r.size();++P2) {
        double NewMin=fabs(RE-MaxDepth-r[P2]);
        if (CurMin<NewMin) {--P2;break;}
        CurMin=NewMin;
    }
    if (P2==r.size()) --P2;

    // prepare output.
    bool OutPutDegree=(degree.empty() || degree[0]>=-1e5);
    degree.clear();

    // start ray tracing.
    //
    //   B,C are angles in the same layer, B=C+D.
    //   B=sin(incident_angle on current layer);
    //   C=sin(takeoff_angle from last layer);
    //   D=sin(trun_angle);

    double deg=0,MaxAngle=sin(TurningAngle*M_PI/180),Rayp=rayp*180/M_PI;
    std::pair<std::pair<double,double>,bool> ans{{0,0},false};
    for (std::size_t i=P1;i<P2;++i){

        double B,C,D;

        B=Rayp*v[i+1]/r[i+1];
        C=Rayp*v[i+1]/r[i];
        D=B*sqrt(1-C*C)-sqrt(1-B*B)*C;

        // Judge turning.
        if (C>=1 || B>1) {
            radius=i;
            degree.push_back(deg);
            ans.second=true;
            return ans;
        }

        double dist=r[i+1]/C*D;
        if (std::isnan(dist)) dist=LocDist(0,0,r[i],asin(D)*180/M_PI,0,r[i+1]);

        // store travel time and distance of this step.
        ans.first.first+=dist/v[i+1];
        ans.first.second+=dist;

        // store the path of this step.
        if (OutPutDegree) degree.push_back(deg);
        deg+=asin(D)*180/M_PI;

        // Judge turning.
        if (B>=MaxAngle) {
            radius=i+1;
            degree.push_back(deg);
            ans.second=true;
            return ans;
        }
    }
    radius=P2;
    degree.push_back(deg);

    return ans;
}

#endif
