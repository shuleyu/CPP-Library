#ifndef ASU_EVENSAMPLEDSIGNAL
#define ASU_EVENSAMPLEDSIGNAL

#include<vector>
#include<cmath>
#include<string>
#include<fstream>
#include<algorithm>

#include<HannTaper.hpp>
#include<Interpolate.hpp>
#include<CreateGrid.hpp>
#include<CompareSignal.hpp>
#include<GaussianBlur.hpp>
#include<Butterworth.hpp>
#include<RemoveTrend.hpp>
#include<WaterLevelDecon.hpp>
#include<StretchSignal.hpp>
#include<DigitalSignal.hpp>
#include<SimpsonRule.hpp>
#include<TstarOperator.hpp>
#include<Convolve.hpp>
#include<SNR.hpp>
#include<CrossCorrelation.hpp>

class EvenSampledSignal : public DigitalSignal {

protected:

    double Dt=0,BeginTime=0,EndTime=0;

public:

    // Constructor/Destructors.
    EvenSampledSignal () = default;
    EvenSampledSignal (const EvenSampledSignal &) = default;
    EvenSampledSignal (const std::string &infile);
    EvenSampledSignal (const DigitalSignal &item, const double &delta);
    EvenSampledSignal (const EvenSampledSignal &item, const double &delta);
    EvenSampledSignal (const EvenSampledSignal &item, const double &t1, const double &t2);
    template<class T> EvenSampledSignal (const std::vector<T> &item, const double &delta, const double &begintime=0, const std::string &infile="");
    virtual ~EvenSampledSignal () = default; // Base class destructor need to be virtual. (this derived class could be a base for another derived class.)

    // Virtual (override) function declarations.
    virtual double bt() const override {return BeginTime;}
    virtual void clear() override {*this=EvenSampledSignal ();}
    virtual double length() const override {return EndTime-BeginTime;}
    virtual double et() const override {return EndTime;}
    virtual double pt() const override {return BeginTime+Peak*Dt;}
    virtual void ShiftTime(const double &t) override {BeginTime+=t;EndTime+=t;}

    virtual bool CheckAndCutToWindow(const double &t1, const double &t2) override;
    virtual void FindPeakAround(const double &t, const double &w=5) override;
    virtual void HannTaper(const double &wl) override;
    virtual std::size_t LocateTime(const double &t) const override;
    virtual void PrintInfo() const override;
    virtual std::pair<double,double> RemoveTrend() override;
    virtual void ZeroOutHannTaper(const double &wl, const double &zl);

    // member operators declearation.
    virtual EvenSampledSignal &operator+=(const double &a);
    virtual EvenSampledSignal &operator*=(const double &a);
    virtual EvenSampledSignal &operator-=(const double &a);
    virtual EvenSampledSignal &operator/=(const double &a);

    // Original (and final) functions.
    double dt() const {return Dt;}

    double AbsIntegral() const;
    void Butterworth(const double &f1, const double &f2);
    void Convolve(const EvenSampledSignal &item);
    void GaussianBlur(const double &sigma=1);
    void Interpolate(const double &delta);
    double SNR(const double &nt1, const double &nt2, const double &st1, const double &st2);
    EvenSampledSignal Stretch(const double &h=1) const;
    EvenSampledSignal Tstar(const double &ts, const double &tol=1e-3) const;
    void WaterLevelDecon(const EvenSampledSignal &source, const double &wl=0.1);

    // member original operators declearation.
    EvenSampledSignal &operator+=(const EvenSampledSignal &item);

