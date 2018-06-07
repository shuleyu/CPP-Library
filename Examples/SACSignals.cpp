#include<iostream>
#include<fstream>
#include<cmath>

#include<SACSignals.hpp>

using namespace std;

int main(){

    SACSignals A("/home/shule/x");
//     SACSignals A("data/SACSignals_in");
//     EvenSampledSignal B("data/ShiftPhase_in");

//     A.clear();

    A.CheckDist(40,85);

//     A.HannTaper(3);
//     cout << A.dt() << endl;
//     B.Interpolate(A.dt());


//     A.PrintLessInfo();

//     A.RemoveRecords(vector<size_t> {1});
//     auto res=A.RemoveTrend();
//     cout << "RemoveTrend first trace result: " << res[0].first << " " << res[0].second << endl;

    A.CheckPhase("S");
//     cout << A.size() << endl;

    A.Interpolate(0.025);
    A.Butterworth(0.033,0.3);

    A.CheckAndCutToWindow(A.TravelTime("S"),-100,100);
//     cout << A.size() << endl;

    A.SetBeginTime(-100);
    A.FindPeakAround(vector<double> (A.size(),0),20);
    A.NormalizeToPeak();

    auto ans=A.XCorrStack(vector<double> (A.size(),0),-5,15,5);
    auto st=A.StationName();
//     auto ESW=ans.second;
//     cout << ESW << endl;
//     cout << ESW.pt() << endl;
    for (size_t i=0;i<A.size();++i)
        cout << st[i] << " " << ans.first[i].first*A.dt() << " " << ans.first[i].second << endl;

//     A.NormalizeToSignal();
//     A.PrintInfo();
//     A.WaterLevelDecon(A);
//     A.WaterLevelDecon(B);
//     A.GaussianBlur(1.3);

//     DumpWaveforms(A,"/home/shule/xx");

//     auto S=A.MakeNeatStack();
//     cout << S << endl;

    return 0;
}
