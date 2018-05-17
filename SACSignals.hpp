#ifndef ASU_SACSIGNALS
#define ASU_SACSIGNALS

#define MAXL 500000

#include<vector>
#include<string>
#include<cstring>
#include<fstream>

extern "C"{
#include<sacio.h>
#include<sac.h>
}

#include<WaterLevelDecon.hpp>

#include<Interpolate.hpp>
#include<EvenSampledSignal.hpp>

struct SACMetaData{
    std::string stnm,netwk;
    double gcarc;
    SACMetaData (const std::string &s, const std::string &n, const double &g) :
        stnm(s.substr(0,s.find_last_not_of(" \n\r\t")+1)),
        netwk(n.substr(0,n.find_last_not_of(" \n\r\t")+1)),
        gcarc(g) {}
};

class SACSignals {

private:
    std::vector<EvenSampledSignal> Data;
    std::vector<SACMetaData> MData;
    std::string FileName="";
    bool SameSamplingRate () const;
    bool SameSize () const;

public:

    // Constructor/Destructors.
    SACSignals () = default;
    SACSignals (const std::string &);
    ~SACSignals () = default;

    // Member function declarations.
    void clear() {*this=SACSignals ();}
    double dt() const {return (SameSamplingRate()?Data[0].dt():0);}
    std::size_t size() const {return Data.size();}
    std::string filename() const {return FileName;}

    void Butterworth(const double &, const double &);
    void CheckDist(const double & =-1, const double & =181);
    void GaussianBlur(const double & =1);
    void HannTaper(const double & =10);
    void Interpolate(const double &);
    void NormalizeToGlobal();
    void NormalizeToPeak();
    void NormalizeToSignal();
    void PrintInfo() const;
    void PrintLessInfo() const;
    void RemoveRecords(const std::vector<std::size_t> &);
    std::vector<std::pair<double,double>> RemoveTrend();
    void WaterLevelDecon(const EvenSampledSignal &, const double & =0.1);
    void WaterLevelDecon(SACSignals &, const double & =0.1);

    // Member function template declarations.
    template<class T> void CheckAndCutToWindow(const std::vector<T> &, const double &, const double &);
    template<class T> void FindPeakAround(const std::vector<T> &, const double & =5);
    template<class T> void ShiftTime(const std::vector<T> &);

    // friends (non-member) declarations.
    friend std::istream &operator>>(std::istream &, SACSignals &);
    friend void DumpWaveforms(const SACSignals &, const std::string &);
    friend class EvenSampledSignal;
    friend class SACMetaData;
};


// Constructors/Destructors definition.
SACSignals::SACSignals (const std::string &s){
    std::ifstream fpin(s);
    fpin >> *this;
    fpin.close();
    FileName=s;
}


// Member function definitions.

bool SACSignals::SameSamplingRate() const{
    double t=0;
    for (std::size_t i=0;i<size();++i) {
        if (i==0) t=Data[i].dt();
        else if (t!=Data[i].dt())
            return false;
    }
    return true;
}

bool SACSignals::SameSize() const{
    size_t n=0;
    for (std::size_t i=0;i<size();++i) {
        if (i==0) n=Data[i].size();
        else if (n!=Data[i].size())
            return false;
    }
    return true;
}

void SACSignals::Butterworth(const double &f1, const double &f2){
    for (std::size_t i=0;i<size();++i)
        Data[i].Butterworth(f1,f2);
}

void SACSignals::CheckDist(const double &d1, const double &d2){
    std::vector<std::size_t> BadIndices;
    for (size_t i=0;i<size();++i)
        if (MData[i].gcarc<d1 || MData[i].gcarc>d2) BadIndices.push_back(i);
    RemoveRecords(BadIndices);
}

void SACSignals::GaussianBlur(const double &sigma){
    for (std::size_t i=0;i<size();++i)
        Data[i].GaussianBlur(sigma);
}

void SACSignals::HannTaper(const double &wl) {
    for (std::size_t i=0;i<size();++i)
        Data[i].HannTaper(wl);
}

void SACSignals::Interpolate(const double &delta) {
    std::vector<EvenSampledSignal> Old_Data;
    std::swap(Data,Old_Data);
    for (std::size_t i=0;i<Old_Data.size();++i){
        EvenSampledSignal Tmp(Old_Data[i],delta);
        Data.push_back(Tmp);
    }
}

void SACSignals::NormalizeToGlobal(){
    double MaxOriginalAmp=-1;
    for (std::size_t i=0;i<size();++i)
        for (std::size_t j=0;j<Data[i].size();++j)
            MaxOriginalAmp=std::max(MaxOriginalAmp,Data[i].OriginalAmp()*fabs(Data[i].Amp[j]));

    for (std::size_t i=0;i<size();++i){
        double x=MaxOriginalAmp/Data[i].OriginalAmp();
        for (std::size_t j=0;j<Data[i].size();++j)
            Data[i].Amp[j]/=x;
        Data[i].AmpScale=MaxOriginalAmp;
    }
}

void SACSignals::NormalizeToPeak(){
    for (std::size_t i=0;i<size();++i)
        Data[i].NormalizeToPeak();
}

void SACSignals::NormalizeToSignal(){
    for (std::size_t i=0;i<size();++i)
        Data[i].NormalizeToSignal();
}

