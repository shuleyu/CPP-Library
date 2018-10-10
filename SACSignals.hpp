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

// Todos:
// MetaData add event, depth, etc. header information.
// Only read-in meta data for station selection.

struct SACMetaData{
    std::string stnm,network;
    double gcarc;
    std::map<std::string,double> tt;           // all travel times avaliable in the sac file header.
    SACMetaData (const std::string &s, const std::string &n, const double &g,
                 const std::map<std::string,double> &m) : stnm(s),network(n),gcarc(g),tt(m) {}
};

std::ostream &operator<<(std::ostream &os, const SACMetaData &item){
    std::cout << "Network: |" << item.network << "|\n";
    std::cout << "StationName: |" << item.stnm << "|\n";
    std::cout << "Gcarc: |" << item.gcarc << "|\n";
    std::cout << "TravelTimes: " << '\n';
    for (const auto &item2:item.tt)
        std::cout << "    " << item2.first << "  -  " << item2.second << " sec.\n";
    return os;
}

class SACSignals {

private:
    std::vector<EvenSampledSignal> data;
    std::vector<SACMetaData> mdata;
    std::string file_list_name,sorted_by;

public:

    // Constructor/Destructors.
    SACSignals ();
    SACSignals (const std::string &infile);
    ~SACSignals () = default;

    // Member function declarations.
    void Clear() {*this=SACSignals();}
    double GetDelta() const {return (SameSamplingRate()?data[0].GetDelta():0);}
    const std::vector<EvenSampledSignal> &GetData() const {return data;}
    const std::vector<SACMetaData> &GetMData() const {return mdata;}
    std::size_t Size() const {return data.size();}
    std::string GetFileListName() const {return file_list_name;}

    void Butterworth(const double &f1, const double &f2, const int &order=2, const int &passes=2);
    std::vector<double> BeginTime() const;
    void CheckDist(const double &d1=-1, const double &d2=181);
    void CheckPhase(const std::string &phase, const double &t1=0,
                    const double &t2=std::numeric_limits<double>::max());
    std::vector<std::size_t> FindByGcarc(const double &gc, const bool &bulk=false);
    std::vector<std::size_t> FindByStnm(const std::string &st, const bool &bulk=false);
    std::vector<std::size_t> FindByNetwork(const std::string &nt, const bool &bulk=false);
    void GaussianBlur(const double &sigma=1);
    std::vector<double> GetDistance() const;
    std::vector<std::string> GetFileList() const;
    std::vector<std::string> GetNetworkNames() const;
    std::vector<std::string> GetStationNames() const;
    std::vector<double> GetTravelTimes(const std::string &phase) const;
    std::vector<std::pair<std::vector<double>,std::vector<double>>>
    GetTimeAndWaveforms(const std::vector<std::size_t> &indices=std::vector<std::size_t> ()) const;
    std::vector<std::vector<double>>
    GetWaveforms(const std::vector<std::size_t> &indices=std::vector<std::size_t> ()) const;
    void HannTaper(const double &wl=10);
    void Integrate();
    void Interpolate(const double &dt);
    EvenSampledSignal MakeNeatStack() const;
    void NormalizeToGlobal();
    void NormalizeToPeak();
    void NormalizeToSignal();
    void OutputToSAC(const std::vector<std::size_t> &indices=std::vector<std::size_t> (),
                     const std::string &prefix="") const;
    std::vector<double> PeakTime() const;
    void PrintInfo() const;
    void PrintListInfo() const;
    void RemoveRecords(const std::vector<std::size_t> &indices);
    std::vector<std::pair<double,double>> RemoveTrend();
    bool SameSamplingRate () const;
    bool SameSize () const;
    void SetBeginTime(const double &t);
    void SortByGcarc();
    void SortByNetwork();
    void SortByStnm();
    std::vector<double> SNR(const double &nt1, const double &nt2,
                            const double &st1, const double &st2,
                            const std::vector<double> &na=std::vector<double> (),
                            const std::vector<double> &sa=std::vector<double> ()) const;
    void WaterLevelDecon(const EvenSampledSignal &s, const double &wl=0.1);
    void WaterLevelDecon(SACSignals &D, const double &wl=0.1);

