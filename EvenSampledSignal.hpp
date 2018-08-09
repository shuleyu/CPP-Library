#ifndef ASU_EVENSAMPLEDSIGNAL
#define ASU_EVENSAMPLEDSIGNAL

#include<vector>
#include<cmath>
#include<string>
#include<fstream>
#include<numeric>
#include<algorithm>

#include<AvrStd.hpp>
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

    /* Not inherited (private) member of DigitalSignal.

//     std::vector<double> time;

    */

protected:

    double delta=0,begin_time=0;

/*     protected members inherited from DigitalSignal.

//     std::vector<double> amp;
//     std::size_t peak;
//     std::string filename;
//     double amp_multiplier;
                                              */

public:

    // Constructor/Destructors.
    EvenSampledSignal ();
    EvenSampledSignal (const std::string &infile);                          // File is even sampled.
    EvenSampledSignal (const std::string &infile, const double &dt);     // File is un-even sampled.
    EvenSampledSignal (const DigitalSignal &item, const double &dt);
    EvenSampledSignal (const EvenSampledSignal &item) = default;
    EvenSampledSignal (const EvenSampledSignal &item, const double &dt);
    EvenSampledSignal (const EvenSampledSignal &item, const double &t1, const double &t2);
    template<class T> EvenSampledSignal (const std::vector<T> &item, const double &dt,
                                         const double &begintime=0, const std::string &infile="");
    virtual ~EvenSampledSignal () = default;

    // Virtual (need to be overritten) functions/operators declarations.
    //
    // based on the materials I read:
    // override keyword means this function comes from a base class.
    // virtual keyword means this function could be override 
    // 
    double BeginTime() const override final {return begin_time;}
    virtual void Clear() override {*this=EvenSampledSignal ();}
    virtual double SignalDuration() const override {return (Size()<=1?0:Delta()*(Size()-1));}
    virtual double et() const override {return (Size()<=1?begin_time:begin_time+SignalDuration();}
    virtual double pt() const override {return begin_time+Peak*Delta();}
    virtual void ShiftTime(const double &t) override {begin_time+=t;EndTime+=t;}

    virtual bool CheckAndCutToWindow(const double &t1, const double &t2) override;
    virtual void FindPeakAround(const double &t, const double &w=5) override;
    virtual void HannTaper(const double &wl) override;
    virtual std::size_t LocateTime(const double &t) const override;
    virtual void PrintInfo() const override;
    virtual std::pair<double,double> RemoveTrend() override;
    virtual void ZeroOutHannTaper(const double &wl, const double &zl);

    virtual EvenSampledSignal &operator+=(const double &a);
    virtual EvenSampledSignal &operator*=(const double &a);
    virtual EvenSampledSignal &operator-=(const double &a);
    virtual EvenSampledSignal &operator/=(const double &a);

    // Original (and final) functions.
    double Delta() const {return delta;}

    double AbsIntegral() const;
    void Butterworth(const double &f1, const double &f2, const int &order=2, const int &passes=2);
    void Convolve(const EvenSampledSignal &item);
    void GaussianBlur(const double &sigma=1);
    void Integrate();
    void Interpolate(const double &dt);
    double SNR(const double &nt1, const double &nt2, const double &st1, const double &st2);
    EvenSampledSignal Stretch(const double &h=1) const;
    std::vector<double> GetTime() const;
    EvenSampledSignal Tstar(const double &ts, const double &tol=1e-3) const;
    void WaterLevelDecon(const EvenSampledSignal &source, const double &wl=0.1);
    EvenSampledSignal &operator+=(const EvenSampledSignal &item);


    // declaration of non-member class/function/operators, who need access to
    // the private/protected parts of this class, therefore they need to be friend.
    friend class SACSignals;
    friend SignalCompareResults CompareSignal(EvenSampledSignal S1, EvenSampledSignal S2, const double &t1, const double &t2, const double &AmpLevel);
    friend std::pair<std::pair<int,double>,std::vector<double>> CrossCorrelation(const EvenSampledSignal &S1, const double &t1, const double &t2, const EvenSampledSignal &S2, const double &h1, const double &h2, const bool &Dump=false, const int &Flip=0, const std::pair<int,int> &ShiftLimit={std::numeric_limits<int>::min(),std::numeric_limits<int>::max()});
    friend std::pair<EvenSampledSignal,EvenSampledSignal> StackSignals (const std::vector<EvenSampledSignal> &Signals, const std::vector<double> &Weights={});
    friend std::istream &operator>>(std::istream &is, EvenSampledSignal &item);
    friend std::ostream &operator<<(std::ostream &os, const EvenSampledSignal &item);
    friend EvenSampledSignal operator+(const EvenSampledSignal &item, const double &a);
    friend EvenSampledSignal operator+(const double &a, const EvenSampledSignal &item);
    friend EvenSampledSignal operator+(const EvenSampledSignal &s1, const EvenSampledSignal &s2);
    friend EvenSampledSignal operator-(const EvenSampledSignal &item, const double &a);
    friend EvenSampledSignal operator*(const EvenSampledSignal &item, const double &a);
    friend EvenSampledSignal operator*(const double &a, const EvenSampledSignal &item);
    friend EvenSampledSignal operator/(const EvenSampledSignal &item, const double &a);

}; // End of class declaration.