    // non-member friends declearation.
    friend class SACSignals;
    friend SignalCompareResults CompareSignal(EvenSampledSignal S1, EvenSampledSignal S2, const double &t1, const double &t2, const double &AmpLevel);
    friend std::pair<std::pair<int,double>,std::vector<double>> CrossCorrelation(const EvenSampledSignal &S1, const double &t1, const double &t2, const EvenSampledSignal &S2, const double &h1, const double &h2);
    friend std::istream &operator>>(std::istream &is, EvenSampledSignal &item);
    friend std::ostream &operator<<(std::ostream &os, const EvenSampledSignal &item);
    friend EvenSampledSignal operator+(const EvenSampledSignal &item, const double &a);
    friend EvenSampledSignal operator+(const double &a, const EvenSampledSignal &item);
    friend EvenSampledSignal operator+(const EvenSampledSignal &s1, const EvenSampledSignal &s2);
    friend EvenSampledSignal operator-(const EvenSampledSignal &item, const double &a);
    friend EvenSampledSignal operator*(const EvenSampledSignal &item, const double &a);
    friend EvenSampledSignal operator*(const double &a, const EvenSampledSignal &item);
    friend EvenSampledSignal operator/(const EvenSampledSignal &item, const double &a);
};

// Constructors/Destructors definition.
EvenSampledSignal::EvenSampledSignal (const std::string &infile) {
    std::ifstream fpin(infile);
    fpin >> *this;
    fpin.close();
    FileName=infile;
}

EvenSampledSignal::EvenSampledSignal (const DigitalSignal &item, const double &delta) {

    // Interpolation.
    auto xx=::CreateGrid(item.bt(),item.et(),delta,1);
    Amp=::Interpolate(item.Time,item.Amp,xx);

    Dt=delta;
    BeginTime=item.bt();
    EndTime=item.et();

    FindPeakAround(item.pt(),0.5);
    FileName=item.FileName;
    AmpMultiplier=item.AmpMultiplier;
}

EvenSampledSignal::EvenSampledSignal (const EvenSampledSignal &item, const double &delta) {

    // If sampling rate is not the same, interpolate to delta.
    if (item.dt()<=delta*0.99 || delta*1.01<=item.dt()){
        auto x=::CreateGrid(item.bt(),item.et(),item.size(),0);
        auto xx=::CreateGrid(item.bt(),item.et(),delta,1);
        Amp=::Interpolate(x,item.Amp,xx);
    }
    else Amp=item.Amp;

    Dt=delta;
    BeginTime=item.bt();
    EndTime=item.et();

    FindPeakAround(item.pt(),0.5);
    FileName=item.FileName;
    AmpMultiplier=item.AmpMultiplier;
}

EvenSampledSignal::EvenSampledSignal (const EvenSampledSignal &item, const double &t1, const double &t2) {
    *this=EvenSampledSignal();

    if (item.CheckWindow(t1,t2)){

        size_t L1=item.LocateTime(t1),L2=item.LocateTime(t2);
        Amp=std::vector<double> (item.Amp.begin()+L1,item.Amp.begin()+L2+1);

        Dt=item.dt();
        BeginTime=item.bt()+L1*item.dt();
        EndTime=item.bt()+L2*item.dt();

        FindPeakAround(item.pt(),0.5);
        FileName=item.FileName;
        AmpMultiplier=item.AmpMultiplier;
    }
}

template<class T>
EvenSampledSignal::EvenSampledSignal (const std::vector<T> &item, const double &delta, const double &begintime, const std::string &infile) {
    Amp.resize(item.size());
    for (size_t i=0;i<size();++i) Amp[i]=item[i];
    Dt=delta;
    BeginTime=EndTime=begintime;
    if (size()>1) EndTime=BeginTime+Dt*(size()-1);
    FileName=infile;
}

// Member function definitions.

// Cut the data within a window. If cut failed, don't cut and return false.
bool EvenSampledSignal::CheckAndCutToWindow(const double &t1, const double &t2){

    if (!CheckWindow(t1,t2)) return false;

    // Cut.
    std::size_t d1=ceil((t1-bt())/dt()),d2=1+floor((t2-bt())/dt());

    EndTime=BeginTime+(d2-1)*dt();
    BeginTime+=d1*dt();

    std::vector<double> NewAmp(Amp.begin()+d1,Amp.begin()+d2);
    std::swap(NewAmp,Amp);

    if (Peak>=d1) Peak-=d1;
    else Peak=0;
    if (Peak>size()) Peak=0;
    return true;
}

