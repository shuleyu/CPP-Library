#ifndef ASU_SACSIGNALS
#define ASU_SACSIGNALS

#define MAXL 500000

#include<vector>
#include<string>
#include<cstring>
#include<fstream>
#include<map>
#include<cstdio>
#include<fcntl.h>
#include<unistd.h>

extern "C"{
#include<sacio.h>
#include<sac.h>
}

#include<SortWithIndex.hpp>
#include<ReorderUseIndex.hpp>
#include<WaterLevelDecon.hpp>
#include<Interpolate.hpp>
#include<EvenSampledSignal.hpp>

struct SACMetaData{
    std::string stnm,netwk;
    double gcarc;
    std::map<std::string,double> tt;
    SACMetaData (const std::string &s, const std::string &n, const double &g, const std::map<std::string,double> &m) : stnm(s),netwk(n),gcarc(g),tt(m) {}
};

class SACSignals {

private:
    std::vector<EvenSampledSignal> Data;
    std::vector<SACMetaData> MData;
    std::string FileName="",SortedBy="None";

public:

    // Constructor/Destructors.
    SACSignals () = default;
    SACSignals (const std::string &infile);
    ~SACSignals () = default;

    // Member function declarations.
    void clear() {*this=SACSignals ();}
    double dt() const {return (SameSamplingRate()?Data[0].dt():0);}
    std::size_t size() const {return Data.size();}
    std::string filename() const {return FileName;}

    void Butterworth(const double &f1, const double &f2, const int &order=2, const int &passes=2);
    std::vector<double> bt() const;
    void CheckDist(const double &d1=-1, const double &d2=181);
    void CheckPhase(const std::string &phase);
    std::vector<double> Distance() const;
    std::vector<std::string> File() const;
    void GaussianBlur(const double &sigma=1);
    std::vector<std::pair<std::vector<double>,std::vector<double>>> GetTimeAndWaveforms() const;
    std::vector<std::vector<double>> GetWaveforms() const;
    std::vector<std::vector<double>> GetWaveforms(const std::vector<std::size_t> &bi) const;
    std::vector<std::vector<double>> GetWaveforms(const std::string &Stnm) const;
    void HannTaper(const double &wl=10);
    void Integrate();
    void Interpolate(const double &delta);
    EvenSampledSignal MakeNeatStack() const;
    std::vector<std::string> NetworkName() const;
    void NormalizeToGlobal();
    void NormalizeToPeak();
    void NormalizeToSignal();
    std::vector<double> PeakTime() const;
    void PrintInfo() const;
    void PrintLessInfo() const;
    void RemoveRecords(std::vector<std::size_t> bi);
    std::vector<std::pair<double,double>> RemoveTrend();
    bool SameSamplingRate () const;
    bool SameSize () const;
    void SetBeginTime(const double &t);
    void SortByGcarc();
    void SortByStnm();
    std::vector<std::string> StationName() const;
    std::vector<double> TravelTime(const std::string &phase) const;
    void WaterLevelDecon(const EvenSampledSignal &s, const double &wl=0.1);
    void WaterLevelDecon(SACSignals &D, const double &wl=0.1);
    std::pair<std::vector<std::pair<int,double>>,EvenSampledSignal> XCorrStack(const std::vector<double> &T, const double &t1, const double &t2, const int loopN) const;

    // Member function template declarations.
    template<class T> void CheckAndCutToWindow(const std::vector<T> &t, const double &t1, const double &t2);
    template<class T> void FindPeakAround(const std::vector<T> &t, const double &wl=5);
    template<class T> void ShiftTime(const std::vector<T> &t);

    // friends (non-member) declarations.
    friend std::istream &operator>>(std::istream &is, SACSignals &item);
    friend void DumpWaveforms(const SACSignals &item, const std::string &dir="./");
    friend class EvenSampledSignal;
    friend class SACMetaData;
};


// Constructors/Destructors definition.
SACSignals::SACSignals (const std::string &infile){
    std::ifstream fpin(infile);
    fpin >> *this;
    fpin.close();
    FileName=infile;
    SortedBy="None";
}


// Member function definitions.

void SACSignals::Butterworth(const double &f1, const double &f2, const int &order, const int &passes){
    for (std::size_t i=0;i<size();++i)
        Data[i].Butterworth(f1,f2,order,passes);
}

std::vector<double> SACSignals::bt() const {
    std::vector<double> ans;
    for (size_t i=0;i<size();++i) ans.push_back(Data[i].bt());
    return ans;
}