    // Member function template declarations.
    template<class T> void CheckAndCutToWindow(const std::vector<T> &center_time,
                                               const double &t1, const double &t2);
    template<class T> void FindPeakAround(const std::vector<T> &center_time, const double &wl=5);
    template<class T> void ShiftTime(const std::vector<T> &t);
    template<class T>
    std::pair<std::pair<std::vector<int>,std::vector<double>>,
              std::pair<EvenSampledSignal,EvenSampledSignal>>
    XCorrStack(const std::vector<T> &center_time,
               const double &t1, const double &t2, const int loopN=5) const;

    // friends (non-member) declarations.
    friend std::istream &operator>>(std::istream &is, SACSignals &item);
};


// Constructors/Destructors definition.
SACSignals::SACSignals (){
    file_list_name="";
    sorted_by="None";
}


SACSignals::SACSignals (const std::string &infile){
    std::ifstream fpin(infile);
    fpin >> *this;
    fpin.close();
    file_list_name=infile;
    sorted_by="None";
}


// Member function definitions.

void SACSignals::Butterworth(const double &f1, const double &f2,
                             const int &order, const int &passes){
    for (std::size_t i=0;i<Size();++i)
        data[i].Butterworth(f1,f2,order,passes);
}

std::vector<double> SACSignals::BeginTime() const {
    std::vector<double> ans;
    for (size_t i=0;i<Size();++i) ans.push_back(data[i].BeginTime());
    return ans;
}

void SACSignals::CheckDist(const double &d1, const double &d2){
    std::vector<std::size_t> BadIndices;
    for (size_t i=0;i<Size();++i)
        if (mdata[i].gcarc<d1 || mdata[i].gcarc>d2) BadIndices.push_back(i);
    RemoveRecords(BadIndices);
}

void SACSignals::CheckPhase(const std::string &phase, const double &t1, const double &t2){
    std::vector<std::size_t> BadIndices;
    for (size_t i=0;i<Size();++i) {
        auto it=mdata[i].tt.find(phase);
        if (it==mdata[i].tt.end() || it->second<t1 || it->second>t2 )
            BadIndices.push_back(i);
    }
    RemoveRecords(BadIndices);
}

std::vector<std::size_t> SACSignals::FindByGcarc(const double &gc, const bool &bulk) {
    std::vector<std::size_t> ans;
    if (sorted_by=="Gcarc") {

        if (Size()==1) {
            ans.push_back(0);
            return ans;
        }

        auto cmp=[](const SACMetaData &m1, const SACMetaData &m2){
            return m1.gcarc<m2.gcarc;
        };
        SACMetaData dummy("","",gc,std::map<std::string, double> ());
        auto it1=std::lower_bound(mdata.begin(),mdata.end(),dummy,cmp);
        auto it2=std::upper_bound(mdata.begin(),mdata.end(),dummy,cmp);
        if (it1==it2) {
            if (it1==mdata.end()) ans.push_back(std::distance(mdata.begin(),std::prev(it1)));
            else if (it1==mdata.begin()) ans.push_back(0);
            else {
                if (fabs(it1->gcarc-gc)<fabs(std::prev(it1)->gcarc-gc))
                    ans.push_back(std::distance(mdata.begin(),it1));
                else
                    ans.push_back(std::distance(mdata.begin(),std::prev(it1)));
            }
        }
        else {
            for (auto it=it1;it!=it2;++it)
                ans.push_back(std::distance(mdata.begin(),it));
        }
        return ans;
    }

    if (bulk) {
        SortByGcarc();
        return FindByGcarc(gc);
    }

    double MinDiff=std::numeric_limits<double>::max();
    for (std::size_t i=0;i<Size();++i) {
        double diff=fabs(mdata[i].gcarc-gc);
        if (MinDiff>diff) {
            ans.clear();
            MinDiff=diff;
            ans.push_back(i);
        }
        else if (MinDiff==diff) ans.push_back(i);
    }
    return ans;
}

std::vector<std::size_t> SACSignals::FindByNetwork(const std::string &nt, const bool &bulk) {
    std::vector<std::size_t> ans;
    if (sorted_by=="Network") {
        auto cmp=[](const SACMetaData &m1, const SACMetaData &m2){
            return m1.network<m2.network;
        };
        SACMetaData dummy("",nt,0,std::map<std::string, double> ());
        auto it1=std::lower_bound(mdata.begin(),mdata.end(),dummy,cmp);
        auto it2=std::upper_bound(mdata.begin(),mdata.end(),dummy,cmp);
        for (auto it=it1;it!=it2;++it)
            ans.push_back(std::distance(mdata.begin(),it));
        return ans;
    }

    if (bulk) {
        SortByNetwork();
        return FindByNetwork(nt);
    }

    for (std::size_t i=0;i<Size();++i)
        if (mdata[i].network==nt)
            ans.push_back(i);
    return ans;
}

