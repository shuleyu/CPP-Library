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
 * const double &LonInc             ----  Longitude increment (roughly).
 * const double &LatInc             ----  Latitude increment (roughly).
 * const bool &CenterValue          ----  (Optional, default is true)
 *                                           true:  will produce longitude grid points start at -180.
 *                                           false: will produce longitude grid points start at -180+LonInc/2.
 * const bool &LongitudeWrapAround  ----  (Optional, default is false)
 *                                           true:  will produce one extra repeated, wrap around longitude value (required by GMT -Rg flag)
 *                                           false: will not produce the extra repeated, wrap around longitude value.
 *
 * return(s):
 * pair<vector<vector<double>> , pair<double,double> > ans:
 *
 * vector<vector<double>> Grid   ----  Grid[i].size() equals 3.
 *
 *                                    Grid[i][0]  --  longitude values.
 *                                    Grid[i][1]  --  latitude values.
 *                                    Grid[i][2]  equals 0. (allocated area for future use.)
 *
 * vector<double>      GridInfo  ----  GridInfo.size() equals 4.
 *
 *                                    GridInfo[0]  --  longitude size.
 *                                    GridInfo[1]  --  latitude size.
 *                                    GridInfo[2]  --  actual longitude increment.
 *                                    GridInfo[3]  --  actual latitude increment.
 *
 *
 * Shule Yu
 * Jan 31 2019
 *
 * Key words: creating globe grid.
******************************************************************/

std::pair<std::vector<std::vector<double>>, std::vector<double>>
CreateGlobeGrid(const double &LonInc, const double &LatInc,
                const bool &CenterValue=true, const bool &LongitudeWrapAround=false) {

    std::pair<std::vector<std::vector<double>>,std::vector<double>> ans;

    auto ParaLat=CreateGrid(-90,90,LatInc,-2);
    auto ParaLon=CreateGrid(0,360,LonInc,-2);
    std::size_t M=ParaLat[0],N=ParaLon[0];
    if (!LongitudeWrapAround) --N;

    ans.second.push_back(N);
    ans.second.push_back(M);
    ans.second.push_back(ParaLon[1]);
    ans.second.push_back(ParaLat[1]);

    ans.first=std::vector<std::vector<double>> (M*N,std::vector<double> (3,0));

    double LonBegin=0,LatBegin=-90;
    if (CenterValue) LonBegin+=ans.second[2]/2;
    for (std::size_t i=0;i<M*N;++i) {
        ans.first[i][0]=LonBegin+ans.second[2]*(i/M);
        ans.first[i][1]=LatBegin+ans.second[3]*(i%M);
    }

    return ans;
}

#endif