void SACSignals::CheckDist(const double &d1, const double &d2){
    std::vector<std::size_t> BadIndices;
    for (size_t i=0;i<size();++i)
        if (MData[i].gcarc<d1 || MData[i].gcarc>d2) BadIndices.push_back(i);
    RemoveRecords(BadIndices);
}

void SACSignals::CheckPhase(const std::string &phase){
    std::vector<std::size_t> BadIndices;
    for (size_t i=0;i<size();++i)
        if (MData[i].tt.find(phase)==MData[i].tt.end() || MData[i].tt[phase]<0) BadIndices.push_back(i);
    RemoveRecords(BadIndices);
}

std::vector<double> SACSignals::Distance() const{
    std::vector<double> ans;
    for (const auto &item:MData)
        ans.push_back(item.gcarc);
    return ans;
}

std::vector<std::string> SACSignals::File() const{
    std::vector<std::string> ans;
    for (const auto &item:Data)
        ans.push_back(item.filename());
    return ans;
}

void SACSignals::GaussianBlur(const double &sigma){
    for (std::size_t i=0;i<size();++i)
        Data[i].GaussianBlur(sigma);
}

std::vector<double> SACSignals::PeakTime() const{
    std::vector<double> ans;
    for (size_t i=0;i<size();++i)
        ans.push_back(Data[i].pt());
    return ans;
}

std::vector<std::pair<std::vector<double>,std::vector<double>>> SACSignals::GetTimeAndWaveforms() const {
    std::vector<std::pair<std::vector<double>,std::vector<double>>> ans;
    for (size_t i=0;i<size();++i) ans.push_back({Data[i].Time(),Data[i].Amp});
    return ans;
}

std::vector<std::vector<double>> SACSignals::GetWaveforms() const {
    std::vector<std::vector<double>> ans;
    for (size_t i=0;i<size();++i) ans.push_back(Data[i].Amp);
    return ans;
}

std::vector<std::vector<double>> SACSignals::GetWaveforms(const std::vector<std::size_t> &bi) const {
    std::vector<std::vector<double>> ans;
    for (const auto &index:bi) {
        if (index>=size()) continue;
        ans.push_back(Data[index].Amp);
    }
    return ans;
}

std::vector<std::vector<double>> SACSignals::GetWaveforms(const std::string &Stnm) const {
    std::vector<std::vector<double>> ans;
    for (size_t i=0;i<size();++i)
        if (MData[i].stnm==Stnm) ans.push_back(Data[i].Amp);
    return ans;
}

void SACSignals::HannTaper(const double &wl) {
    for (std::size_t i=0;i<size();++i)
        Data[i].HannTaper(wl);
}

void SACSignals::Integrate() {
    for (std::size_t i=0;i<size();++i)
        Data[i].Integrate();
}

void SACSignals::Interpolate(const double &delta) {
    std::vector<EvenSampledSignal> Old_Data;
    std::swap(Data,Old_Data);
    for (std::size_t i=0;i<Old_Data.size();++i){
        EvenSampledSignal Tmp(Old_Data[i],delta);
        Data.push_back(Tmp);
    }
}

EvenSampledSignal SACSignals::MakeNeatStack() const {
    EvenSampledSignal ans;
    if (size()==0) return ans;
    ans=Data[0];
    ans.Peak=0;ans.FileName="";ans.AmpMultiplier=1.0;
    for (size_t i=1;i<size();++i) ans+=Data[i];
    return ans;
}

std::vector<std::string> SACSignals::NetworkName() const{
    std::vector<std::string> ans;
    for (const auto &item:MData)
        ans.push_back(item.netwk);
    return ans;
}

void SACSignals::NormalizeToGlobal(){
    double MaxOriginalAmp=-1;
    for (std::size_t i=0;i<size();++i)
        for (std::size_t j=0;j<Data[i].size();++j)
            MaxOriginalAmp=std::max(MaxOriginalAmp,Data[i].GetAmpMultiplier()*fabs(Data[i].Amp[j]));

    for (std::size_t i=0;i<size();++i){
        double x=MaxOriginalAmp/Data[i].GetAmpMultiplier();
        for (std::size_t j=0;j<Data[i].size();++j)
            Data[i].Amp[j]/=x;
        Data[i].AmpMultiplier=MaxOriginalAmp;
    }
}

// Notice this could flip the polarity of some of the signals.
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
        std::cout << "TravelTimes: " << '\n';
        for (const auto &item:MData[i].tt)
            std::cout << "    " << item.first << "  -  " << item.second << " sec.\n";
        Data[i].PrintInfo();
        std::cout << "------ \n";
    }
}

