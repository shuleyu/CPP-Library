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


    A.PrintLessInfo();

//     A.RemoveRecords(vector<size_t> {1});
//     auto res=A.RemoveTrend();
//     cout << "RemoveTrend first trace result: " << res[0].first << " " << res[0].second << endl;

    A.NormalizeToSignal();
//     A.FindPeakAround(vector<double> (A.size(),10));
//     A.NormalizeToPeak();

    A.Interpolate(0.02);
    A.Butterworth(0.033,0.3);


    A.CheckPhase("S");

    A.CheckAndCutToWindow(A.TravelTime("S"),-100,100);
    A.SetBeginTime(-100);

//     A.PrintInfo();
//     A.WaterLevelDecon(A);
//     A.WaterLevelDecon(B);
//     A.GaussianBlur(1.3);

//     DumpWaveforms(A,"/home/shule/xx");

    auto S=A.MakeStack();
    cout << S << endl;

    return 0;
}
