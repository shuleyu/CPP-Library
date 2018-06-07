#include<iostream>
#include<fstream>
#include<algorithm>
#include<cmath>

#include<FindPeak.hpp>
#include<Normalize.hpp>
#include<GaussianBlur.hpp>
#include<Butterworth.hpp>
#include<TriangleSignal.hpp>
#include<WaterLevelDecon.hpp>

using namespace std;

int main(){

//     // Example 1.
//     // Parameters.
//     double delta=0.025,wl=0.1,secondarrival=3,secondamp=0.3,ulvzarrival=10,ulvzamp=0.2;
//
//     // Find length of traces.
//     int NPTS_source=1+50/delta;
//     int NPTS_struct=(int)((5+2*ulvzarrival)/delta);
//     int NPTS_signal=NPTS_struct+NPTS_source-1;
//     int NPTS_decon=2*max(NPTS_signal,NPTS_source);
//
//     // Make Strucuture.
//     vector<double> Structure(NPTS_struct,0);
//     Structure[NPTS_struct/2]=1;
//     Structure[NPTS_struct/2-(int)(ulvzarrival/delta)]=-ulvzamp;
//     Structure[NPTS_struct/2+(int)(ulvzarrival/delta)]=ulvzamp;
//
//
//     // Make source.
// //     double sigma=1;
// //     auto y=GaussianSignal(NPTS_source,delta,sigma);
//     auto y=TriangleSignal(NPTS_source,10.0/(NPTS_source*delta));
//     Normalize(y);
//
//     // Add a bump to create an unsymmetric Source.
//     int P=(int)(secondarrival/delta);
//     for (int i=NPTS_source-1;i>=P;--i) y[i]+=secondamp*y[i-P];
//     Normalize(y);
//
//     // Find the Source's peak.
//     int py=distance(y.begin(),max_element(y.begin(),y.end()));
//
//     // Make signal.
//     auto x=Convolve(Structure,y);
//     Normalize(x);
//
//     // Find the Signal's peak.
//     size_t px=distance(x.begin(),max_element(x.begin(),x.end()));
//
//     // Decon.
//     auto Decon=WaterLevelDecon(x,px,y,py,delta,wl);
//
//     // Smooth and normalze.
//     GaussianBlur(Decon,delta,0.1);
//     Normalize(Decon);
//
//
//     // Output.
//     ofstream fpout("data/WaterLevelDecon_out_structure");
//     for (int i=0;i<NPTS_struct;++i)
//         fpout << delta*(i-NPTS_struct/2) << " " << Structure[i] << '\n';
//     fpout.close();
//
//     fpout.open("data/WaterLevelDecon_out_source");
//     for (int i=0;i<NPTS_source;++i)
//         fpout << delta*(i-py) << " " << y[i] << '\n';
//     fpout.close();
//
//     fpout.open("data/WaterLevelDecon_out_signal");
//     for (int i=0;i<NPTS_signal;++i)
//         fpout << delta*(i-px[0]) << " " << x[0][i] << '\n';
//     fpout.close();
//
//     fpout.open("data/WaterLevelDecon_out_decon");
//     for (int i=0;i<NPTS_decon;++i)
//         fpout << delta*(i-NPTS_decon/2) << " " << Decon[0][i] << '\n';
//     fpout.close();


    // Example 2.
    ifstream fpin;
    int pSource=0,pScS=0;
    double x,y,MaxVal;
    vector<double> ScS,Source;

    fpin.open("/home/shule/PROJ/t041.CA_D/Stretch/200608250044/S.modifiedsource");
    MaxVal=numeric_limits<double>::min();
    while (fpin >> x >> y) {
        if (MaxVal<fabs(y)) {
            MaxVal=fabs(y);
            pSource=Source.size();
        }
        Source.push_back(y);
    }
    fpin.close();

    fpin.open("/home/shule/PROJ/t041.CA_D/ESF/200608250044_ScS/1/SMER.waveform");
    MaxVal=numeric_limits<double>::min();
    while (fpin >> x >> y) {
        if (MaxVal<fabs(y)) {
            MaxVal=fabs(y);
            pScS=ScS.size();
        }
        if (fabs(x)<200) ScS.push_back(y);
    }
    fpin.close();


    pSource=FindPeak(Source,pSource,-200,600);
    pScS=FindPeak(ScS,pScS,-200,600);


    auto Decon=WaterLevelDecon(ScS,pScS,Source,pSource,0.025,0.1);
    GaussianBlur(Decon,0.025,1.27398);
    Butterworth(Decon,0.025,0.03,1);
    Normalize(Decon);


    ofstream fpout;
    fpout.open("data/WaterLevelDecon_out_source");
    for (size_t i=0;i<Source.size();++i)
        fpout << -30+i*0.025 << " " << Source[i] << '\n';
    fpout.close();

    fpout.open("data/WaterLevelDecon_out_signal");
    for (size_t i=0;i<ScS.size();++i)
        fpout << -200+i*0.025 << " " << ScS[i] << '\n';
    fpout.close();

    fpout.open("data/WaterLevelDecon_out_decon");
    for (size_t i=0;i<Decon.size();++i)
        fpout << i*0.025 << " " << Decon[i] << '\n';
    fpout.close();

    return 0;
}