void SACSignals::PrintLessInfo() const {
    std::cout << "Read from file: " << filename() << '\n';
    std::cout << "Current valid trace nubmer: " << size() << '\n';
}

void SACSignals::RemoveRecords(std::vector<std::size_t> bi){
    std::sort(bi.begin(),bi.end(),std::greater<std::size_t>());
    for (const auto &index:bi){
        std::swap(MData[index],MData.back());
        MData.pop_back();
        std::swap(Data[index],Data.back());
        Data.pop_back();
    }
    SortedBy="None";
}

std::vector<std::pair<double,double>> SACSignals::RemoveTrend(){
    std::vector<std::pair<double,double>> ans;
    for (std::size_t i=0;i<size();++i)
        ans.push_back(Data[i].RemoveTrend());
    return ans;
}

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

void SACSignals::SetBeginTime(const double &t){
    auto A=bt();
    for (auto &item:A) item=-item+t;
    ShiftTime(A);
}

void SACSignals::SortByGcarc() {
    if (SortedBy=="Gcarc") return;
    std::vector<double> A;
    for (std::size_t i=0;i<size();++i) A.push_back(MData[i].gcarc);
    auto res=SortWithIndex(A.begin(),A.end());
    ReorderUseIndex(Data.begin(),Data.end(),res);
    ReorderUseIndex(MData.begin(),MData.end(),res);
    SortedBy="Gcarc";
    return;
}

void SACSignals::SortByStnm() {
    if (SortedBy=="Stnm") return;
    std::vector<std::string> A;
    for (std::size_t i=0;i<size();++i) A.push_back(MData[i].stnm);
    auto res=SortWithIndex(A.begin(),A.end());
    ReorderUseIndex(Data.begin(),Data.end(),res);
    ReorderUseIndex(MData.begin(),MData.end(),res);
    SortedBy="Stnm";
    return;
}

std::vector<std::string> SACSignals::StationName() const{
    std::vector<std::string> ans;
    for (const auto &item:MData)
        ans.push_back(item.stnm);
    return ans;
}

std::vector<double> SACSignals::TravelTime(const std::string &phase) const{
    std::vector<double> ans;
    for (const auto &item:MData) {
        if (item.tt.find(phase)==item.tt.end()) ans.push_back(-1);
        else ans.push_back(item.tt.at(phase));
    }
    return ans;
}

void SACSignals::WaterLevelDecon(const EvenSampledSignal &s, const double &wl){
    for (std::size_t i=0;i<size();++i)
        Data[i].WaterLevelDecon(s,wl);
}

void SACSignals::WaterLevelDecon(SACSignals &D, const double &wl){
    //check size;
    if (size()!=D.size())
        throw std::runtime_error("Waterlevel decon source signal array size doesn't match.");
    for (std::size_t i=0;i<size();++i)
        Data[i].WaterLevelDecon(D.Data[i],wl);
}

std::pair<std::vector<std::pair<int,double>>,EvenSampledSignal> SACSignals::XCorrStack(const std::vector<double> &T, const double &t1, const double &t2, const int loopN) const{

    if (!SameSamplingRate()) throw std::runtime_error("Tried to XCorrStack differently sampled signals.");
    if (T.size()!=size()) throw std::runtime_error("XCorrStack number of signals doesn't agree with nubmer of time windows.");
    if (t1>=t2) throw std::runtime_error("XCorrStack window length <=0.");

    // will return {shift amount, ccc}s and the stack (length is t2-t1).
    // If the trace is not used in the satck, shift==-1.

    std::vector<std::pair<int,double>> ans;
    if (size()==0) return {ans,EvenSampledSignal()};


    // prepare output.
    ans.resize(size(),{-1,0});

    // Find the good records which has waveform avaliable within its XCorrStack window.
    std::vector<size_t> GoodIndex;
    for (size_t i=0;i<size();++i)
        if (Data[i].CheckWindow(T[i]+t1,T[i]+t2))
            GoodIndex.push_back(i);

    // First stack: directly stack the windowed section.

    EvenSampledSignal S0;
    for (const auto &i:GoodIndex) {
        if (S0.size()==0) {
            S0=EvenSampledSignal(Data[i],T[i]+t1,T[i]+t2);
            S0.SetBeginTime(t1);
        }
        else {
            auto Tmp=EvenSampledSignal(Data[i],T[i]+t1,T[i]+t2);
            Tmp.SetBeginTime(t1);
            S0+=Tmp;
        }
    }

    // Do the cross-correlation loop.
    EvenSampledSignal S_prev=S0,S_cur;
    double WeightSum=0.0;
    for (int loop=0;loop<loopN;++loop){
        WeightSum=0;
        for (const auto &i:GoodIndex) {
            auto res=CrossCorrelation(S_prev,t1,t2,Data[i],T[i]+t1,T[i]+t2);

            ans[i].first=-res.first.first;
            double st=res.first.first*dt();

            ans[i].second=res.first.second;
            WeightSum+=res.first.second;

            if (S_cur.size()==0) {
                S_cur=EvenSampledSignal(Data[i],T[i]-st+t1,T[i]-st+t2);
                S_cur.SetBeginTime(t1);
                S_cur*=res.first.second;
            }
            else {
                auto Tmp=EvenSampledSignal(Data[i],T[i]-st+t1,T[i]-st+t2);
                Tmp.SetBeginTime(t1);
                S_cur+=Tmp*res.first.second;
            }
        }
        S_prev=S_cur;
        S_cur.clear();
    }
    S_prev.NormalizeToSignal();

    return {ans,S_prev};
}

