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
    ~EvenSampledSignal () = default;

    // Override functions/operators declarations.

    double BeginTime() const override final {return begin_time;}
    void Clear() override {*this=EvenSampledSignal ();}
    double EndTime() const override {return BeginTime()+SignalDuration();}
    double PeakTime() const override {return BeginTime()+GetPeak()*GetDelta();}
    void ShiftTime(const double &t) override {begin_time+=t;}
    double SignalDuration() const override {return (Size()<=1?0:GetDelta()*(Size()-1));}

    bool CheckAndCutToWindow(const double &t1, const double &t2) override;
    void FindPeakAround(const double &t, const double &w=5) override;
    const std::vector<double> &GetTime() const override;
    void HannTaper(const double &wl) override;
    std::size_t LocateTime(const double &t) const override;
    void PrintInfo() const override;
    std::pair<double,double> RemoveTrend() override;
    void ZeroOutHannTaper(const double &wl, const double &zl) override;


    // Declaration of first appear and final functions/operators.
    // They are not virtual to increase performance.
    // Therefore in drived class, care is needed not to define these function again.
    // Also, because they are not virtual with final flag,
    // you need to guarantee they behaves well for all derived classes.
    // Because they are intended unchangeable, only protected and public memebers can appear here(?)


    double AbsIntegral() const;
    void Butterworth(const double &f1, const double &f2, const int &order=2, const int &passes=2);
    void Convolve(const EvenSampledSignal &item);
    void GaussianBlur(const double &sigma=1);
    double GetDelta() const {return delta;}
    void Integrate();
    void Interpolate(const double &dt);
    double SNR(const double &nt1, const double &nt2, const double &st1, const double &st2);
    EvenSampledSignal Stretch(const double &h=1) const;
    EvenSampledSignal Tstar(const double &ts, const double &tol=1e-3) const;
    void WaterLevelDecon(const EvenSampledSignal &source, const double &wl=0.1);
    // operator+= is overloaded, need "using" to make the version in DigitalSignal visible.
    using DigitalSignal::operator+=;
    EvenSampledSignal &operator+=(const EvenSampledSignal &item);


    // declaration of non-member class/function/operators, who need access to
    // the private/protected parts of this class, therefore they need to be friend.
    // ??? friend class SACSignals;
    friend SignalCompareResults CompareSignal(EvenSampledSignal S1, EvenSampledSignal S2, const double &t1, const double &t2, const double &AmpLevel);
    friend std::pair<std::pair<int,double>,std::vector<double>> CrossCorrelation(const EvenSampledSignal &S1, const double &t1, const double &t2, const EvenSampledSignal &S2, const double &h1, const double &h2, const bool &Dump=false, const int &Flip=0, const std::pair<int,int> &ShiftLimit={std::numeric_limits<int>::min(),std::numeric_limits<int>::max()});
    friend std::pair<EvenSampledSignal,EvenSampledSignal> StackSignals (const std::vector<EvenSampledSignal> &Signals, const std::vector<double> &Weights={});
    friend std::istream &operator>>(std::istream &is, EvenSampledSignal &item);
    friend std::ostream &operator<<(std::ostream &os, const EvenSampledSignal &item);

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

    FindPeakAround(item.pt(),0.5);
    FileName=item.FileName;
    AmpMultiplier=item.AmpMultiplier;
}

EvenSampledSignal::EvenSampledSignal (const EvenSampledSignal &item, const double &dt) {

    // If sampling rate is not the same, interpolate to dt.
    if (item.GetDelta()<=dt*0.99 || dt*1.01<=item.GetDelta()){
        auto x=::CreateGrid(item.BeginTime(),item.et(),item.Size(),0);
        auto xx=::CreateGrid(item.BeginTime(),item.et(),dt,1);
        Amp=::Interpolate(x,item.Amp,xx);
    }
    else Amp=item.Amp;

    delta=dt;
    begin_time=item.BeginTime();

    FindPeakAround(item.pt(),0.5);
    FileName=item.FileName;
    AmpMultiplier=item.AmpMultiplier;
}

