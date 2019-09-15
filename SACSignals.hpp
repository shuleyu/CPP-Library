#ifndef ASU_SACSIGNALS
#define ASU_SACSIGNALS

#define MAXL 5000001

#include<vector>
#include<string>
#include<cstring>
#include<cstdlib>
#include<fstream>
#include<sstream>
#include<map>
#include<cstdio>
#include<fcntl.h>
#include<unistd.h>

extern "C"{
#include<sacio.h>
#include<sac.h>
}

#include<Lon2360.hpp>
#include<GcpDistance.hpp>
#include<FindAz.hpp>
#include<SortWithIndex.hpp>
#include<ReorderUseIndex.hpp>
#include<EvenSampledSignal.hpp>

// Todos:
// MetaData add event, depth, etc. header information.
// Only read-in meta data for station selection.

struct SACMetaData{
    std::string stnm,network;
    double gcarc,az,evde,evlo,evla,stlo,stla;
    std::map<std::string,double> tt;           // all travel times avaliable in the sac file header.

    SACMetaData () : SACMetaData("") {}

    SACMetaData (const std::string &s) : SACMetaData(s,"") {}

    SACMetaData (const std::string &s, const std::string &n) : SACMetaData(s,n,0) {}

    SACMetaData (const std::string &s, const std::string &n, const double &g) : SACMetaData(s,n,g,0) {}

    SACMetaData (const std::string &s, const std::string &n, const double &g, const double &a) :
                 SACMetaData(s,n,g,a,0,0,0,0,0) {}

    SACMetaData (const std::string &s, const std::string &n, const double &g, const double &a,
                 const double &ede, const double &elo, const double &ela, const double &slo, const double &sla) :
                 SACMetaData(s,n,g,a,ede,elo,ela,slo,sla,std::map<std::string, double> ()) {}

    SACMetaData (const std::string &s, const std::string &n, const double &g, const double &a,
                 const double &ede, const double &elo, const double &ela, const double &slo, const double &sla,
                 const std::map<std::string,double> &m) : stnm(s),network(n),gcarc(g),az(a),
                                                          evde(ede), evlo(elo), evla(ela), stlo(slo), stla(sla), tt(m) {}
};

