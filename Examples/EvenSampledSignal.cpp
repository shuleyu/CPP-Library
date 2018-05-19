#include<iostream>
#include<fstream>
#include<cmath>

#include<EvenSampledSignal.hpp>

using namespace std;

int main(){

    EvenSampledSignal A("data/ShiftPhase_in");

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

    cout << A.dt() << endl;

    A.WaterLevelDecon(A);
    A.GaussianBlur(1.3);
    A.Butterworth(0.033,1.0/A.dt());

    A.Interpolate(0.02);
    cout << A.dt() << endl;

    return 0;
}