EvenSampledSignal::EvenSampledSignal (const EvenSampledSignal &item, const double &t1, const double &t2) {
    *this=EvenSampledSignal();

    if (item.CheckWindow(t1,t2)){

        size_t L1=item.LocateTime(t1),L2=item.LocateTime(t2);
        Amp=std::vector<double> (item.Amp.begin()+L1,item.Amp.begin()+L2+1);

        delta=item.GetDelta();
        begin_time=item.BeginTime()+L1*item.GetDelta();

        FindPeakAround(item.pt(),0.5);
        FileName=item.FileName;
        AmpMultiplier=item.AmpMultiplier;
    }
}

template<class T>
EvenSampledSignal::EvenSampledSignal (const std::vector<T> &item, const double &dt, const double &begintime, const std::string &infile) {
    Amp.resize(item.Size());
    for (size_t i=0;i<Size();++i) Amp[i]=item[i];
    delta=dt;
    begin_time=begintime;
    FileName=infile;
}

// Member function definitions.

// Cut the data within a window. If cut failed, don't cut and return false.
bool EvenSampledSignal::CheckAndCutToWindow(const double &t1, const double &t2){

    if (!CheckWindow(t1,t2)) return false;

    // Cut.
    std::size_t d1=ceil((t1-BeginTime())/GetDelta()),d2=1+floor((t2-BeginTime())/GetDelta());

    begin_time+=d1*GetDelta();

    std::vector<double> NewAmp(Amp.begin()+d1,Amp.begin()+d2);
    std::swap(NewAmp,Amp);

    if (Peak>=d1) Peak-=d1;
    else Peak=0;
    if (Peak>Size()) Peak=0;
    return true;
}

// Find the position of max|amp| around given time.
void EvenSampledSignal::FindPeakAround(const double &t, const double &w){
    std::size_t WB=std::max(0.0,(t-w-begin_time)/delta),WE=std::min(Size(),(size_t)ceil((t+w-begin_time+delta/2)/delta));

    double AbsMax=-1;
    for (std::size_t i=WB;i<WE;++i){
        if (AbsMax<fabs(Amp[i])) {
            AbsMax=fabs(Amp[i]);
            Peak=i;
        }
    }
}

const std::vector<double> &EvenSampledSignal::GetTime() const {
    std::vector<double> ans;
    for (size_t i=0;i<Size();++i)
        ans.push_back(BeginTime()+i*GetDelta());
    return ans;
}


// cos (-pi,pi) shaped taper at two ends.
void EvenSampledSignal::HannTaper(const double &wl){
    if (wl*2>length()) throw std::runtime_error("Hanning window too wide.");
    for (size_t i=0;i<Size();++i){
        double len=std::min(i,Size()-i-1)*delta;
        if (len<wl) Amp[i]*=0.5-0.5*cos(len/wl*M_PI);
    }
}

std::size_t EvenSampledSignal::LocateTime(const double &t) const{
    if (t<BeginTime() && BeginTime()-t<GetDelta()/2) return 0;
    if (t>et() && t-et()<GetDelta()/2) return Size()-1;
    if (t<BeginTime() || t>et()) return -1;
    std::size_t ans=(t-BeginTime())/GetDelta();
    if (ans+1==Size()) return ans;
    if (fabs(t-(BeginTime()+ans*GetDelta()))<fabs(t-(BeginTime()+(ans+1)*GetDelta()))) return ans;
    else return ans+1;
}

// Print some info.
void EvenSampledSignal::PrintInfo() const {
    DigitalSignal::PrintInfo();
    std::cout << "Sampling rate: " << GetDelta() << '\n';
}

// remove drift and DC.
std::pair<double,double> EvenSampledSignal::RemoveTrend(){
    return ::RemoveTrend(Amp,delta,begin_time);
}