// Constructors/Destructors definition.
EvenSampledSignal::EvenSampledSignal () {
    delta=0;
    begin_time=0;
}

EvenSampledSignal::EvenSampledSignal (const std::string &infile) {
    std::ifstream fpin(infile);
    fpin >> *this;
    fpin.close();
    FileName=infile;
}

EvenSampledSignal::EvenSampledSignal (const DigitalSignal &item, const double &dt) {

    // Interpolation.
    auto xx=::CreateGrid(item.BeginTime(),item.et(),dt,1);
    Amp=::Interpolate(item.GetTime(),item.GetAmp(),xx);

    delta=dt;
    begin_time=item.BeginTime();
    EndTime=item.et();

    FindPeakAround(item.pt(),0.5);
    FileName=item.FileName;
    AmpMultiplier=item.AmpMultiplier;
}

EvenSampledSignal::EvenSampledSignal (const EvenSampledSignal &item, const double &dt) {

    // If sampling rate is not the same, interpolate to dt.
    if (item.Delta()<=dt*0.99 || dt*1.01<=item.Delta()){
        auto x=::CreateGrid(item.BeginTime(),item.et(),item.size(),0);
        auto xx=::CreateGrid(item.BeginTime(),item.et(),dt,1);
        Amp=::Interpolate(x,item.Amp,xx);
    }
    else Amp=item.Amp;

    delta=dt;
    begin_time=item.BeginTime();
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

        Dt=item.Delta();
        begin_time=item.BeginTime()+L1*item.Delta();
        EndTime=item.BeginTime()+L2*item.Delta();

        FindPeakAround(item.pt(),0.5);
        FileName=item.FileName;
        AmpMultiplier=item.AmpMultiplier;
    }
}

template<class T>
EvenSampledSignal::EvenSampledSignal (const std::vector<T> &item, const double &dt, const double &begintime, const std::string &infile) {
    Amp.resize(item.size());
    for (size_t i=0;i<size();++i) Amp[i]=item[i];
    delta=dt;
    begin_time=EndTime=begintime;
    if (size()>1) EndTime=begin_time+Dt*(size()-1);
    FileName=infile;
}

// Member function definitions.

// Cut the data within a window. If cut failed, don't cut and return false.
bool EvenSampledSignal::CheckAndCutToWindow(const double &t1, const double &t2){

    if (!CheckWindow(t1,t2)) return false;

    // Cut.
    std::size_t d1=ceil((t1-BeginTime())/Delta()),d2=1+floor((t2-BeginTime())/Delta());

    EndTime=begin_time+(d2-1)*Delta();
    begin_time+=d1*Delta();

    std::vector<double> NewAmp(Amp.begin()+d1,Amp.begin()+d2);
    std::swap(NewAmp,Amp);

    if (Peak>=d1) Peak-=d1;
    else Peak=0;
    if (Peak>size()) Peak=0;
    return true;
}

// Find the position of max|amp| around given time.
void EvenSampledSignal::FindPeakAround(const double &t, const double &w){
    std::size_t WB=std::max(0.0,(t-w-begin_time)/Dt),WE=std::min(size(),(size_t)ceil((t+w-begin_time+Dt/2)/Dt));

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
        double len=std::min(i,size()-i-1)*Dt;
        if (len<wl) Amp[i]*=0.5-0.5*cos(len/wl*M_PI);
    }
}

std::size_t EvenSampledSignal::LocateTime(const double &t) const{
    if (t<BeginTime() && BeginTime()-t<Delta()/2) return 0;
    if (t>et() && t-et()<Delta()/2) return size()-1;
    if (t<BeginTime() || t>et()) return -1;
    std::size_t ans=(t-BeginTime())/Delta();
    if (ans+1==size()) return ans;
    if (fabs(t-(BeginTime()+ans*Delta()))<fabs(t-(BeginTime()+(ans+1)*Delta()))) return ans;
    else return ans+1;
}

