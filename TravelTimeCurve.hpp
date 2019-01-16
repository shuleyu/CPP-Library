#ifndef ASU_TRAVELTIMECURVE
#define ASU_TRAVELTIMECURVE
// Need TauP Toolkit

#include<iostream>
#include<sstream>
#include<string>
#include<vector>

#include<ShellExec.hpp>
#include<SortWithIndex.hpp>
#include<CreateGrid.hpp>
#include<Interpolate.hpp>
#include<ReorderUseIndex.hpp>

/*****************************************************
 * This C++ function use TauP toolkit to calculate and
 * post process the travel time curve for input
 * parameters.
 *
 * Using TauP toolkit. Model is PREM.
 *
 * input(s):
 * const double &EVDP          ----  source depth (in km.)
 * const string &Phase         ----  seismic phase.
 * const double &inc           ----  distance increment (in deg)
 * const bool   &FirstArrival  ----  (optional) default is true.
 *                                   true : will find the first arrivals for each distances.
 *                                   false: will return the TauP result.
 *                                   both : if there are multiple segment, they will be sorted
 *                                          according to distance.
 *
 * return(s):
 * vector<pair<vector<double>,vector<double>>> ans  ----  {Distances,ArrivalTimes}
 *
 * Shule Yu
 * Jan 08 2019
 *
 * Key words: travel time curve.
*****************************************************/

std::vector<std::pair<std::vector<double>,std::vector<double>>>
TravelTimeCurve(const double &EVDP, const std::string &Phase,
                const double &inc, const bool &FirstArrival=true){

    std::string command="taup_curve -mod prem -h "+std::to_string(EVDP)+" -ph "+Phase
                        +" -o stdout | grep -v '>' ";

    std::string res=ShellExec(command);

    if (res.empty()) {
        std::cerr <<  "Warning in " << __func__ << ": No such phase for given parameters ..." << std::endl;
        return {};
    }

    std::stringstream ss(res);
    std::vector<std::pair<std::vector<double>,std::vector<double>>> data;
    std::vector<double> dd,tt;
    std::string s1,s2;
    while (ss >> s1 >> s2){
        double d=stod(s1),t=stod(s2);
        if (dd.size()>=2 && (d-dd.back())*(dd.back()-dd[dd.size()-2])<0) {
            data.push_back({dd,tt});
            dd=std::vector<double> {dd.back()};
            tt=std::vector<double> {tt.back()};

            auto index=SortWithIndex(data.back().first.begin(),data.back().first.end());
            ReorderUseIndex(data.back().second.begin(),data.back().second.end(),index);
        }
        dd.push_back(d);
        tt.push_back(t);
    }

    data.push_back({dd,tt});
    dd=std::vector<double> {dd.back()};
    tt=std::vector<double> {tt.back()};

    auto index=SortWithIndex(data.back().first.begin(),data.back().first.end());
    ReorderUseIndex(data.back().second.begin(),data.back().second.end(),index);

    if (!FirstArrival) return data;

    //

    auto new_dist=CreateGrid(0,180,inc,2);
    std::vector<double> new_time(new_dist.size(),0.0/0.0);
    for (std::size_t i=0;i<data.size();++i){
        auto res=Interpolate(data[i].first,data[i].second,new_dist);
        for (std::size_t j=0;j<new_dist.size();++j)
            if (!std::isnan(res[j]) && (std::isnan(new_time[j]) || new_time[j]>res[j]))
                new_time[j]=res[j]; 
    }

    std::vector<std::pair<std::vector<double>,std::vector<double>>> ans;
    std::vector<double> dist,time;
    for (std::size_t i=0;i<new_dist.size();++i) {
        if (std::isnan(new_time[i])) {
            if (!dist.empty()) {
                ans.push_back({dist,time});
                dist.clear();
                time.clear();
            }
        }
        else {
            dist.push_back(new_dist[i]);
            time.push_back(new_time[i]);
        }
    }

    if (!dist.empty()) ans.push_back({dist,time});
    return ans;
}

#endif
