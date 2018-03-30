#include<iostream>
#include<fstream>
#include<algorithm>

#include<Normalize.hpp>
#include<GaussianBlur.hpp>
#include<TriangleSignal.hpp>
#include<WaterLevelDecon.hpp>

using namespace std;

int main(){

    // Parameters.
    double delta=0.025,sigma=1,wl=0.1,secondarrival=3,secondamp=0.3,ulvzarrival=10,ulvzamp=0.2;

    // Find length of traces.
    int NPTS_source=1+50/delta;
    int NPTS_struct=(int)((5+2*ulvzarrival)/delta);
    int NPTS_signal=NPTS_struct+NPTS_source-1;
    int NPTS_decon=2*max(NPTS_signal,NPTS_source);

    // Make Strucuture.
    vector<double> Structure(NPTS_struct,0);
    Structure[NPTS_struct/2]=1;
    Structure[NPTS_struct/2-(int)(ulvzarrival/delta)]=-ulvzamp;
    Structure[NPTS_struct/2+(int)(ulvzarrival/delta)]=ulvzamp;


    // Make source.
//     auto y=GaussianSignal(NPTS_source,delta,sigma);
	auto y=TriangleSignal(NPTS_source,10.0/(NPTS_source*delta));
    Normalize(y);

    // Add a bump to create an unsymmetric Source.
    int P=(int)(secondarrival/delta);
    for (int i=NPTS_source-1;i>=P;--i) y[i]+=secondamp*y[i-P];
    Normalize(y);

    // Find the Source's peak.
    int py=distance(y.begin(),max_element(y.begin(),y.end()));

    // Make signal.
    vector<vector<double>> x(1,vector<double> ());
    x[0]=Convolve(Structure,y);
    Normalize(x[0]);

    // Find the Signal's peak.
    vector<int> px;
	px.push_back(distance(x[0].begin(),max_element(x[0].begin(),x[0].end())));

    // Decon.
    auto Decon=WaterLevelDecon(x,px,y,py,delta,wl);

    // Smooth and normalze.
    GaussianBlur(Decon,delta,0.1);
    Normalize(Decon[0]);


    // Output.
	ofstream fpout("data/WaterLevelDecon_out_structure");
    for (int i=0;i<NPTS_struct;++i)
        fpout << delta*(i-NPTS_struct/2) << " " << Structure[i] << '\n';
    fpout.close();

    fpout.open("data/WaterLevelDecon_out_source");
    for (int i=0;i<NPTS_source;++i)
        fpout << delta*(i-py) << " " << y[i] << '\n';
    fpout.close();

    fpout.open("data/WaterLevelDecon_out_signal");
    for (int i=0;i<NPTS_signal;++i)
        fpout << delta*(i-px[0]) << " " << x[0][i] << '\n';
    fpout.close();

    fpout.open("data/WaterLevelDecon_out_decon");
    for (int i=0;i<NPTS_decon;++i)
        fpout << delta*(i-NPTS_decon/2) << " " << Decon[0][i] << '\n';
    fpout.close();

    return 0;
}
