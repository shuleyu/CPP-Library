#include<iostream>
#include<fstream>
#include<cmath>

#include<DigitalSignal.hpp>

using namespace std;

int main(){

//     DigitalSignal A("data/ShiftPhase_in");
    DigitalSignal A("data/Signal.txt");

//     A.clear();
//     cout << A.bt() << endl;
//     cout << A.et() << endl;
//     cout << A.length() << endl;
//     cout << A.pt() << endl;
    A.CheckAndCutToWindow(5,15);
//     A.FindPeakAround(10,2);
//     A.HannTaper(3);
// 
//     A.PrintInfo();
//     auto res=A.RemoveTrend();
//     cout << "RemoveTrend result: " << res.first << " " << res.second << endl;
// 
//     cout << A.bt() << endl;
//     A.ShiftTime(3);
//     cout << A.bt() << endl;
// 
//     cout << A.filename() << endl;
//     cout << A.peak() << endl;
//     cout << A.size() << endl;
// 
//     cout << A.OriginalAmp() << endl;
//     A.NormalizeToSignal();
//     cout << A.OriginalAmp() << endl;
// 
//     cout << A.pt() << endl;
//     A.ShiftTimeRelativeToPeak();
//     cout << A.pt() << endl;
// 
//     cout << A.OriginalAmp() << endl;
//     A.NormalizeToPeak();
//     cout << A.OriginalAmp() << endl;

//     A+=3;
//     cout << (A/=2) << endl;
    return 0;
}