// Print some info.
void EvenSampledSignal::PrintInfo() const {
    DigitalSignal::PrintInfo();
    std::cout << "Sampling rate: " << Delta() << '\n';
}

// remove drift and DC.
std::pair<double,double> EvenSampledSignal::RemoveTrend(){
    return ::RemoveTrend(Amp,Dt,begin_time);
}

// taper window half-length is wl, zero half-length is zl.
void EvenSampledSignal::ZeroOutHannTaper(const double &wl, const double &zl){
    if ((wl+zl)*2>length()) throw std::runtime_error("ZeroOutHanning window too wide.");
    for (size_t i=0;i<size();++i){
        double len=std::min(i,size()-i-1)*Dt;
        if (len<zl) Amp[i]=0;
        else if (len<zl+wl) Amp[i]*=0.5-0.5*cos((len-zl)/wl*M_PI);
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
    return ::SimpsonRule(Amp.begin(),Amp.end(),Delta(),f);
}

// butterworth filter.
// changes: Amp(value change).
void EvenSampledSignal::Butterworth(const double &f1, const double &f2, const int &order, const int &passes){
    ::Butterworth(Amp,Dt,f1,f2,order,passes);
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
    ::GaussianBlur(Amp,Dt,sigma);
}

// Integerate (from velocity to displacement).
void EvenSampledSignal::Integrate(){
    std::partial_sum(Amp.begin(),Amp.end(),Amp.begin());
    for (double &item: Amp) item*=Dt;
}

// Interpolate to certain sampling rate.
void EvenSampledSignal::Interpolate(const double &dt){
    *this=EvenSampledSignal (*this,dt);
}

// Measure SNR.
double EvenSampledSignal::SNR(const double &nt1, const double &nt2, const double &st1, const double &st2){
    int NB=ceil((nt1-BeginTime())/Delta()),NL=ceil((nt2-nt1)/Delta());
    int SB=ceil((st1-BeginTime())/Delta()),SL=ceil((st2-st1)/Delta());
    return ::SNR(Amp,NB,NL,SB,SL);
}

// Stretch the signal horizontally and vertically.
// Keep sampling rate the same, keep peak time the same, which means updates:
// begin_time, EndTime, Peak,
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

    // Fix peak time (by adjusting begin_time)
    ans.begin_time=OldPeakTime-ans.peak()*Delta();
    ans.EndTime=ans.begin_time+(ans.size()-1)*Delta();
    ans.FindPeakAround(pt(),1);

    return ans;
}

std::vector<double> EvenSampledSignal::GetTime() const {
    std::vector<double> ans;
    for (size_t i=0;i<size();++i) ans.push_back(begin_time+i*Dt);
    return ans;
}

// Make a t-stared waveform.
// Keep sampling rate the same, keep data length the same.
// Notice: Peak amplitude and position could be changed.
EvenSampledSignal EvenSampledSignal::Tstar(const double &ts, const double &tol) const{
    EvenSampledSignal ans(*this);
    if (ts<=0) return ans;

    // Create a t* operator.
    auto res=::TstarOperator(ts,Delta(),tol);
    EvenSampledSignal Ts(res.first,Delta(),-Delta()*res.second);
    Ts.FindPeakAround(0,1);
    ans.Convolve(Ts);
    return ans;
}

// Notice: remove trend and do a taper (10% of length) both on source and signal before the decon.
// Changes:
// Amp(signal length change),Peak(=Amp.size()/2),begin_time(relative to peak time),EndTime(relative to peak time)
void EvenSampledSignal::WaterLevelDecon(const EvenSampledSignal &source, const double &wl) {
    if (fabs(Dt-source.Dt)>1e-5)
        throw std::runtime_error("Signal inputs of decon have different sampling rate.");

    HannTaper(0.1*length());
    RemoveTrend();

    auto S=source.Amp;
    ::HannTaper(S,0.1);
    ::RemoveTrend(S,Dt,source.BeginTime());

    Amp=::WaterLevelDecon(Amp,Peak,S,source.Peak,Dt,wl);
    Peak=size()/2;
    begin_time=(Peak-1)*Dt*-1;
    EndTime=begin_time+(size()-1)*Dt;
}