// Member template function definitions.
template<class T>
void SACSignals::CheckAndCutToWindow(const std::vector<T> &t, const double &t1, const double &t2){
    //check size;
    if (size()!=t.size())
        throw std::runtime_error("Cut reference time array size doesn't match.");
    std::vector<std::size_t> BadIndices;
    for (size_t i=0;i<size();++i)
        if (!Data[i].CheckAndCutToWindow(t[i]+t1,t[i]+t2)) BadIndices.push_back(i);
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
    char file[300],stnm1[20],netwk1[20],p[20];
    char st[6]="kstnm",kt[7]="knetwk",gc[6]="gcarc";
    char tname[][3]={"t0","t1","t2","t3","t4","t5","t6","t7","t8","t9"};
    char pname[][4]={"kt0","kt1","kt2","kt3","kt4","kt5","kt6","kt7","kt8","kt9"};
    std::string stnm,netwk;
    std::map<std::string,double> M;
    int rawnpts,maxl=MAXL,nerr;
    float rawbeg,rawdel,rawdata[MAXL],gcarc,t;
    std::vector<double> D;

    while (is >> filename){
        strcpy(file,filename.c_str());
        rsac1(file,rawdata,&rawnpts,&rawbeg,&rawdel,&maxl,&nerr,strlen(file));
        if (nerr!=0) continue; // ignore unevenly sampled records.

        D.resize(rawnpts);
        for (int i=0;i<rawnpts;++i) D[i]=rawdata[i];

        EvenSampledSignal Tmp(D,rawdel,rawbeg,filename);
        item.Data.push_back(Tmp);

        // deal with headers, we only pull these headers:
        // network code, station code, gcarc, traveltimes.
        M.clear();

// hide warning output.
const int stdoutfd(dup(fileno(stdout)));
int newstdout = open("/dev/null", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
dup2(newstdout, fileno(stdout));
close(newstdout);

        getkhv(st,stnm1,&nerr,5,20);
        std::string tmpstr(stnm1);
        stnm=tmpstr.substr(0,tmpstr.find_first_of(" \n\r\t"));

        getkhv(kt,netwk1,&nerr,6,20);
        tmpstr=std::string(netwk1);
        netwk=tmpstr.substr(0,tmpstr.find_first_of(" \n\r\t"));

        for (size_t i=0;i<10;++i) {
            getfhv(tname[i],&t,&nerr,2);
            getkhv(pname[i],p,&nerr,3,20);
            std::string tmpstr(p);
            tmpstr=tmpstr.substr(0,tmpstr.find_first_of(" \n\r\t"));
            if (tmpstr!="-12345") M[tmpstr]=t;
        }
        getfhv(gc,&gcarc,&nerr,5);

// hide SAC warning output.
fflush(stdout);
dup2(stdoutfd, fileno(stdout));
close(stdoutfd);

        item.MData.push_back(SACMetaData(stnm,netwk,gcarc,M));
    }

    return is;
}

void DumpWaveforms(const SACSignals &item,const std::string &dir){
    std::string c=(dir.back()=='/'?"":"/");
    for (size_t i=0;i<item.size();++i){
        auto s=item.Data[i].filename();
        s=s.substr(s.find_last_of('/')+1);
        std::ofstream fpout(dir+c+s+".txt");
        fpout << item.Data[i];
        fpout.close();
    }
}

#endif
