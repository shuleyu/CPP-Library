#include<iostream>
#include<fstream>
#include<cmath>

#include<EvenSampledSignal.hpp>

using namespace std;

int main(){

//     EvenSampledSignal A("data/Signal.txt");
//     EvenSampledSignal A("data/ShiftPhase_in");
//     EvenSampledSignal A("data/Butterworth_infile",0.01,0);
    vector<double> a(100,1),b(100,2);
    EvenSampledSignal A(a,0.04,0),B(b,0.04,1);
    A.Mask(2,3);

//     cout << A << endl;
//     A.Clear();

//     cout << A.BeginTime() << endl;
//     cout << A.EndTime() << endl;
//     A.FindPeakAround(25);
//     cout << A.PeakTime() << endl;
//     A.ShiftTime(3);
//     cout << A.SignalDuration() << endl;
//     cout << A.BeginTime() << endl;
//     A.SetBeginTime(0);
//     cout << A.BeginTime() << endl;

//     A.CheckAndCutToWindow(15,45);

//     A.FindPeakAround(25,4);
//     auto B=A;
//     B.FindPeakAround(20,4);
//     cout << A.PeakTime() << " " << B.PeakTime() << endl;
//     cout << CompareSignal(A,B) << endl;

//     cout << A.GetAmpMultiplier() << endl;

//     auto res=StackSignals({A,B},{0.5,0.8});
//     cout << res.first;
//     cout << A << endl << endl;
//     cout << A << endl;
//     cout << A.LocateTime(1.36) << endl;
//
//
//     cout << A.GetDelta() << endl;
//     cout << A.EndTime() << endl;
//     A.FindPeakAround(25,2);
//     A.HannTaper(3);
//     cout << A ;
//     A.ZeroOutHannTaper(0.5,1);
//     cout << ">\n";
//     cout << A ;

//     A.PrintInfo();
//     auto res=A.RemoveTrend();
//     cout << "RemoveTrend result: " << res.first << " " << res.second << endl;
//
//
//     cout << A.GetFileName() << endl;
//     cout << A.GetPeak() << endl;
//     cout << A.Size() << endl;
//
//     A.NormalizeToSignal();
//     cout << A.GetAmpMultiplier() << endl;
//
//     cout << A.PeakTime() << endl;
//     A.ShiftTimeReferenceToPeak();
//     cout << A.PeakTime() << endl;
//
//     cout << A.GetAmpMultiplier() << endl;
//     A.NormalizeToPeak();
//     cout << A.GetAmpMultiplier() << endl;
//
//     cout << A.GetDelta() << endl;
//
//     A.WaterLevelDecon(A);
//     A.GaussianBlur(1.3);
//     A.Butterworth(0.033,1.0/A.GetDelta());
//
//     A.Interpolate(0.02);
//     cout << A.GetDelta() << endl;

//     cout << A.SNR(-2.5,-1,-0.5,0.5) << endl;
//     cout << A.SNR(10,15,24,26) << endl;
//     A.Diff();
//     A.OutputToFile("xx");
//     A.AddSignal(B,-1.5);
    cout << A << endl;
    return 0;
}
