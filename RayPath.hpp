#ifndef ASU_RAYPATH
#define ASU_RAYPATH

#include<iostream>
#include<vector>
#include<cmath>
#include<algorithm>

#include<LocDist.hpp>

/********************************************************************
 * This C++ template finds the ray path for the given ray parameters.
 *
 * const vector<T1> &r             ----  layer radius array. r[0] is the shallowest layer.
 * const vector<T2> &v             ----  velocity array at each radius.
 * const T3         &rayp          ----  ray parameter. (rad/sec, same unit as the output of taup)
 * const T4         &MinDepth      ----  depth to start the ray tracing.
 * const T5         &MaxDepth      ----  depth to stop the ray tracing.
 * const T6         &TurningAngle  ----  (optional) the critical angle for turning assessment.
 *                                       default value is 89.9 deg.
 *
 * returns:
 * pair<double,double>  ans      ----  travel time / pursuit distance.
 * vector<double>       &degree  ----  ray path, angles. If input is {-1e6}, will only output degree.back();
 * vector<double>       &radius  ----  ray path, radiuses. If input is {-1e6}, will only output radius.back();
 *
 * Shule Yu
 * Jan 28 2018
 *
 * Key words: ray path, ray tracing, layers.
********************************************************************/

template<class T1, class T2, class T3, class T4, class T5, class T6=double>
std::pair<double,double> RayPath(const std::vector<T1> &r, const std::vector<T2> &v,
                                 const T3 &rayp, const T4 &MinDepth, const T5 MaxDepth,
                                 std::vector<double> &degree,std::vector<double> &radius,
                                 const T6 &TurningAngle=89.9){


    // check inputs.
    double RE=6371.0;
    if (MaxDepth<=MinDepth || MaxDepth>RE-r.back() || MinDepth<RE-r[0]){
        std::cerr <<  __func__ << "; Error: MinDepth/MaxDepth input error ..." << std::endl;
        return {-1,-1};
    }

    auto cmp=[](const T1 &a, const T1 &b){
        return a>=b;
    };
    if (!std::is_sorted(r.begin(),r.end(),cmp)) {
        std::cerr <<  __func__ << "; Error: layers (radius) is not strict monotonic decreasing..." << std::endl;
        return {-1,-1};
    }

    // locate our start Layer and end Layer.
    // because the grid could be unevenly spaced, this is a bit lengthy;
    size_t P1;
    double CurMin=std::numeric_limits<double>::max();
    for (P1=0;P1<r.size();++P1) {
        double NewMin=fabs(RE-MinDepth-r[P1]);
        if (CurMin<NewMin) {--P1;break;}
        CurMin=NewMin;
    }

    size_t P2;
    CurMin=std::numeric_limits<double>::max();
    for (P2=0;P2<r.size();++P2) {
        double NewMin=fabs(RE-MaxDepth-r[P2]);
        if (CurMin<NewMin) {--P2;break;}
        CurMin=NewMin;
    }

    // prepare output.
    bool OutPutDegree=true,OutPutRadius=true;
    if (!degree.empty() && degree[0]<-1e5)
        OutPutDegree=false;
    if (!radius.empty() && radius[0]<-1e5)
        OutPutRadius=false;

    degree.clear();
    radius.clear();

    // start ray tracing.
    //   B=sin(incident_angle);
    //   C=sin(takeoff);
    //   D=sin(trun_angle);

    double deg=0,rad=r[P1],dist=0,MaxAngle=sin(TurningAngle*M_PI/180),Rayp=rayp*180/M_PI;
    std::pair<double,double> ans{0,0};
    for (size_t i=P1;i<P2;++i){

        double B,C,D;

        B=Rayp*v[i]/r[i+1];
        C=Rayp*v[i]/r[i];
        D=B*sqrt(1-C*C)-sqrt(1-B*B)*C;

        // Judge turning.
        if (B>=MaxAngle) break;

		dist=r[i+1]/C*D;
		if (std::isnan(dist)) dist=LocDist(0,0,r[i],asin(D)*180/M_PI,0,r[i+1]);

        // store travel time and distance of this step.
        ans.first+=dist/v[i];
        ans.second+=dist;

        // store the path of this step.
        if (OutPutDegree) degree.push_back(deg);
        if (OutPutRadius) radius.push_back(rad);
        deg+=asin(D)*180/M_PI;
        rad=r[i+1];
    }
    degree.push_back(deg);
    radius.push_back(rad);

    return ans;
}

#endif