// taper window half-length is wl, zero half-length is zl.
void EvenSampledSignal::ZeroOutHannTaper(const double &wl, const double &zl){
    if ((wl+zl)*2>length()) throw std::runtime_error("ZeroOutHanning window too wide.");
    for (size_t i=0;i<Size();++i){
        double len=std::min(i,Size()-i-1)*delta;
        if (len<zl) Amp[i]=0;
        else if (len<zl+wl) Amp[i]*=0.5-0.5*cos((len-zl)/wl*M_PI);
    }
}

// Take absolute amplitude then do the integral.
double EvenSampledSignal::AbsIntegral() const {
    auto f=[](const double &s){return (s>0?s:-s);};
    return ::SimpsonRule(Amp.begin(),Amp.end(),GetDelta(),f);
}

// butterworth filter.
// changes: Amp(value change).
void EvenSampledSignal::Butterworth(const double &f1, const double &f2, const int &order, const int &passes){
    ::Butterworth(Amp,delta,f1,f2,order,passes);
}

// Convolve with another signal S2, truncated relative to S2'peak position. (keep s1's size).
// This is acausal convolution, trying to keep the original peak's position.
void EvenSampledSignal::Convolve(const EvenSampledSignal &item){
    auto res=::Convolve(Amp,item.Amp);
    std::rotate(res.begin(),res.begin()+item.peak(),res.end());
    res.resize(Size());
    Amp=res;
}

// gaussian blur.
// changes: Amp(value change).
void EvenSampledSignal::GaussianBlur(const double &sigma){
    ::GaussianBlur(Amp,delta,sigma);
}

// Integerate (from velocity to displacement).
void EvenSampledSignal::Integrate(){
    std::partial_sum(Amp.begin(),Amp.end(),Amp.begin());
    for (double &item: Amp) item*=delta;
}

// Interpolate to certain sampling rate.
void EvenSampledSignal::Interpolate(const double &dt){
    *this=EvenSampledSignal (*this,dt);
}

// Measure SNR.
double EvenSampledSignal::SNR(const double &nt1, const double &nt2, const double &st1, const double &st2){
    int NB=ceil((nt1-BeginTime())/GetDelta()),NL=ceil((nt2-nt1)/GetDelta());
    int SB=ceil((st1-BeginTime())/GetDelta()),SL=ceil((st2-st1)/GetDelta());
    return ::SNR(Amp,NB,NL,SB,SL);
}

// Stretch the signal horizontally and vertically.
// Keep sampling rate the same, keep peak time the same, which means updates:
// begin_time, peak,
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
    ans.Peak=ans.Size();
    auto S=::StretchSignal(std::vector<double> (Amp.begin()+peak(),Amp.end()),h);
    ans.Amp.insert(ans.Amp.end(),S.begin(),S.end());

    // Fix peak time (by adjusting begin_time)
    ans.begin_time=OldPeakTime-ans.peak()*GetDelta();
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
    auto res=::TstarOperator(ts,GetDelta(),tol);
    EvenSampledSignal Ts(res.first,GetDelta(),-GetDelta()*res.second);
    Ts.FindPeakAround(0,1);
    ans.Convolve(Ts);
    return ans;
}

// Notice: remove trend and do a taper (10% of length) both on source and signal before the decon.
// Changes:
// Amp(signal length change),Peak(=Amp.size()/2),begin_time(relative to peak time)
void EvenSampledSignal::WaterLevelDecon(const EvenSampledSignal &source, const double &wl) {
    if (fabs(delta-source.delta)>1e-5)
        throw std::runtime_error("Signal inputs of decon have different sampling rate.");

    HannTaper(0.1*length());
    RemoveTrend();

    auto S=source.Amp;
    ::HannTaper(S,0.1);
    ::RemoveTrend(S,delta,source.BeginTime());

    Amp=::WaterLevelDecon(Amp,Peak,S,source.Peak,delta,wl);
    Peak=Size()/2;
    begin_time=(Peak-1)*delta*-1;
}