std::ostream &operator<<(std::ostream &os, const SACMetaData &item){
    std::cout << "Network: |" << item.network << "|\n";
    std::cout << "StationName: |" << item.stnm << "|\n";
    std::cout << "Az: |" << item.az << "|\n";
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
    SACSignals (const SACSignals &item) = default;
    SACSignals (const std::string &infile);                 // a file contains path(s) to SAC file(s).
    SACSignals (const std::vector<std::string> &infiles);   // a vector contains paths(s) to SAC file(s).
    ~SACSignals () = default;

    // Member function declarations.
    void Clear() {*this=SACSignals();}
    double GetDelta() const {return (SameSamplingRate()?data[0].GetDelta():0);}
    const std::vector<EvenSampledSignal> &GetData() const {return data;}
    const std::vector<SACMetaData> &GetMData() const {return mdata;}
    std::size_t Size() const {return data.size();}
    std::string GetFileListName() const {return file_list_name;}

    void AddSignal(const EvenSampledSignal &s2, const std::vector<double> &dt={});
    void AmplitudeDivision(const std::vector<double> &scales);
    void Butterworth(const double &f1, const double &f2, const int &order=2, const int &passes=2);
    std::vector<double> BeginTime(const std::vector<std::size_t> &indices=std::vector<std::size_t> ()) const;
    void CheckAz(const double &d1=0, const double &d2=0);
    void CheckDist(const double &d1=-1, const double &d2=181);
    void CheckPhase(const std::string &phase, const double &t1=0,
                    const double &t2=std::numeric_limits<double>::max());
    std::pair<std::vector<double>,std::vector<double>>
        CrossCorrelation(const std::vector<double> &t1, const std::vector<double> &t2,
                         const EvenSampledSignal &item, const double &h1, const double &h2,
                         const int &Flip=0, const std::pair<int,int> &ShiftLimit=
                         {std::numeric_limits<int>::min(),std::numeric_limits<int>::max()}) const;
    std::pair<std::vector<double>,std::vector<double>>
        CrossCorrelation(const double &t1, const double &t2,
                         const EvenSampledSignal &item, const double &h1, const double &h2,
                         const int &Flip=0, const std::pair<int,int> &ShiftLimit=
                         {std::numeric_limits<int>::min(),std::numeric_limits<int>::max()}) const;
    void Diff();
    void DumpWaveforms(const std::string &dir=".", const std::string &namingConvention="",
                       const std::string &prefix="", const std::string &seperator="_", const std::string &extension="txt") const;
    std::vector<double> EndTime(const std::vector<std::size_t> &indices=std::vector<std::size_t> ()) const;
    void FlipPeakDown();
    void FlipPeakUp();
    std::vector<std::size_t> FindByGcarc(const double &gc, const bool &bulk=false);
    std::vector<std::size_t> FindByStnm(const std::string &st, const bool &bulk=false);
    std::vector<std::size_t> FindByNetwork(const std::string &nt, const bool &bulk=false);
    void GaussianBlur(const double &sigma=1);
    double GetDistance(const std::size_t &index=0) const;
    std::vector<double> GetDistances(const std::vector<std::size_t> &indices=std::vector<std::size_t> ()) const;
    std::vector<std::string> GetFileList() const;
    std::vector<std::string> GetNetworkNames() const;
    std::vector<std::string> GetSACFiles() const;
    std::vector<std::string> GetStationNames() const;
    std::vector<double> GetTravelTimes(const std::string &phase,
                                       const std::vector<std::size_t> &indices=std::vector<std::size_t> ()) const;
    std::vector<std::pair<std::vector<double>,std::vector<double>>>
        GetTimeAndWaveforms(const std::vector<std::size_t> &indices=std::vector<std::size_t> ()) const;
    std::vector<std::vector<double>>
        GetWaveforms(const std::vector<std::size_t> &indices=std::vector<std::size_t> ()) const;
    void HannTaper(const double &wl=10);
    void Integrate();
    void Interpolate(const double &dt);
    void KeepRecords(const std::vector<std::size_t> &indices);
    EvenSampledSignal MakeNeatStack() const;
    void NormalizeToGlobal();
    void NormalizeToPeak();
    void NormalizeToSignal();
    void OutputToSAC(const std::string &prefix="", const std::vector<std::size_t> &indices={},
                     const std::vector<std::map<std::string,double>> &F={},
                     const std::vector<std::map<std::string,std::string>> &M={}) const;
    std::vector<double> PeakAmp(const std::vector<std::size_t> &indices=std::vector<std::size_t> ()) const;
    std::vector<double> PeakTime(const std::vector<std::size_t> &indices=std::vector<std::size_t> ()) const;
    void PrintInfo() const;
    void PrintListInfo() const;
    void ReCalcAz();
    void ReCalcDist();
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
    void StripSignal(const EvenSampledSignal &s2, const std::vector<double> &dt={});
    void WaterLevelDecon(const EvenSampledSignal &s, const double &wl=0.1);
    void WaterLevelDecon(SACSignals &D, const double &wl=0.1);

    // Member function template declarations.
    template<typename T> void CheckAndCutToWindow(const std::vector<T> &center_time,
                                                  const double &t1, const double &t2);
    void CheckAndCutToWindow(const double &t1, const double &t2);
    template<typename T> void FindPeakAround(const std::vector<T> &center_time, const double &wl=5, const bool &positiveOnly=false);
    void FindPeakAround(const double &center_time, const double &wl=5, const bool &positiveOnly=false);
    template<typename T> void FlipReverseSum(const std::vector<T> &t);
    void FlipReverseSum(const double &t);
    template<typename T> void ShiftTime(const std::vector<T> &t);
    template<typename T> void ShiftTime(const T &t);
    void ShiftTimeReferenceToPeak();
    std::pair<std::pair<std::vector<double>,std::vector<double>>,std::pair<EvenSampledSignal,EvenSampledSignal>>
        XCorrStack(const double &center_time, const double &t1, const double &t2, const int loopN=5) const;
    std::pair<std::pair<std::vector<double>,std::vector<double>>,std::pair<EvenSampledSignal,EvenSampledSignal>>
        XCorrStack(const std::vector<double> &center_time, const double &t1, const double &t2, const int loopN=5) const;

    SACSignals &operator*=(const double &a){
        for (std::size_t i=0;i<Size();++i) data[i]*=a;
        return *this;
    }

    SACSignals &operator/=(const double &a){
        if (a==0)
            throw std::runtime_error("Divided by zero ...");
        return *this*=1/a;
    }

    SACSignals &operator-=(const EvenSampledSignal &item){
        for (std::size_t i=0;i<Size();++i) data[i]-=item;
        return *this;
    }

    // friends (non-member) declarations.
    friend std::istream &operator>>(std::istream &is, SACSignals &item);
    friend SACSignals operator*(const SACSignals &item,const double &a);
    friend SACSignals operator-(const SACSignals &input,const EvenSampledSignal &item);
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

SACSignals::SACSignals (const std::vector<std::string> &infiles){
    std::stringstream ss;
    std::copy(infiles.begin(),infiles.end(),std::ostream_iterator<std::string>(ss,"\n"));
    ss >> *this;
    ss.str(std::string());
    file_list_name="None";
    sorted_by="None";
}


// Member function definitions.

void SACSignals::AddSignal(const EvenSampledSignal &s2, const std::vector<double> &dt){

    if (!dt.empty() && Size()!=dt.size())
        throw std::runtime_error("Add signal time shift have different size.");

    if (dt.empty())
        for (std::size_t i=0;i<Size();++i)
            data[i].AddSignal(s2);
    else
        for (std::size_t i=0;i<Size();++i)
            data[i].AddSignal(s2,dt[i]);
    return;
}

void SACSignals::AmplitudeDivision(const std::vector<double> &scales){
    if (scales.size()!=Size())
        throw std::runtime_error("Scales size doesn't match.");

    for (std::size_t i=0;i<Size();++i)
        data[i]/=scales[i];
}

void SACSignals::Butterworth(const double &f1, const double &f2,
                             const int &order, const int &passes){
    for (std::size_t i=0;i<Size();++i)
        data[i].Butterworth(f1,f2,order,passes);
}

std::vector<double> SACSignals::BeginTime(const std::vector<std::size_t> &indices) const {
    std::vector<double> ans;
    if (indices.empty())
        for (std::size_t i=0;i<Size();++i) ans.push_back(data[i].BeginTime());
    else
        for (const auto &i:indices) {
            if (i>=Size()) continue;
            ans.push_back(data[i].BeginTime());
        }
    return ans;
}

void SACSignals::CheckAz(const double &d1, const double &d2){
    if (d1==d2) return;

    double lowerBound=Lon2360(d1), upperBound=Lon2360(d2);

    std::vector<std::size_t> BadIndices;
    if (lowerBound>upperBound) {
        for (std::size_t i=0;i<Size();++i)
            if (Lon2360(mdata[i].az)<lowerBound && Lon2360(mdata[i].az)>upperBound) BadIndices.push_back(i);
    }
    else {
        for (std::size_t i=0;i<Size();++i)
            if (Lon2360(mdata[i].az)<lowerBound || Lon2360(mdata[i].az)>upperBound) BadIndices.push_back(i);
    }
    RemoveRecords(BadIndices);
}

void SACSignals::CheckDist(const double &d1, const double &d2){
    std::vector<std::size_t> BadIndices;
    for (std::size_t i=0;i<Size();++i)
        if (mdata[i].gcarc<d1 || mdata[i].gcarc>d2) BadIndices.push_back(i);
    RemoveRecords(BadIndices);
}

void SACSignals::CheckPhase(const std::string &phase, const double &t1, const double &t2){
    std::vector<std::size_t> BadIndices;
    for (std::size_t i=0;i<Size();++i) {
        auto it=mdata[i].tt.find(phase);
        if (it==mdata[i].tt.end() || it->second<t1 || it->second>t2 )
            BadIndices.push_back(i);
    }
    RemoveRecords(BadIndices);
}

// negative shift time: item is shifting forward.
std::pair<std::vector<double>,std::vector<double>>
SACSignals::CrossCorrelation(const std::vector<double> &t1, const std::vector<double> &t2,
                             const EvenSampledSignal &item, const double &h1, const double &h2,
                             const int &Flip, const std::pair<int,int> &ShiftLimit) const {
    std::pair<std::vector<double>,std::vector<double>> ans;
    for (std::size_t i=0;i<Size();++i) {
        auto res=data[i].CrossCorrelation(t1[i],t2[i],item,h1,h2,Flip,ShiftLimit);
        ans.first.push_back(res.first);
        ans.second.push_back(res.second);
    }
    return ans;
}

std::pair<std::vector<double>,std::vector<double>>
SACSignals::CrossCorrelation(const double &t1, const double &t2,
                             const EvenSampledSignal &item, const double &h1, const double &h2,
                             const int &Flip, const std::pair<int,int> &ShiftLimit) const {
    return CrossCorrelation(std::vector<double> (Size(),t1),std::vector<double> (Size(),t2),item,h1,h2,Flip,ShiftLimit);
}

void SACSignals::Diff() {
    for (std::size_t i=0;i<Size();++i)
        data[i].Diff();
}

void SACSignals::DumpWaveforms(const std::string &dir, const std::string &namingConvention,
                               const std::string &prefix, const std::string &seperator, const std::string &extension) const{
    if (seperator.find("/")!=std::string::npos)
        throw std::runtime_error("In DumpWaveforms, seperator has sub directory.");

    std::string c=(dir.back()=='/'?"":"/");
    if (namingConvention=="StationName") {
        auto stationNames=GetStationNames();
        for (std::size_t i=0;i<Size();++i){
            std::ofstream fpout(dir+c+prefix+seperator+stationNames[i]+"."+extension);
            fpout << GetData()[i];
            fpout.close();
        }
    }
    else {
        for (std::size_t i=0;i<Size();++i){

            auto s=GetData()[i].GetFileName();
            s=s.substr(s.find_last_of('/')+1);

            std::ofstream fpout(dir+c+s+".txt");
            fpout << GetData()[i];
            fpout.close();
        }
    }
}

std::vector<double> SACSignals::EndTime(const std::vector<std::size_t> &indices) const {
    std::vector<double> ans;
    if (indices.empty())
        for (std::size_t i=0;i<Size();++i) ans.push_back(data[i].EndTime());
    else
        for (const auto &i:indices) {
            if (i>=Size()) continue;
            ans.push_back(data[i].EndTime());
        }
    return ans;
}

void SACSignals::FlipPeakDown() {
    for (std::size_t i=0;i<Size();++i)
        data[i].FlipPeakUp();
    (*this)*=-1;
}

void SACSignals::FlipPeakUp() {
    for (std::size_t i=0;i<Size();++i)
        data[i].FlipPeakUp();
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
        SACMetaData dummy("","",gc);
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
        SACMetaData dummy("",nt);
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
        SACMetaData dummy(st);
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

double SACSignals::GetDistance(const std::size_t &index) const {
    if (index>=Size()) return 0.0/0.0;
    return mdata[index].gcarc;
}

std::vector<double> SACSignals::GetDistances(const std::vector<std::size_t> &indices) const{
    std::vector<double> ans;
    if (indices.empty())
        for (const auto &item:mdata)
            ans.push_back(item.gcarc);
    else {
        for (const auto &i:indices) {
            if (i>=Size()) continue;
            ans.push_back(mdata[i].gcarc);
        }
    }
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


std::vector<double> SACSignals::GetTravelTimes(const std::string &phase,
                                               const std::vector<std::size_t> &indices) const {

    // Notice: if phase = "S" or "P", this will also return Sdiff and Pdiff travel times.
    std::string phase2="xxxx";
    if (phase=="S") phase2="Sdiff";
    if (phase=="P") phase2="Pdiff";

    std::vector<double> ans;
    if (indices.empty())
        for (const auto &item:mdata) {
            auto it=item.tt.find(phase), it2=item.tt.find(phase2);
            if (it!=item.tt.end()) ans.push_back(it->second);
            else if (it2!=item.tt.end()) ans.push_back(it2->second);
            else ans.push_back(-1);
        }
    else
        for (const auto &i:indices){
            if (i>=Size()) continue;
            auto it=mdata[i].tt.find(phase), it2=mdata[i].tt.find(phase2);
            if (it!=mdata[i].tt.end()) ans.push_back(it->second);
            else if (it2!=mdata[i].tt.end()) ans.push_back(it2->second);
            else ans.push_back(-1);
        }
    return ans;
}


std::vector<std::pair<std::vector<double>,std::vector<double>>>
SACSignals::GetTimeAndWaveforms(const std::vector<std::size_t> &indices) const {
    std::vector<std::pair<std::vector<double>,std::vector<double>>> ans;
    if (indices.empty())
        for (std::size_t i=0;i<Size();++i)
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
        for (std::size_t i=0;i<Size();++i)
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

void SACSignals::KeepRecords(const std::vector<std::size_t> &indices){
    std::vector<size_t> BadIndices;
    if (indices.empty())
        for (std::size_t i=0;i<Size();++i)
            BadIndices.push_back(i);
    else
        for (std::size_t i=0;i<Size();++i)
            if (count(indices.begin(),indices.end(),i)==0)
                BadIndices.push_back(i);
    RemoveRecords(BadIndices);
    return;
}

EvenSampledSignal SACSignals::MakeNeatStack() const {
    EvenSampledSignal ans;
    if (Size()==0) return ans;
    for (std::size_t i=0;i<Size();++i) ans+=data[i];
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

// Only normalize to the magnitude of the peak.
void SACSignals::NormalizeToPeak(){
    for (std::size_t i=0;i<Size();++i)
        data[i].NormalizeToPeak();
}

void SACSignals::NormalizeToSignal(){
    for (std::size_t i=0;i<Size();++i)
        data[i].NormalizeToSignal();
}

void SACSignals::OutputToSAC(const std::string &prefix, const std::vector<std::size_t> &indices,
                             const std::vector<std::map<std::string,double>> &F,
                             const std::vector<std::map<std::string,std::string>> &M) const {
    std::vector<std::size_t> ind;
    if (indices.empty()) {
        ind.resize(Size());
        for (std::size_t i=0;i<Size();++i) ind[i]=i;
    }
    else ind=indices;
    if (!F.empty() && F.size()!=ind.size()) throw std::runtime_error("Float info length doesn't match.");
    if (!M.empty() && M.size()!=ind.size()) throw std::runtime_error("String info length doesn't match.");

    int npts,nerr,prev_size=0,ltrue=true,lfalse;
    float dt,bt,*amp=nullptr;
    std::size_t k=0;
    for (const std::size_t &i:ind) {

        std::string outfile=prefix+std::to_string(i+1)+".sac";
        npts=data[i].Size();
        dt=data[i].GetDelta();
        bt=data[i].BeginTime();

        if (prev_size!=npts) {
            delete [] amp;
            amp = new float [npts];
            prev_size=npts;
        }
        for (int j=0;j<npts;++j) amp[j]=data[i].GetAmp()[j];

        // basic info.
        newhdr();
        setnhv((char *)"npts",&npts, &nerr, -1);
        setfhv((char *)"b", &bt, &nerr, -1);
        setfhv((char *)"delta", &dt, &nerr, -1);
        setihv((char *)"iftype", (char *)"itime", &nerr, -1, -1);
        setlhv((char *)"leven", &ltrue, &nerr, -1);
        setlhv((char *)"lpspol", &ltrue, &nerr, -1);
        setlhv((char *)"lcalda", &lfalse, &nerr, -1);

        // headers.
        if (k<F.size()) {
            for (const auto &item: F[k]){
                float tmpval=item.second;
                setfhv((char *)item.first.c_str(), &tmpval, &nerr, -1);
            }
        }
        if (k<M.size()) {
            for (const auto &item: M[k]){
                setkhv((char *)item.first.c_str(), (char *)item.second.c_str(), &nerr, -1, -1);
            }
        }
        ++k;

        wsac0((char *)outfile.c_str(),&dt,amp,&nerr,-1);
    }
    delete [] amp;
}

std::vector<double> SACSignals::PeakAmp(const std::vector<std::size_t> &indices) const{
    std::vector<double> ans;
    if (indices.empty())
        for (std::size_t i=0;i<Size();++i)
            ans.push_back(data[i].PeakAmp());
    else {
        for (const auto &i:indices) {
            if (i>=Size()) continue;
            ans.push_back(data[i].PeakAmp());
        }
    }
    return ans;
}

std::vector<double> SACSignals::PeakTime(const std::vector<std::size_t> &indices) const{
    std::vector<double> ans;
    if (indices.empty())
        for (std::size_t i=0;i<Size();++i)
            ans.push_back(data[i].PeakTime());
    else {
        for (const auto &i:indices) {
            if (i>=Size()) continue;
            ans.push_back(data[i].PeakTime());
        }
    }
    return ans;
}

void SACSignals::PrintInfo() const {
    PrintListInfo();
    std::cout << "====== \n";
    for (std::size_t i=0;i<Size();++i) {
        std::cout << mdata[i] << '\n';
        data[i].PrintInfo();
        std::cout << "\n------ \n";
    }
}

void SACSignals::PrintListInfo() const {
    std::cout << "Read from file: " << GetFileListName() << '\n';
    std::cout << "Current valid trace nubmer: " << Size() << '\n';
}


void SACSignals::ReCalcAz(){
    for (std::size_t i=0; i<Size(); ++i)
        mdata[i].az=FindAz(mdata[i].evlo, mdata[i].evla, mdata[i].stlo, mdata[i].stla);
}

void SACSignals::ReCalcDist(){
    for (std::size_t i=0; i<Size(); ++i)
        mdata[i].gcarc=GcpDistance(mdata[i].evlo, mdata[i].evla, mdata[i].stlo, mdata[i].stla);
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
    std::size_t n=data[0].Size();
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

void SACSignals::StripSignal(const EvenSampledSignal &s2, const std::vector<double> &dt){

    if (!dt.empty() && Size()!=dt.size())
        throw std::runtime_error("Strip signal time shift have different size.");

    if (dt.empty())
        for (std::size_t i=0;i<Size();++i)
            data[i].StripSignal(s2);
    else
        for (std::size_t i=0;i<Size();++i)
            data[i].StripSignal(s2,dt[i]);
    return;
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
template<typename T>
void SACSignals::CheckAndCutToWindow(const std::vector<T> &center_time,
                                     const double &t1, const double &t2){
    //check size;
    if (Size()!=center_time.size())
        throw std::runtime_error("Cut reference time array size doesn't match.");
    std::vector<std::size_t> BadIndices;
    for (std::size_t i=0;i<Size();++i)
        if (!data[i].CheckAndCutToWindow(center_time[i]+t1,center_time[i]+t2))
            BadIndices.push_back(i);
    RemoveRecords(BadIndices);
}
void SACSignals::CheckAndCutToWindow(const double &t1, const double &t2){
    CheckAndCutToWindow(std::vector<double> (Size(),0),t1,t2);
}

template<typename T>
void SACSignals::FindPeakAround(const std::vector<T> &center_time, const double &wl, const bool &positiveOnly){
    //check size;
    if (Size()!=center_time.size())
        throw std::runtime_error("FindPeak time array size doesn't match.");
    for (std::size_t i=0;i<Size();++i)
        data[i].FindPeakAround(center_time[i],wl,positiveOnly);
}

void SACSignals::FindPeakAround(const double &center_time, const double &wl, const bool &positiveOnly){
    FindPeakAround(std::vector<double> (Size(),center_time),wl,positiveOnly);
}

template<typename T>
void SACSignals::FlipReverseSum(const std::vector<T> &t){
    if (Size()!=t.size())
        throw std::runtime_error("FRS center time point array size doesn't match.");
    for (std::size_t i=0;i<Size();++i)
        data[i].FlipReverseSum(t[i]);
}
void SACSignals::FlipReverseSum(const double &t){
    FlipReverseSum(std::vector<double> (Size(),t));
}

// Set the given time to zero.
template<typename T>
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
template<typename T>
void SACSignals::ShiftTime(const T &t){
    ShiftTime(std::vector<T> (Size(),t));
}

void SACSignals::ShiftTimeReferenceToPeak(){
    ShiftTime(PeakTime());
}

std::pair<std::pair<std::vector<double>,std::vector<double>>,std::pair<EvenSampledSignal,EvenSampledSignal>>
SACSignals::XCorrStack(const double &center_time, const double &t1, const double &t2, const int loopN) const {
    return XCorrStack(std::vector<double> (Size(),center_time),t1,t2,loopN);
}

std::pair<std::pair<std::vector<double>,std::vector<double>>,std::pair<EvenSampledSignal,EvenSampledSignal>>
SACSignals::XCorrStack(const std::vector<double> &center_time, const double &t1, const double &t2, const int loopN) const{

    if (!SameSamplingRate())
        throw std::runtime_error("Tried to XCorrStack signals with different sample rate.");
    if (center_time.size()!=Size())
        throw std::runtime_error("In XCorrStack, # of signals doesn't agree with # of windows.");
    if (t1>=t2) throw std::runtime_error("XCorrStack window length <=0.");

    // will return {shift time, ccc}, {stack(averaged) of valid overlapping part of shifted orignal signal, stack stdandard deviation}
    // If the trace is not used in the satck, ccc will be zero (use ccc as weights).

    std::pair<std::vector<double>, std::vector<double>> ans;
    if (Size()==0) return {ans,{EvenSampledSignal(),EvenSampledSignal()}};


    // Find the good records which has waveform avaliable within its XCorrStack window.
    std::vector<std::size_t> GoodIndex;
    for (std::size_t i=0;i<Size();++i)
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

    // Do the cross-correlation loop.
    EvenSampledSignal S=S0,STD;
    for (int loop=0;loop<loopN;++loop){

        std::vector<EvenSampledSignal> TmpData;
        std::vector<double> ccc;
        double newBeginTime=-std::numeric_limits<double>::max();
        double newEndTime=std::numeric_limits<double>::max();
        for (std::size_t i=0;i<GoodIndex.size();++i) {

            std::size_t j=GoodIndex[i];

            auto res=S.CrossCorrelation(t1,t2,data[j],center_time[j]+t1,center_time[j]+t2);

            auto Tmp=data[j];
            if (Tmp.CheckWindow(center_time[j]+t1-res.first,center_time[j]+t2-res.first)) {
                ccc.push_back(res.second);
                Tmp.ShiftTime(-res.first);
                Tmp.NormalizeToWindow(t1,t2);
                newBeginTime=std::max(newBeginTime,Tmp.BeginTime());
                newEndTime=std::min(newEndTime,Tmp.EndTime());
                TmpData.push_back(Tmp);
            }
        }
        for (auto &item: TmpData) {
            item.CheckAndCutToWindow(newBeginTime,newEndTime);
            item.SetBeginTime(newBeginTime);
        }
        std::tie(S,STD)=StackSignals(TmpData,ccc);
    }

    // Traces not contributing to ESW have ccc=nan.
    std::vector<double> CCC(Size(),0.0/0.0),AlignTime(Size(),0);
    for (std::size_t i=0;i<GoodIndex.size();++i) {
        std::size_t j=GoodIndex[i];
        auto res=S.CrossCorrelation(t1,t2,data[j],center_time[j]+t1,center_time[j]+t2);
        AlignTime[j]=-res.first;
        CCC[j]=res.second;
    }

    return {{AlignTime,CCC},{S,STD}};
}

/* -----------------------------------------------------------------------------
End of member function/operator definitions. -----------------------------------
----------------------------------------------------------------------------- */

// Non-member functions.
std::istream &operator>>(std::istream &is, SACSignals &item){

    item.Clear();
    std::string sacfilename;
    char file[300],stnm1[20],netwk1[20],p[20];
    char st[6]="kstnm",kt[7]="knetwk",gc[6]="gcarc",ede[5]="evdp",elo[5]="evlo",ela[5]="evla",slo[5]="stlo",sla[5]="stla",azimuth[3]="az";
    char tname[][3]={"t0","t1","t2","t3","t4","t5","t6","t7","t8","t9"};
    char pname[][4]={"kt0","kt1","kt2","kt3","kt4","kt5","kt6","kt7","kt8","kt9"};
    std::string stnm,netwk;
    std::map<std::string,double> M;
    int rawnpts,maxl=MAXL,nerr;
    float rawbeg,rawdel,gcarc,t,evde,evlo,evla,stlo,stla,az;
    float *rawdata = new float[MAXL];
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
        // network code, station code, gcarc, traveltimes,
        // event lon/lat, station lon/lat.
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

        for (std::size_t i=0;i<10;++i) {
            getfhv(tname[i],&t,&nerr,2);
            getkhv(pname[i],p,&nerr,3,20);
            std::string tmpstr(p);
            tmpstr=tmpstr.substr(0,tmpstr.find_first_of(" \n\r\t"));
            if (tmpstr!="-12345") M[tmpstr]=t;
        }
        getfhv(gc,&gcarc,&nerr,5);
        getfhv(ede,&evde,&nerr,4);
        getfhv(elo,&evlo,&nerr,4);
        getfhv(ela,&evla,&nerr,4);
        getfhv(slo,&stlo,&nerr,4);
        getfhv(sla,&stla,&nerr,4);
        getfhv(azimuth,&az,&nerr,2);

        // to hide SAC warning
        fflush(stdout);
        dup2(stdoutfd, fileno(stdout));
        close(stdoutfd);
        // to hide SAC warning

        item.mdata.push_back(SACMetaData(stnm,netwk,gcarc,az,evde,evlo,evla,stlo,stla,M));
    }
    delete [] rawdata;

    return is;
}

SACSignals operator*(const SACSignals &input,const double &a){
    SACSignals ans(input);
    for (auto &item:ans.data) item*=a;
    return ans;
}
SACSignals operator*(const double &a, const SACSignals &input){
    return input*a;
}
SACSignals operator-(const SACSignals &input,const EvenSampledSignal &item){
    SACSignals ans(input);
    for (std::size_t i=0;i<ans.Size();++i) ans.data[i]-=item;
    return ans;
}

#endif