std::vector<std::size_t> SACSignals::FindByStnm(const std::string &st, const bool &bulk) {
    std::vector<std::size_t> ans;
    if (sorted_by=="Stnm") {
        auto cmp=[](const SACMetaData &m1, const SACMetaData &m2){
            return m1.stnm<m2.stnm;
        };
        SACMetaData dummy(st,"",0,std::map<std::string, double> ());
        auto it1=std::lower_bound(mdata.begin(),mdata.end(),dummy,cmp);
        auto it2=std::upper_bound(mdata.begin(),mdata.end(),dummy,cmp);
        for (auto it=it1;it!=it2;++it)
            ans.push_back(std::distance(mdata.begin(),it));
        return ans;
    }

    if (bulk) {
        SortByStnm();
        return FindByStnm(st);
    }

    for (std::size_t i=0;i<Size();++i)
        if (mdata[i].stnm==st)
            ans.push_back(i);
    return ans;
}

void SACSignals::GaussianBlur(const double &sigma){
    for (std::size_t i=0;i<Size();++i)
        data[i].GaussianBlur(sigma);
}

std::vector<double> SACSignals::GetDistance() const{
    std::vector<double> ans;
    for (const auto &item:mdata)
        ans.push_back(item.gcarc);
    return ans;
}

std::vector<std::string> SACSignals::GetFileList() const{
    std::vector<std::string> ans;
    for (const auto &item:data)
        ans.push_back(item.GetFileName());
    return ans;
}

std::vector<std::string> SACSignals::GetNetworkNames() const{
    std::vector<std::string> ans;
    for (const auto &item:mdata)
        ans.push_back(item.network);
    return ans;
}


std::vector<std::string> SACSignals::GetStationNames() const{
    std::vector<std::string> ans;
    for (const auto &item:mdata)
        ans.push_back(item.stnm);
    return ans;
}


std::vector<double> SACSignals::GetTravelTimes(const std::string &phase) const{
    std::vector<double> ans;
    for (const auto &item:mdata) {
        auto it=item.tt.find(phase);
        if (it==item.tt.end()) ans.push_back(-1);
        else ans.push_back(it->second);
    }
    return ans;
}


std::vector<std::pair<std::vector<double>,std::vector<double>>>
SACSignals::GetTimeAndWaveforms(const std::vector<std::size_t> &indices) const {
    std::vector<std::pair<std::vector<double>,std::vector<double>>> ans;
    if (indices.empty())
        for (size_t i=0;i<Size();++i)
            ans.push_back({data[i].GetTime(),data[i].GetAmp()});
    else
        for (const auto &i:indices) {
            if (i>=Size()) continue;
            ans.push_back({data[i].GetTime(),data[i].GetAmp()});
        }
    return ans;
}

std::vector<std::vector<double>>
SACSignals::GetWaveforms(const std::vector<std::size_t> &indices) const {
    std::vector<std::vector<double>> ans;
    if (indices.empty())
        for (size_t i=0;i<Size();++i)
            ans.push_back(data[i].GetAmp());
    else
        for (const auto &i:indices) {
            if (i>=Size()) continue;
            ans.push_back(data[i].GetAmp());
        }
    return ans;
}

void SACSignals::HannTaper(const double &wl) {
    for (std::size_t i=0;i<Size();++i)
        data[i].HannTaper(wl);
}

void SACSignals::Integrate() {
    for (std::size_t i=0;i<Size();++i)
        data[i].Integrate();
}

void SACSignals::Interpolate(const double &dt) {
    std::vector<EvenSampledSignal> old_data;
    std::swap(data,old_data);
    for (const EvenSampledSignal &item:old_data)
        data.push_back(EvenSampledSignal(item,dt));
}

EvenSampledSignal SACSignals::MakeNeatStack() const {
    EvenSampledSignal ans;
    if (Size()==0) return ans;
    for (size_t i=0;i<Size();++i) ans+=data[i];
    return ans;
}