void SACSignals::PrintInfo() const {
    PrintLessInfo();
    std::cout << "====== \n";
    for (size_t i=0;i<size();++i) {
        std::cout << "Network: |" << MData[i].netwk << "|\n";
        std::cout << "StationName: |" << MData[i].stnm << "|\n";
        std::cout << "Gcarc: |" << MData[i].gcarc << "|\n";
        Data[i].PrintInfo();
        std::cout << "------ \n";
    }
}

void SACSignals::PrintLessInfo() const {
    std::cout << "Read from file: " << filename() << '\n';
    std::cout << "Current valid trace nubmer: " << size() << '\n';
}

void SACSignals::RemoveRecords(const std::vector<std::size_t> &badindices){
    for (const auto &index:badindices){
        std::swap(MData[index],MData.back());
        MData.pop_back();
        std::swap(Data[index],Data.back());
        Data.pop_back();
    }
}

std::vector<std::pair<double,double>> SACSignals::RemoveTrend(){
    std::vector<std::pair<double,double>> ans;
    for (std::size_t i=0;i<size();++i)
        ans.push_back(Data[i].RemoveTrend());
    return ans;
}

void SACSignals::WaterLevelDecon(const EvenSampledSignal &s, const double &wl){
//     if (SameSamplingRate() && SameSize()) { // Bulk mode.
// std::cout << "Bulk mode"  << std::endl;
//         if (fabs(dt()-s.dt())>1e-5)
//             throw std::runtime_error("Signal inputs of decon have different sampling rate.");
//
//         std::vector<std::vector<double>> In;
//         std::vector<std::size_t> In2;
//         for (std::size_t i=0;i<size();++i) {
//             In.push_back(Data[i].Amp);
//             In2.push_back(Data[i].peak());
//         }
//         auto ans=::WaterLevelDecon(In,In2,s.Amp,s.peak(),dt(),wl);
//         for (std::size_t i=0;i<size();++i) {
//             Data[i].Amp=ans[i];
//             Data[i].Peak=ans[i].size()/2;
//             Data[i].BeginTime=(Data[i].Peak-1)*dt()*-1;
//             Data[i].EndTime=Data[i].BeginTime+(Data[i].size()-1)*dt();
//         }
//     }
//     else {
        for (std::size_t i=0;i<size();++i)
            Data[i].WaterLevelDecon(s,wl);
//     }
}

void SACSignals::WaterLevelDecon(SACSignals &D, const double &wl){
    //check size;
    if (size()!=D.size())
        throw std::runtime_error("Waterlevel decon source signal array size doesn't match.");
    for (std::size_t i=0;i<size();++i)
        Data[i].WaterLevelDecon(D.Data[i],wl);
}

// Member template function definitions.
template<class T>
void SACSignals::CheckAndCutToWindow(const std::vector<T> &t, const double &t1, const double &t2){
    //check size;
    if (size()!=t.size())
        throw std::runtime_error("Cut reference time array size doesn't match.");
    std::vector<std::size_t> BadIndices;
    for (size_t i=0;i<size();++i)
        if (!Data[i].CheckAndCutToWindow(t[i],t1,t2)) BadIndices.push_back(i);
    RemoveRecords(BadIndices);
}

template<class T>
void SACSignals::FindPeakAround(const std::vector<T> &t, const double &wl){
    //check size;
    if (size()!=t.size())
        throw std::runtime_error("FindPeak time array size doesn't match.");
    for (std::size_t i=0;i<size();++i)
        Data[i].FindPeakAround(t[i],wl);
}

template<class T>
void SACSignals::ShiftTime(const std::vector<T> &t){
    //check size;
    if (size()!=t.size())
        throw std::runtime_error("Time shift array size doesn't match.");
    for (std::size_t i=0;i<size();++i)
        Data[i].ShiftTime(t[i]);
}


// Non-member functions.
std::istream &operator>>(std::istream &is, SACSignals &item){

    item.clear();
    std::string filename;
    char file[300],stnm[20],netwk[20],st[6]="kstnm",kt[7]="knetwk",gc[6]="gcarc";
    int rawnpts,maxl=MAXL,nerr;
    float rawbeg,rawdel,rawdata[MAXL],gcarc;
    std::vector<double> D;

    while (is >> filename){
        strcpy(file,filename.c_str());
        rsac1(file,rawdata,&rawnpts,&rawbeg,&rawdel,&maxl,&nerr,strlen(file));
        if (nerr!=0) continue;
        getkhv(st,stnm,&nerr,5,20);
        getkhv(kt,netwk,&nerr,6,20);
        getfhv(gc,&gcarc,&nerr,5);
        item.MData.push_back(SACMetaData(std::string(stnm),std::string(netwk),gcarc));

        D.resize(rawnpts);
        for (int i=0;i<rawnpts;++i) D[i]=rawdata[i];

        EvenSampledSignal Tmp(D,rawdel,rawbeg,filename);
        item.Data.push_back(Tmp);
    }

    return is;
}

void DumpWaveforms(const SACSignals &item,const std::string &dir){
    std::string c=(dir.back()=='/'?"":"/");
    for (size_t i=0;i<item.size();++i){
        std::ofstream fpout(dir+c+item.MData[i].stnm+".Num"+std::to_string(i)+".waveform");
        fpout << item.Data[i];
        fpout.close();
    }
}

#endif
