#include<iostream>
#include<fstream>
#include<cmath>

#include<DigitalSignal.hpp>

using namespace std;

int main(){

    DigitalSignal A("data/ShiftPhase_in");
    auto res=A.RemoveTrend();
    cout << "RemoveTrend result: " << res.first << " " << res.second << endl;
    
    A.FindPeakAround((A.bt()+A.et())/2);
    A.PrintInfo();
    cout << endl;

    A.HannTaper();
    A.ShiftTimeRelativeToPeak();
    A.NormalizeToPeak();

//     cout << A << endl;

	return 0;
}