void SACSignals::NormalizeToGlobal(){
    double MaxOriginalAmp=-1;
    for (std::size_t i=0;i<Size();++i)
        MaxOriginalAmp=std::max(MaxOriginalAmp,data[i].MaxAmp()*fabs(data[i].GetAmpMultiplier()));

    for (std::size_t i=0;i<Size();++i){
        double x=MaxOriginalAmp/fabs(data[i].GetAmpMultiplier());
        data[i]/=x;
    }
}

// Notice this could flip the polarity of some of the signals.
void SACSignals::NormalizeToPeak(){
    for (std::size_t i=0;i<Size();++i)
        data[i].NormalizeToPeak();
}

void SACSignals::NormalizeToSignal(){
    for (std::size_t i=0;i<Size();++i)
        data[i].NormalizeToSignal();
}

void SACSignals::OutputToSAC(const std::vector<std::size_t> &indices,
                             const std::string &prefix) const {
    std::vector<std::size_t> ind;
    if (indices.empty()) {
        ind.resize(Size());
        for (std::size_t i=0;i<Size();++i) ind[i]=i;
    }
    else ind=indices;

    char tmpfilename[300];
    int npts,nerr,prev_size=0;
    float dt,bt,*amp=nullptr;
    for (const size_t &i:ind) {

        std::string outfile=prefix+std::to_string(i)+".sac";
        strcpy(tmpfilename,outfile.c_str());
        npts=data[i].Size();
        dt=data[i].GetDelta();
        bt=data[i].BeginTime();

        if (prev_size!=npts) {
            delete [] amp;
            amp = new float [npts];
            prev_size=npts;
        }
        for (int j=0;j<npts;++j) amp[j]=data[i].GetAmp()[j];

        wsac1(tmpfilename,amp,&npts,&bt,&dt,&nerr,outfile.size());
    }
    delete [] amp;
}

std::vector<double> SACSignals::PeakTime() const{
    std::vector<double> ans;
    for (size_t i=0;i<Size();++i)
        ans.push_back(data[i].PeakTime());
    return ans;
}


void SACSignals::PrintInfo() const {
    PrintListInfo();
    std::cout << "====== \n";
    for (size_t i=0;i<Size();++i) {
        std::cout << mdata[i] << '\n';
        data[i].PrintInfo();
        std::cout << "\n------ \n";
    }
}

void SACSignals::PrintListInfo() const {
    std::cout << "Read from file: " << GetFileListName() << '\n';
    std::cout << "Current valid trace nubmer: " << Size() << '\n';
}

void SACSignals::RemoveRecords(const std::vector<std::size_t> &indices){
    if (indices.empty()) return;
    auto ind=indices;
    std::sort(ind.begin(),ind.end(),std::greater<std::size_t>());
    for (const auto &i:ind){
        std::swap(mdata[i],mdata.back());mdata.pop_back();
        std::swap(data[i],data.back());data.pop_back();
    }
    sorted_by="None";
}

std::vector<std::pair<double,double>> SACSignals::RemoveTrend(){
    std::vector<std::pair<double,double>> ans;
    for (std::size_t i=0;i<Size();++i)
        ans.push_back(data[i].RemoveTrend());
    return ans;
}

bool SACSignals::SameSamplingRate() const{
    if (Size()<=1) return true;
    double dt=data[0].GetDelta();
    for (std::size_t i=1;i<Size();++i)
        if (dt!=data[i].GetDelta())
            return false;
    return true;
}

bool SACSignals::SameSize() const{
    if (Size()<=1) return true;
    size_t n=data[0].Size();
    for (std::size_t i=1;i<Size();++i)
        if (n!=data[i].Size())
            return false;
    return true;
}

void SACSignals::SetBeginTime(const double &t){
    for (auto &item:data)
        item.SetBeginTime(t);
}

void SACSignals::SortByGcarc() {
    if (sorted_by=="Gcarc") return;
    auto cmp=[](const SACMetaData &m1, const SACMetaData &m2){return m1.gcarc<m2.gcarc;};
    ReorderUseIndex(data.begin(),data.end(),SortWithIndex(mdata.begin(),mdata.end(),cmp));
    sorted_by="Gcarc";
    return;
}

