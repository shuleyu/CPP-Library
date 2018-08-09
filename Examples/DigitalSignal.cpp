#include<iostream>
#include<fstream>
#include<cmath>

#include<DigitalSignal.hpp>

using namespace std;

int main(){

//     DigitalSignal A("data/ShiftPhase_in");
    DigitalSignal A("data/Signal.txt");
    A.PrintInfo();
    cout << A << endl;

//     A.Clear();
    cout << A.BeginTime() << endl;
    cout << A.EndTime() << endl;
    cout << A.SignalDuration() << endl;

    auto &B=A.GetTime();
    auto &C=A.GetAmp();
    cout << B.size() << " " << C.size() << " " << A.Size() << endl;


    cout << A.GetAmpMultiplier() << endl;
    cout << *max_element(C.begin(),C.end()) << " " << *min_element(C.begin(),C.end()) <<endl;
//
    A+=1.1;
    A-=0.1;
    A*=3;
    auto D=A+4;
    cout << A << ">" << '\n' << D;

    cout << A.GetAmpMultiplier() << endl;
    cout << *max_element(C.begin(),C.end()) << " " << *min_element(C.begin(),C.end()) <<endl;

    A.CheckAndCutToWindow(5,15);
    A.FindPeakAround(1,2);
    cout << A.PeakTime() << endl;

    A.HannTaper(1.5);

    cout << A.LocateTime(3.00001) << endl;

    auto res=D.RemoveTrend();
    cout << "RemoveTrend result: " << res.first << " " << res.second << endl;
    res=A.RemoveTrend();
    cout << "RemoveTrend result: " << res.first << " " << res.second << endl;

    cout << B[0] << " " << A.PeakTime() << endl;
    A.ShiftTime(3);
    cout << B[0] << " " << A.PeakTime() << endl;
//
    cout << A << endl << ">" << endl;
    A.ZeroOutHannTaper(1,0.5);
//
    cout << A.GetAmpMultiplier() << endl;
    A*=3;
    A.NormalizeToSignal();
    A.NormalizeToPeak();
    cout << A.GetPeak() << endl;
    cout << A.GetAmpMultiplier() << endl;
//
    cout << A.GetFileName() << endl;
//
    cout << (A.CheckWindow(1,2)?"Yes":"No") << endl;
    A.ShiftTime(3);
    cout << (A.CheckWindow(1,2)?"Yes":"No") << endl;

    cout << A.MaxAmp() << " " << A.MaxVal() << " " <<  A.MinVal() << endl;

    cout << A.BeginTime() << " " << A.EndTime() << endl;
    A.SetBeginTime(3);
    cout << A.BeginTime() << " " << A.EndTime() << endl;

    cout << A.PeakTime() << endl;
    A.ShiftTimeReferenceToPeak();
    cout << A.PeakTime() << endl;

    return 0;
}