// Find the position of max|amp| around some time.
void EvenSampledSignal::FindPeakAround(const double &t, const double &w){
    std::size_t WB=std::max(0.0,(t-w-BeginTime)/Dt),WE=std::min(size(),(size_t)ceil((t+w-BeginTime+Dt/2)/Dt));

    double AbsMax=-1;
    for (std::size_t i=WB;i<WE;++i){
        if (AbsMax<fabs(Amp[i])) {
            AbsMax=fabs(Amp[i]);
            Peak=i;
        }
    }
}

// cos (-pi,pi) shaped taper at two ends.
void EvenSampledSignal::HannTaper(const double &wl){
    if (wl*2>length()) throw std::runtime_error("Hanning window too wide.");
    for (size_t i=0;i<size();++i){
        double dt=std::min(i,size()-i-1)*Dt;
        if (dt<wl) Amp[i]*=0.5-0.5*cos(dt/wl*M_PI);
    }
}

std::size_t EvenSampledSignal::LocateTime(const double &t) const{
    if (t<bt() && bt()-t<dt()/2) return 0;
    if (t>et() && t-et()<dt()/2) return size()-1;
    if (t<bt() || t>et()) return -1;
    std::size_t ans=(t-bt())/dt();
    if (ans+1==size()) return ans;
    if (fabs(t-(bt()+ans*dt()))<fabs(t-(bt()+(ans+1)*dt()))) return ans;
    else return ans+1;
}

// Print some info.
void EvenSampledSignal::PrintInfo() const {
    DigitalSignal::PrintInfo();
    std::cout << "Sampling rate: " << dt() << '\n';
}

// remove drift and DC.
std::pair<double,double> EvenSampledSignal::RemoveTrend(){
    return ::RemoveTrend(Amp,Dt,BeginTime);
}

// taper window half-length is wl, zero half-length is zl.
void EvenSampledSignal::ZeroOutHannTaper(const double &wl, const double &zl){
    if ((wl+zl)*2>length()) throw std::runtime_error("ZeroOutHanning window too wide.");
    for (size_t i=0;i<size();++i){
        double dt=std::min(i,size()-i-1)*Dt;
        if (dt<zl) Amp[i]=0;
        else if (dt<zl+wl) Amp[i]*=0.5-0.5*cos((dt-zl)/wl*M_PI);
    }
}

// virtual member operators definition.
EvenSampledSignal &EvenSampledSignal::operator+=(const double &a){
    for (size_t i=0;i<size();++i) Amp[i]+=a;
    return *this;
}
EvenSampledSignal &EvenSampledSignal::operator*=(const double &a){
    for (size_t i=0;i<size();++i) Amp[i]*=a;
    return *this;
}
EvenSampledSignal &EvenSampledSignal::operator-=(const double &a){
    *this+=(-a);
    return *this;
}
EvenSampledSignal &EvenSampledSignal::operator/=(const double &a){
    if (a==0) throw std::runtime_error("Dividing amplitude with zero.");
    *this*=(1.0/a);
    return *this;
}

// Take absolute amplitude then do the integral.
double EvenSampledSignal::AbsIntegral() const {
    auto f=[](const double &s){return (s>0?s:-s);};
    return ::SimpsonRule(Amp.begin(),Amp.end(),dt(),f);
}

// butterworth filter.
// changes: Amp(value change).
void EvenSampledSignal::Butterworth(const double &f1, const double &f2){
    std::vector<std::vector<double>> In{Amp};
    ::Butterworth(In,Dt,f1,f2);
    Amp=In[0];
}

// Convolve with another signal S2, truncated relative to S2'peak position. (keep s1's size).
// This is acausal convolution, trying to keep the original peak's position.
void EvenSampledSignal::Convolve(const EvenSampledSignal &item){
    auto res=::Convolve(Amp,item.Amp);
    std::rotate(res.begin(),res.begin()+item.peak(),res.end());
    res.resize(size());
    Amp=res;
}

// gaussian blur.
// changes: Amp(value change).
void EvenSampledSignal::GaussianBlur(const double &sigma){
    std::vector<std::vector<double>> In{Amp};
    ::GaussianBlur(In,Dt,sigma);
    Amp=In[0];
}

// Interpolate to certain sampling rate.
void EvenSampledSignal::Interpolate(const double &delta){
    *this=EvenSampledSignal (*this,delta);
}