void SACSignals::SortByNetwork() {
    if (sorted_by=="Network") return;
    auto cmp=[](const SACMetaData &m1, const SACMetaData &m2){return m1.network<m2.network;};
    ReorderUseIndex(data.begin(),data.end(),SortWithIndex(mdata.begin(),mdata.end(),cmp));
    sorted_by="Network";
    return;
}

void SACSignals::SortByStnm() {
    if (sorted_by=="Stnm") return;
    auto cmp=[](const SACMetaData &m1, const SACMetaData &m2){return m1.stnm<m2.stnm;};
    ReorderUseIndex(data.begin(),data.end(),SortWithIndex(mdata.begin(),mdata.end(),cmp));
    sorted_by="Stnm";
    return;
}

std::vector<double> SACSignals::SNR(const double &nt1, const double &nt2,
                                    const double &st1, const double &st2, 
                                    const std::vector<double> &na,
                                    const std::vector<double> &sa) const{
    std::vector<double> ans;
    for (std::size_t i=0;i<Size();++i)
        ans.push_back(data[i].SNR(na.empty()?0:na[i]+nt1,na.empty()?0:na[i]+nt2,
                                  sa.empty()?0:sa[i]+st1,sa.empty()?0:sa[i]+st2));
    return ans;
}

void SACSignals::WaterLevelDecon(const EvenSampledSignal &s, const double &wl){
    for (std::size_t i=0;i<Size();++i)
        data[i].WaterLevelDecon(s,wl);
}

void SACSignals::WaterLevelDecon(SACSignals &D, const double &wl){
    //check size;
    if (Size()!=D.Size())
        throw std::runtime_error("Waterlevel decon source signal array size doesn't match.");
    for (std::size_t i=0;i<Size();++i)
        data[i].WaterLevelDecon(D.data[i],wl);
}


// Member template function definitions.
template<class T>
void SACSignals::CheckAndCutToWindow(const std::vector<T> &center_time,
                                     const double &t1, const double &t2){
    //check size;
    if (Size()!=center_time.size())
        throw std::runtime_error("Cut reference time array size doesn't match.");
    std::vector<std::size_t> BadIndices;
    for (size_t i=0;i<Size();++i)
        if (!data[i].CheckAndCutToWindow(center_time[i]+t1,center_time[i]+t2))
            BadIndices.push_back(i);
    RemoveRecords(BadIndices);
}

template<class T>
void SACSignals::FindPeakAround(const std::vector<T> &center_time, const double &wl){
    //check size;
    if (Size()!=center_time.size())
        throw std::runtime_error("FindPeak time array size doesn't match.");
    for (std::size_t i=0;i<Size();++i)
        data[i].FindPeakAround(center_time[i],wl);
}

// Set the given time to zero.
template<class T>
void SACSignals::ShiftTime(const std::vector<T> &t){
    //check size;
    if (Size()!=t.size())
        throw std::runtime_error("Time shift array size doesn't match.");
    for (std::size_t i=0;i<Size();++i) {
        data[i].ShiftTime(-t[i]);
        for (auto it=mdata[i].tt.begin();it!=mdata[i].tt.end();++it)
            it->second-=t[i];
    }
}

template<class T>
std::pair<std::pair<std::vector<int>,std::vector<double>>,
          std::pair<EvenSampledSignal,EvenSampledSignal>>
