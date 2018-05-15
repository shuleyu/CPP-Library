#include<iostream>
#include<fstream>
#include<cmath>

#include<EvenSampledSignal.hpp>

using namespace std;

int main(){

    EvenSampledSignal A("data/ShiftPhase_in");
    auto res=A.RemoveTrend();
    cout << "RemoveTrend result: " << res.first << " " << res.second << endl;

    A.FindPeakAround((A.bt()+A.et())/2);
    A.PrintInfo();

    A.HannTaper();
    A.ShiftTimeRelativeToPeak();
    A.NormalizeToPeak();

    EvenSampledSignal B=A;
    A.WaterLevelDecon(B);
    A.GaussianBlur(1);
    A.Butterworth(0.033,0.3);

//     cout << A << endl;

	return 0;
}