// Measure SNR.
double EvenSampledSignal::SNR(const double &nt1, const double &nt2, const double &st1, const double &st2){
    int NB=ceil((nt1-bt())/dt()),NL=ceil((nt2-nt1)/dt());
    int SB=ceil((st1-bt())/dt()),SL=ceil((st2-st1)/dt());
    return ::SNR(Amp,NB,NL,SB,SL);
}

// Stretch the signal horizontally and vertically.
// Keep sampling rate the same, keep peak time the same, which means updates:
// BeginTime, EndTime, Peak,
EvenSampledSignal EvenSampledSignal::Stretch(const double &h) const{

    EvenSampledSignal ans(*this);
    if (h==1) return ans;
    double OldPeakTime=pt();

    // Stretch one side.
    ans.Amp=std::vector<double> (Amp.begin(),Amp.begin()+peak()+1);
    std::reverse(ans.Amp.begin(),ans.Amp.end());
    ans.Amp=::StretchSignal(ans.Amp,h);
    std::reverse(ans.Amp.begin(),ans.Amp.end());

    // Stretch another side.
    ans.Amp.pop_back();
    ans.Peak=ans.Amp.size();
    auto S=::StretchSignal(std::vector<double> (Amp.begin()+peak(),Amp.end()),h);
    ans.Amp.insert(ans.Amp.end(),S.begin(),S.end());

    // Fix peak time (by adjusting BeginTime)
    ans.BeginTime=OldPeakTime-ans.peak()*dt();
    ans.EndTime=ans.BeginTime+(ans.size()-1)*dt();
    ans.FindPeakAround(pt(),1);

    return ans;
}

// Make a t-stared waveform.
// Keep sampling rate the same, keep data length the same.
// Notice: Peak amplitude and position could be changed.
EvenSampledSignal EvenSampledSignal::Tstar(const double &ts, const double &tol) const{
    EvenSampledSignal ans(*this);
    if (ts<=0) return ans;

    // Create a t* operator.
    auto res=::TstarOperator(ts,dt(),tol);
    EvenSampledSignal Ts(res.first,dt(),-dt()*res.second);
    Ts.FindPeakAround(0,1);
    ans.Convolve(Ts);
    return ans;
}

// Notice: remove trend and do a taper (10% of length) both on source and signal before the decon.
// Changes:
// Amp(signal length change),Peak(=Amp.size()/2),BeginTime(relative to peak time),EndTime(relative to peak time)
void EvenSampledSignal::WaterLevelDecon(const EvenSampledSignal &source, const double &wl) {
    if (fabs(Dt-source.Dt)>1e-5)
        throw std::runtime_error("Signal inputs of decon have different sampling rate.");

    HannTaper(0.1*length());
    RemoveTrend();

    auto S=source.Amp;
    ::HannTaper(S,0.1);
    ::RemoveTrend(S,Dt,source.bt());

    std::vector<std::vector<double>> In{Amp};
    std::vector<std::size_t> P{Peak};
    auto ans=::WaterLevelDecon(In,P,S,source.Peak,Dt,wl);

    Amp=ans[0];
    Peak=size()/2;
    BeginTime=(Peak-1)*Dt*-1;
    EndTime=BeginTime+(size()-1)*Dt;
}

// Stack two same sampling rate, same begintime signal.
EvenSampledSignal &EvenSampledSignal::operator+=(const EvenSampledSignal &item){
    if (fabs(dt()-item.dt())>1e-5)
        throw std::runtime_error("Tried to stack two signals with different sampling rate: "+std::to_string(dt())+" v.s. "+std::to_string(item.dt()));
    if (fabs(bt()-item.bt())>1e-5)
        throw std::runtime_error("Tried to stack two signals with different begin time: "+std::to_string(bt())+" v.s. "+std::to_string(item.bt()));
    if (size()!=item.size())
        throw std::runtime_error("Tried to stack two signals with different number of points: "+std::to_string(size())+" v.s. "+std::to_string(item.size()));
    for (size_t i=0;i<size();++i) Amp[i]+=item.Amp[i];
    return *this;
}

// Non-member functions.

