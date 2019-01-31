#ifndef ASU_CREATEGLOBEGRID
#define ASU_CREATEGLOBEGRID

#include<iostream>
#include<vector>
#include<cmath>

#include<CreateGrid.hpp>

/*******************************************************************
 * This C++ template returns a globe grid using the input parameters
 *
 * input(s):
 * const double &LonInc          ----  Longitude increment (roughly).
 * const double &LatInc          ----  Latitude increment (roughly).
 * const bool &ValueOnGridPoint  ----  (default is false)
 *                                     true:  will produce longitude grid points start at -180.
 *                                     false: will produce longitude grid points start at -180+LonInc/2.
 *
 * return(s):
 * pair<vector<vector<double>> Grid, pair<double,double> GridInc> ans:
 *
 * vector<vector<double>> Grid  ----  Grid[i].size() equals 3.
 *
 *                                    Grid[i][0]  --  longitude values.
 *                                    Grid[i][1]  --  latitude values.
 *                                    Grid[i][2]  equals 0.
 *
 * pair<double,double> GridInc  ----  Adjusted grid increment.
 *
 *                                    GridInc.first   --  actual longitude increment.
 *                                    GridInc.second  --  actual latitude increment.
 *
 *
 * Shule Yu
 * Jan 31 2019
 *
 * Key words: creating globe grid.
******************************************************************/

std::pair<std::vector<std::vector<double>>, std::pair<double,double>>
CreateGlobeGrid(const double &LonInc, const double &LatInc, const bool &ValueOnGridPoint=false) {

    std::pair<std::vector<std::vector<double>>,std::pair<double,double>> ans;

    auto ParaLon=CreateGrid(-180,180,LonInc,-2);
    auto ParaLat=CreateGrid(-90,90,LatInc,-2);
    std::size_t M=ParaLat[0],N=ParaLon[0]-1;

    ans.second.first=ParaLon[1];
    ans.second.second=ParaLat[1];

    ans.first=std::vector<std::vector<double>> (M*N,std::vector<double> (3,0));

    double LonBegin=-180,LatBegin=-90;
    if (!ValueOnGridPoint) LonBegin+=ans.second.first/2;
    for (std::size_t i=0;i<M*N;++i) {
        ans.first[i][0]=LonBegin+ans.second.first*(i/M);
        ans.first[i][1]=LatBegin+ans.second.second*(i%M);
    }

    return ans;
}

#endif
