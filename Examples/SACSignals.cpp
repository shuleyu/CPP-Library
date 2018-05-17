#include<iostream>
#include<fstream>
#include<cmath>

#include<SACSignals.hpp>

using namespace std;

int main(){

    SACSignals A("data/SACSignals_in");
    EvenSampledSignal B("data/ShiftPhase_in");

//     A.clear();
    cout << A.size() << endl;
    cout << A.filename() << endl;

    A.CheckDist();

    A.HannTaper(3);
//     A.Interpolate(0.02);
//     A.CheckAndCutToWindow(vector<float> (A.size(),100),-100,200);
//     cout << A.dt() << endl;
//     B.Interpolate(A.dt());


//     A.PrintLessInfo();

//     A.RemoveRecords(vector<size_t> {1});
//     auto res=A.RemoveTrend();
//     cout << "RemoveTrend first trace result: " << res[0].first << " " << res[0].second << endl;

//     A.NormalizeToGlobal();
//     A.FindPeakAround(vector<double> (A.size(),10));
//     A.NormalizeToPeak();
//     A.NormalizeToSignal();

//     A.ShiftTime(vector<double> (A.size(),3));

//     A.PrintInfo();
//     A.WaterLevelDecon(A);
//     A.WaterLevelDecon(B);
//     A.GaussianBlur(1.3);
//     A.Butterworth(0.033,100);

    DumpWaveforms(A,"/home/shule/xx");

	return 0;
}
