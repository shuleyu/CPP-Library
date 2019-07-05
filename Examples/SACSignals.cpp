#include<iostream>
#include<fstream>
#include<cmath>

#include<SACSignals.hpp>
#include<ShiftPhase.hpp>

using namespace std;

int main(){

//     SACSignals A("data/SACSignals_in");
    vector<string> infiles{
        "/home/shule/PROJ/t056.Data/201204141056/201204141056.1C.BLS.HHT.sac",
        "/home/shule/PROJ/t041.DATA/NorthAmerican/200608250044/200608250044.TA.109C.BHT.sac",
        "/home/shule/PROJ/t041.DATA/NorthAmerican/200608250044/200608250044.TA.115A.BHT.sac",
        "/home/shule/PROJ/t041.DATA/NorthAmerican/200608250044/200608250044.TA.116A.BHT.sac",
        "/home/shule/PROJ/t041.DATA/NorthAmerican/200608250044/200608250044.XQ.A03T.BHT.sac",
        "/home/shule/PROJ/t041.DATA/NorthAmerican/200608250044/200608250044.XQ.A04T.BHT.sac",
        "/home/shule/PROJ/t041.DATA/NorthAmerican/200608250044/200608250044.XQ.A09T.BHT.sac",
        "/home/shule/PROJ/t041.DATA/NorthAmerican/200608250044/200608250044.XQ.A15T.BHT.sac"
    };
    SACSignals A(infiles);


//     A.SortByGcarc();
//     auto res=A.FindByNetwork("TA");
//     for (auto item:res)
//         cout << A.GetMData()[item] << endl;

//     A.clear();

//     A.CheckDist(40,85);

//     A.HannTaper(3);
//     cout << A.dt() << endl;
//     EvenSampledSignal B("data/ShiftPhase_in");


//     A.PrintListInfo();

//     A.RemoveRecords(vector<size_t> {1});
//     auto res=A.RemoveTrend();
//     cout << "RemoveTrend first trace result: " << res[0].first << " " << res[0].second << endl;

//     A.CheckPhase("S");
//     cout << A.Size() << endl;

//     A.CheckAndCutToWindow(A.TravelTime("ScP"),-20,20);
//     cout << A.Size() << endl;

//     A.SetBeginTime(-20);
//     A.FindPeakAround(vector<double> (A.Size(),0),5);
//     A.NormalizeToSignal();



//     A.Interpolate(0.025);
//     A.Butterworth(0.033,0.3);
//     A.FindPeakAround(A.GetTravelTimes("S"));
//     A.NormalizeToPeak();
//     auto ans=A.XCorrStack(A.GetTravelTimes("S"),-15,15,5);
//     auto st=A.GetStationNames();
//     auto ESW=ans.second.first;
//     for (size_t i=0;i<A.Size();++i)
//         cout << st[i] << " " << ans.first.first[i] << " " << ans.first.second[i] << endl;
//     A.CheckAndCutToWindow(A.GetTravelTimes("S"),-50,50);
//     A.ShiftTime(A.GetTravelTimes("S"));
//     A.DumpWaveforms("/home/shule/new","StationName","201500","_","frs");

//     A.NormalizeToSignal();
    A.PrintInfo();
//     A.WaterLevelDecon(A);
//     A.WaterLevelDecon(B);
//     A.GaussianBlur(1.3);

//     auto amp=A.GetWaveforms(vector<size_t> {0});
//     auto shifted=ShiftPhase(amp[0],95.140819153);
//     for (size_t i=0;i<shifted.size();++i) {
//         size_t x=i+(int)(1.7/0.15);
//         cout << -20+i*0.15 << " " << shifted[i]+(x<shifted.size()?amp[0][x]/2.945615:0) << '\n';
//     }


//     A*=2;

//     auto S=A.MakeNeatStack();
//     cout << S << endl;

//     cout << A.GetFileList()[0] << endl;
//     A.OutputToSAC(vector<size_t> {0});

    return 0;
}