// Compare two signals around their peaks.
// t1 t2 are time window relative to the peak (in seconds, e.g. t1=-5, t2=5)
SignalCompareResults CompareSignal(EvenSampledSignal S1, EvenSampledSignal S2, const double &t1, const double &t2, const double &AmpLevel){
    if (fabs(S1.dt()-S2.dt())>0.01*S1.dt()) throw std::runtime_error("Comparing two differently sampled signals.");
    S1.NormalizeToPeak();
    S2.NormalizeToPeak();
    return ::CompareSignal(S1.Amp,S1.peak(),S2.Amp,S2.peak(),S1.dt(),t1,t2,AmpLevel);
}

std::pair<std::pair<int,double>,std::vector<double>> CrossCorrelation(const EvenSampledSignal &S1, const double &t1, const double &t2, const EvenSampledSignal &S2, const double &h1, const double &h2){
    // Check window position.
    if (!S1.CheckWindow(t1,t2)) throw std::runtime_error("CrossCorrelation window on signal 1 is not proper.");
    if (!S2.CheckWindow(h1,h2)) throw std::runtime_error("CrossCorrelation window on signal 2 is not proper.");

    return ::CrossCorrelation(S1.Amp.begin()+S1.LocateTime(t1),S1.Amp.begin()+S1.LocateTime(t2)+1,S2.Amp.begin()+S2.LocateTime(h1),S2.Amp.begin()+S2.LocateTime(h2)+1);

}

// Overload operator ">>" to read a signal from a two-columned input (stdin/file/etc.)
std::istream &operator>>(std::istream &is, EvenSampledSignal &item){

    item.clear();
    double x,y,dt=-1;
    while (is >> x >> y) {
        if (item.Amp.empty()) item.BeginTime=x;
        else {
            // Check the ascending of Time serise.
            if (x<=item.EndTime)
                throw std::runtime_error("Time serise is either not increasing or has repeating values.");

            // Check even sampling.
            if (dt<0) dt=x-item.EndTime;
            else if (x-item.EndTime<=dt*0.99 || dt*1.01<=x-item.EndTime)
                throw std::runtime_error("Input seems not even sampled.");
            else dt=(x-item.BeginTime)/item.Amp.size();
        }

        item.Amp.push_back(y);
        item.EndTime=x;
    }

    item.Dt=(item.EndTime-item.BeginTime);
    if (item.Amp.size()>1) item.Dt/=(item.Amp.size()-1);

    return is;
}

// Overload operator "<<" to print a signal in a two-columned format.
std::ostream &operator<<(std::ostream &os, const EvenSampledSignal &item){

//     // record original format.
//     std::ios state(NULL);
//     state.copyfmt(os);
//
//     os << std::fixed << std::scientific << std::setprecision(10);
    for (std::size_t i=0;i<item.size();++i)
        os << item.BeginTime+item.Dt*i << '\t' << item.Amp[i] << '\n';

//     // restore print format.
//     os.copyfmt(state);
    return os;
}

// Overload operator "+,-" to ShiftDC.
// Not changing AmpMultiplier.
EvenSampledSignal operator+(const EvenSampledSignal &item,const double &a){
    EvenSampledSignal ans(item);
    ans+=a;
    return ans;
}
EvenSampledSignal operator+(const double &a,const EvenSampledSignal &item){
    return item+a;
}
EvenSampledSignal operator+(const EvenSampledSignal &s1,const EvenSampledSignal &s2){
    EvenSampledSignal ans(s1);
    ans+=s2;
    return ans;
}
EvenSampledSignal operator-(const EvenSampledSignal &item,const double &a){
    return item+(-a);
}

// Overload operator "*,/" to Scale.
// Not changing AmpMultiplier.
EvenSampledSignal operator*(const EvenSampledSignal &item,const double &a){
    EvenSampledSignal ans(item);
    ans*=a;
    return ans;
}
EvenSampledSignal operator*(const double &a,const EvenSampledSignal &item){
    return item*a;
}
EvenSampledSignal operator/(const EvenSampledSignal &item,const double &a){
    EvenSampledSignal ans(item);
    ans/=a;
    return ans;
}

#endif