SACSignals::XCorrStack(const std::vector<T> &center_time,
                       const double &t1, const double &t2, const int loopN) const{

    if (!SameSamplingRate())
        throw std::runtime_error("Tried to XCorrStack differently sampled signals.");
    if (center_time.size()!=Size())
        throw std::runtime_error("In XCorrStack, # of signals doesn't agree with # of windows.");
    if (t1>=t2) throw std::runtime_error("XCorrStack window length <=0.");

    // will return {shift points, ccc} and the stack (duration is t2-t1), stdandard deviation.
    // If the trace is not used in the satck, ccc will be zero (use ccc as weights).

    std::pair<std::vector<int>, std::vector<double>> ans;
    if (Size()==0) return {ans,{EvenSampledSignal(),EvenSampledSignal()}};


    // Find the good records which has waveform avaliable within its XCorrStack window.
    std::vector<size_t> GoodIndex;
    for (size_t i=0;i<Size();++i)
        if (data[i].CheckWindow(center_time[i]+t1,center_time[i]+t2))
            GoodIndex.push_back(i);


    // First stack: directly stack the windowed section.
    EvenSampledSignal S0;
    for (const auto &i:GoodIndex) {
        auto Tmp=EvenSampledSignal(data[i]);
        Tmp.CheckAndCutToWindow(center_time[i]+t1,center_time[i]+t2);
        Tmp.SetBeginTime(t1);
        Tmp.NormalizeToSignal();
        S0+=Tmp;
    }


    // prepare output.
    std::vector<int> shift(GoodIndex.size(),0);
    std::vector<double> ccc(GoodIndex.size(),0);

    // Do the cross-correlation loop.
    EvenSampledSignal S=S0,STD;
    for (int loop=0;loop<loopN;++loop){

        std::vector<EvenSampledSignal> TmpData;
        for (size_t i=0;i<GoodIndex.size();++i) {

            size_t j=GoodIndex[i];

            auto res=CrossCorrelation(S,t1,t2,data[j],center_time[j]+t1,center_time[j]+t2);
            shift[i]=-res.first.first;
            ccc[i]=res.first.second;
            double shift_time=shift[i]*GetDelta();

            auto Tmp=EvenSampledSignal(data[j]);
            Tmp.CheckAndCutToWindow(center_time[j]+shift_time+t1,center_time[j]+shift_time+t2);
            Tmp.SetBeginTime(t1);
            Tmp.NormalizeToSignal();

            TmpData.push_back(Tmp);
        }
        std::tie(S,STD)=StackSignals(TmpData,ccc);
    }

    std::vector<int> Shift(Size(),0);
    std::vector<double> CCC(Size(),0);

    for (size_t i=0;i<GoodIndex.size();++i) {
        size_t j=GoodIndex[i];
        Shift[j]=shift[i];
        CCC[j]=ccc[i];
    }

    return {{Shift,CCC},{S,STD}};
}

/* -----------------------------------------------------------------------------
End of member function/operator definitions. -----------------------------------
----------------------------------------------------------------------------- */

// Non-member functions.
std::istream &operator>>(std::istream &is, SACSignals &item){

    item.Clear();
    std::string sacfilename;
    char file[300],stnm1[20],netwk1[20],p[20];
    char st[6]="kstnm",kt[7]="knetwk",gc[6]="gcarc";
    char tname[][3]={"t0","t1","t2","t3","t4","t5","t6","t7","t8","t9"};
    char pname[][4]={"kt0","kt1","kt2","kt3","kt4","kt5","kt6","kt7","kt8","kt9"};
    std::string stnm,netwk;
    std::map<std::string,double> M;
    int rawnpts,maxl=MAXL,nerr;
    float rawbeg,rawdel,rawdata[MAXL],gcarc,t;
    std::vector<double> D;

    while (is >> sacfilename){
        strcpy(file,sacfilename.c_str());
        rsac1(file,rawdata,&rawnpts,&rawbeg,&rawdel,&maxl,&nerr,strlen(file));
        if (nerr!=0) continue; // ignore unevenly sampled records.

        D.resize(rawnpts);
        for (int i=0;i<rawnpts;++i) D[i]=rawdata[i];

        EvenSampledSignal Tmp(D,rawdel,rawbeg,sacfilename);
        item.data.push_back(Tmp);

        // deal with headers, we only pull these headers:
        // network code, station code, gcarc, traveltimes.
        M.clear();

        // to hide SAC warning.
        const int stdoutfd(dup(fileno(stdout)));
        int newstdout = open("/dev/null",O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        dup2(newstdout, fileno(stdout));
        close(newstdout);
        // to hide SAC warning.

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

        // to hide SAC warning
        fflush(stdout);
        dup2(stdoutfd, fileno(stdout));
        close(stdoutfd);
        // to hide SAC warning

        item.mdata.push_back(SACMetaData(stnm,netwk,gcarc,M));
    }

    return is;
}

void DumpWaveforms(const SACSignals &item,const std::string &dir){
    std::string c=(dir.back()=='/'?"":"/");
    for (size_t i=0;i<item.Size();++i){

        auto s=item.GetData()[i].GetFileName();
        s=s.substr(s.find_last_of('/')+1);

        std::ofstream fpout(dir+c+s+".txt");
        fpout << item.GetData()[i];
        fpout.close();
    }
}

#endif
