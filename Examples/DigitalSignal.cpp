#include<iostream>
#include<fstream>
#include<cmath>

#include<DigitalSignal.hpp>

using namespace std;

int main(){

    DigitalSignal A("data/ShiftPhase_in");

//     A.clear();
    cout << A.bt() << endl;
    cout << A.et() << endl;
    cout << A.length() << endl;
    cout << A.PeakTime() << endl;
    A.CheckAndCutToWindow(10,-5,5);
    A.FindPeakAround(10,2);
    A.HannTaper(3);

    A.PrintInfo();
    auto res=A.RemoveTrend();
    cout << "RemoveTrend result: " << res.first << " " << res.second << endl;

    cout << A.bt() << endl;
    A.ShiftTime(3);
    cout << A.bt() << endl;

    cout << A.filename() << endl;
    cout << A.peak() << endl;
    cout << A.size() << endl;

    cout << A.OriginalAmp() << endl;
    A.NormalizeToSignal();
    cout << A.OriginalAmp() << endl;

    cout << A.PeakTime() << endl;
    A.ShiftTimeRelativeToPeak();
    cout << A.PeakTime() << endl;

    cout << A.OriginalAmp() << endl;
    A.NormalizeToPeak();
    cout << A.OriginalAmp() << endl;

    return 0;
}