// Stack two same sampling rate, same begintime signal.
EvenSampledSignal &EvenSampledSignal::operator+=(const EvenSampledSignal &item){
    if (fabs(Delta()-item.Delta())>1e-5)
        throw std::runtime_error("Tried to stack two signals with different sampling rate: "+std::to_string(Delta())+" v.s. "+std::to_string(item.Delta()));
    if (fabs(BeginTime()-item.BeginTime())>1e-5)
        throw std::runtime_error("Tried to stack two signals with different begin time: "+std::to_string(BeginTime())+" v.s. "+std::to_string(item.BeginTime()));
    if (size()!=item.size())
        throw std::runtime_error("Tried to stack two signals with different number of points: "+std::to_string(size())+" v.s. "+std::to_string(item.size()));
    for (size_t i=0;i<size();++i) Amp[i]+=item.Amp[i];
    return *this;
}

/* -----------------------------------------------------------------------------
End of member function/operator definitions. -----------------------------------
----------------------------------------------------------------------------- */

// Compare two signals around their peaks.
// t1 t2 are time window relative to the peak (in seconds, e.g. t1=-5, t2=5)
SignalCompareResults CompareSignal(EvenSampledSignal S1, EvenSampledSignal S2, const double &t1, const double &t2, const double &AmpLevel){
    if (fabs(S1.Delta()-S2.Delta())>0.01*S1.Delta()) throw std::runtime_error("Comparing two differently sampled signals.");
    S1.NormalizeToPeak();
    S2.NormalizeToPeak();
    return ::CompareSignal(S1.Amp,S1.peak(),S2.Amp,S2.peak(),S1.Delta(),t1,t2,AmpLevel);
}

std::pair<std::pair<int,double>,std::vector<double>> CrossCorrelation(const EvenSampledSignal &S1, const double &t1, const double &t2, const EvenSampledSignal &S2, const double &h1, const double &h2, const bool &Dump, const int &Flip, const std::pair<int,int> &ShiftLimit){
    // Check window position.
    if (!S1.CheckWindow(t1,t2)) throw std::runtime_error("CrossCorrelation window on signal 1 is not proper.");
    if (!S2.CheckWindow(h1,h2)) throw std::runtime_error("CrossCorrelation window on signal 2 is not proper.");

    return ::CrossCorrelation(S1.Amp.begin()+S1.LocateTime(t1),S1.Amp.begin()+S1.LocateTime(t2)+1,S2.Amp.begin()+S2.LocateTime(h1),S2.Amp.begin()+S2.LocateTime(h2)+1,Dump,Flip,ShiftLimit);

}

std::pair<EvenSampledSignal,EvenSampledSignal> StackSignals(const std::vector<EvenSampledSignal> &Signals, const std::vector<double> &Weights) {

    // Check size.
    std::size_t m=Signals.size();
    if (m==0) return {};
    if (!Weights.empty() && Weights.size()!=m)
        throw std::runtime_error("Input signals nubmer is different from weight number.");

    std::size_t n=Signals[0].size();
    double dt=Signals[0].Delta(),begintime=Signals[0].BeginTime();
    for (const auto &item:Signals) {
        if (item.size()!=n)
            throw std::runtime_error("Input signals have different lengths.");
        if (item.Delta()!=dt)
            throw std::runtime_error("Input signals have different sampling rates.");
        if (item.BeginTime()!=begintime)
            throw std::runtime_error("Input signals have different begin times.");
    }

    // Make stack.
    std::vector<double> V(m,0),W=Weights,S(n,0),STD(n,0);

    for (std::size_t i=0;i<n;++i) {
        for (std::size_t j=0;j<m;++j)
            V[j]=Signals[j].Amp[i];
        auto res=AvrStd(V,W);
        S[i]=res.first;
        STD[i]=res.second;
    }
    return {EvenSampledSignal(S,Signals[0].Delta(),Signals[0].BeginTime()),EvenSampledSignal(STD,Signals[0].Delta(),Signals[0].BeginTime())};
}

// Overload operator ">>" to read a signal from a two-columned input (stdin/file/etc.)
std::istream &operator>>(std::istream &is, EvenSampledSignal &item){

    item.Clear();
    double x,y,dt=-1;
    while (is >> x >> y) {
        if (item.Amp.empty()) item.begin_time=x;
        else {
            // Check the ascending of Time serise.
            if (x<=item.EndTime)
                throw std::runtime_error("Time serise is either not increasing or has repeating values.");

            // Check even sampling.
            if (dt<0) dt=x-item.EndTime;
            else if (x-item.EndTime<=dt*0.99 || dt*1.01<=x-item.EndTime)
                throw std::runtime_error("Input seems not even sampled.");
            else dt=(x-item.begin_time)/item.Amp.size();
        }

        item.Amp.push_back(y);
        item.EndTime=x;
    }

    item.Dt=(item.EndTime-item.begin_time);
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
        os << item.begin_time+item.Dt*i << '\t' << item.Amp[i] << '\n';

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