// Stack two same sampling rate, same begintime signal.
EvenSampledSignal &EvenSampledSignal::operator+=(const EvenSampledSignal &item){
    if (fabs(GetDelta()-item.GetDelta())>1e-5)
        throw std::runtime_error("Tried to stack two signals with different sampling rate: "+std::to_string(GetDelta())+" v.s. "+std::to_string(item.GetDelta()));
    if (fabs(BeginTime()-item.BeginTime())>1e-5)
        throw std::runtime_error("Tried to stack two signals with different begin time: "+std::to_string(BeginTime())+" v.s. "+std::to_string(item.BeginTime()));
    if (Size()!=item.Size())
        throw std::runtime_error("Tried to stack two signals with different number of points: "+std::to_string(Size())+" v.s. "+std::to_string(item.Size()));
    for (size_t i=0;i<Size();++i) Amp[i]+=item.Amp[i];
    return *this;
}

/* -----------------------------------------------------------------------------
End of member function/operator definitions. -----------------------------------
----------------------------------------------------------------------------- */

// Compare two signals around their peaks.
// t1 t2 are time window relative to the peak (in seconds, e.g. t1=-5, t2=5)
SignalCompareResults CompareSignal(EvenSampledSignal S1, EvenSampledSignal S2, const double &t1, const double &t2, const double &AmpLevel){
    if (fabs(S1.GetDelta()-S2.GetDelta())>0.01*S1.GetDelta()) throw std::runtime_error("Comparing two differently sampled signals.");
    S1.NormalizeToPeak();
    S2.NormalizeToPeak();
    return ::CompareSignal(S1.Amp,S1.peak(),S2.Amp,S2.peak(),S1.GetDelta(),t1,t2,AmpLevel);
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

    std::size_t n=Signals[0].Size();
    double dt=Signals[0].GetDelta(),begintime=Signals[0].BeginTime();
    for (const auto &item:Signals) {
        if (item.Size()!=n)
            throw std::runtime_error("Input signals have different lengths.");
        if (item.GetDelta()!=dt)
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
    return {EvenSampledSignal(S,Signals[0].GetDelta(),Signals[0].BeginTime()),EvenSampledSignal(STD,Signals[0].GetDelta(),Signals[0].BeginTime())};
}

// Overload operator ">>" to read a signal from a two-columned input (stdin/file/etc.)
std::istream &operator>>(std::istream &is, EvenSampledSignal &item){

    item.Clear();
    double x,y,dt=-1;
    while (is >> x >> y) {
        if (item.Amp.empty()) item.begin_time=x;
        else {
            // Check the ascending of Time serise.
            if (x<=item.EndTime())
                throw std::runtime_error("Time serise is either not increasing or has repeating values.");

            // Check even sampling.
            if (dt<0) dt=x-item.EndTime();
            else if (x-item.EndTime()<=dt*0.99 || dt*1.01<=x-item.EndTime())
                throw std::runtime_error("Input seems not even sampled.");
            else dt=(x-item.begin_time)/item.Size();
        }

        item.Amp.push_back(y);
    }

    item.delta=(item.EndTime()-item.begin_time);
    if (item.Size()>1) item.delta/=(item.Size()-1);

    return is;
}

// Overload operator "<<" to print a signal in a two-columned format.
// Customize this section code to define other printing format.
std::ostream &operator<<(std::ostream &os, const EvenSampledSignal &item){

    /*

    // record original format.
    std::ios state(NULL);
    state.copyfmt(os);

    // set new printing format.
    os << std::fixed << std::scientific << std::setprecision();

                                    */

    for (std::size_t i=0;i<item.Size();++i)
        os << item.BeginTime()+item.GetDelta()*i << '\t' << item.GetAmp()[i] << (i+1==item.Size()?"":"\n");

    /*

    // restore previous format.
    os.copyfmt(state);

                                    */
    return os;
}

// Overload operator "+,-" to ShiftDC.
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
