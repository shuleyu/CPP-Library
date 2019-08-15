#ifndef ASU_EVENSAMPLEDSIGNAL
#define ASU_EVENSAMPLEDSIGNAL

#include<vector>
#include<cmath>
#include<string>
#include<fstream>
#include<numeric>
#include<algorithm>

#include<AvrStd.hpp>
#include<Butterworth.hpp>
#include<CompareSignal.hpp>
#include<Convolve.hpp>
#include<CreateGrid.hpp>
#include<CrossCorrelation.hpp>
#include<DigitalSignal.hpp>
#include<Diff.hpp>
#include<FFT.hpp>
#include<GaussianBlur.hpp>
#include<HannTaper.hpp>
#include<IFFT.hpp>
#include<Interpolate.hpp>
#include<RemoveTrend.hpp>
#include<SimpsonRule.hpp>
#include<SNR.hpp>
#include<StretchSignal.hpp>
#include<TstarOperator.hpp>
#include<WaterLevelDecon.hpp>

class EvenSampledSignal : public DigitalSignal {

private:

    /* Not inherited (private) member of DigitalSignal.

//     std::vector<double> time;

    */

protected:


/*     protected members inherited from DigitalSignal.

//     std::vector<double> amp;
//     std::size_t peak;
//     std::string filename;
//     double amp_multiplier;
                                              */
    double delta=0,begin_time=0;

    // An auxiliary data designed for GetTime();
    std::vector<double> aux_time;

    void AddStripSignal(const EvenSampledSignal &s2, const double &dt=0, const bool &flag=true);

public:

    // Constructor/Destructors.
    EvenSampledSignal ();
    EvenSampledSignal (const std::string &infile);                                 // 2-column file.
    EvenSampledSignal (const std::string &infile,                                  // 1-column file.
                       const double &dt, const double &bt=0);
    EvenSampledSignal (const DigitalSignal &item, const double &dt);
    EvenSampledSignal (const EvenSampledSignal &item) = default;
    EvenSampledSignal (const EvenSampledSignal &item, const double &dt);
    template<typename T> EvenSampledSignal (const std::vector<T> &item, const double &dt,
                                            const double &bt=0, const std::string &infile="");
    ~EvenSampledSignal () = default;

    // Override functions/operators declarations.

    void Identify () const override {std::cout << "Using EvenSampledSignal methods" << std::endl;}

    double BeginTime() const override final {return begin_time;}
    void Clear() override {*this=EvenSampledSignal ();}
    double EndTime() const override final {return BeginTime()+SignalDuration();}
    double PeakAmp() const override final {return GetAmp()[GetPeak()];}
    double PeakTime() const override final {return BeginTime()+GetPeak()*GetDelta();}
    void ShiftTime(const double &t) override final {begin_time+=t;}
    double SignalDuration() const override final {return (Size()<=1?0:GetDelta()*(Size()-1));}

    bool CheckAndCutToWindow(const double &t1, const double &t2) override final;
    void FindPeakAround(const double &t, const double &w=5, const bool &positiveOnly=false) override final;
    const std::vector<double> &GetTime() const override final;
    void HannTaper(const double &wl) override final;
    std::size_t LocateTime(const double &t) const override final;
    void OutputToFile(const std::string &s) const override;
    void PrintInfo() const override;
    std::pair<double,double> RemoveTrend() override final;
    double SumArea(const double &t1=-std::numeric_limits<double>::max(),
                   const double &t2=std::numeric_limits<double>::max(), const size_t &mode=0) const override final;
    void ZeroOutHannTaper(const double &wl, const double &zl) override final;


    // Declaration of first appear and final functions/operators.
    // They are not virtual to increase performance.
    // Therefore in drived class, care is needed not to define these function again.
    // Also, because they are not virtual with final flag,
    // you need to guarantee they behaves well for all derived classes.
    // Because they are intended unchangeable, only protected and public memebers can appear here(?)


    double GetDelta() const {return delta;}

    double AbsIntegral() const;
    void AddSignal(const EvenSampledSignal &s2, const double &dt=0);
    void Butterworth(const double &f1, const double &f2, const int &order=2, const int &passes=2);
    std::pair<double,double> CrossCorrelation(const double &t1, const double &t2,
                                              const EvenSampledSignal &S2, const double &h1, const double &h2,
                                              const int &Flip=0, const std::pair<int,int> &ShiftLimit=
                                              {std::numeric_limits<int>::min(),std::numeric_limits<int>::max()}) const;
    void Convolve(const EvenSampledSignal &item);
    void Diff();
    std::pair<EvenSampledSignal,EvenSampledSignal> FFT(const bool &ReturnAmpAndPhase=true) const;
    void FlipReverseSum(const double &t);
    void GaussianBlur(const double &sigma=1);
    void Integrate();
    void Interpolate(const double &dt);
    double SNR(const double &nt1, const double &nt2, const double &st1, const double &st2) const;
    EvenSampledSignal Stretch(const double &h=1) const;
    void StripSignal(const EvenSampledSignal &s2, const double &dt=0);
    EvenSampledSignal Tstar(const double &ts, const double &tol=1e-3) const;
    void WaterLevelDecon(const EvenSampledSignal &source, const double &wl=0.1);
    // notice operator+=, operator-= is overloaded,
    // need "using" to make the DigitalSignal version visible.
    using DigitalSignal::operator+=;
    using DigitalSignal::operator-=;
    EvenSampledSignal &operator+=(const EvenSampledSignal &item);
    EvenSampledSignal &operator-=(const EvenSampledSignal &item);

    // declaration of non-member class/function/operators as friend.
    // Input operator >> need access to the private/protected parts of this class,
    // therefore it needs to be friend.
    friend std::istream &operator>>(std::istream &is, EvenSampledSignal &item);

}; // End of class declaration.

// Constructors/Destructors definition.
EvenSampledSignal::EvenSampledSignal () {
    // The default base constructor DigitalSignal () is automatically called when
    // no other base constructor is called.
    delta=0;
    begin_time=0;
}

EvenSampledSignal::EvenSampledSignal (const std::string &infile) {
    std::ifstream fpin(infile);
    fpin >> *this;
    if (!fpin.eof()) {                            // failed to read from even sampled 2-column file.
        fpin.close();
        std::cerr << "Warning: when constructing EvenSampledSignal, the input file: " << infile
                  << " shows sign of un-even sampling. Using interpolation ..."<< std::endl;

        DigitalSignal item=DigitalSignal(infile);
        double dt=item.SignalDuration()/(item.Size()-1);
        *this=EvenSampledSignal(item,dt);
    }
    else {
        filename=infile;
        fpin.close();
    }
}


EvenSampledSignal::EvenSampledSignal (const std::string &infile,
                                      const double &dt, const double &bt) {
    std::ifstream fpin(infile);
    double y;
    while (fpin >> y) amp.push_back(y);
    fpin.close();

    delta=dt;
    begin_time=bt;
    filename=infile;
}


EvenSampledSignal::EvenSampledSignal (const DigitalSignal &item, const double &dt) {

    // Interpolation.
    auto xx=::CreateGrid(item.BeginTime(),item.EndTime(),dt,1);
    amp=::Interpolate(item.GetTime(),item.GetAmp(),xx);

    delta=dt;
    begin_time=item.BeginTime();
    filename=item.GetFileName();
    amp_multiplier=item.GetAmpMultiplier();

    if (item.GetPeak()!=(std::size_t)-1) FindPeakAround(item.PeakTime(),10*GetDelta());
}

EvenSampledSignal::EvenSampledSignal (const EvenSampledSignal &item, const double &dt) {

    // If sampling rate is not the same, interpolate to dt.
    if (item.GetDelta()<=dt*0.99 || dt*1.01<=item.GetDelta()){
        auto xx=::CreateGrid(item.BeginTime(),item.EndTime(),dt,1);
        amp=::Interpolate(item.GetTime(),item.GetAmp(),xx);
    }
    else amp=item.GetAmp();

    delta=dt;
    begin_time=item.BeginTime();
    filename=item.GetFileName();
    amp_multiplier=item.GetAmpMultiplier();

    if (item.GetPeak()!=(std::size_t)-1) FindPeakAround(item.PeakTime(),10*GetDelta());
}

template<typename T>
EvenSampledSignal::EvenSampledSignal (const std::vector<T> &item, const double &dt,
                                      const double &bt, const std::string &infile) {
    amp.resize(item.size());
    for (std::size_t i=0;i<Size();++i) amp[i]=item[i];
    delta=dt;
    begin_time=bt;
    filename=infile;
}

// Member function definitions.

// Cut the data within a window. If cut failed, don't cut and return false.
bool EvenSampledSignal::CheckAndCutToWindow(const double &t1, const double &t2){

    if (!CheckWindow(t1,t2)) return false;

    // Cut.
    std::size_t d1=LocateTime(t1),d2=LocateTime(t2);
    ++d2;

    std::vector<double> NewAmp(GetAmp().begin()+d1,GetAmp().begin()+d2);
    std::swap(NewAmp,amp);

    if (GetPeak()<d2 && GetPeak()>=d1) peak-=d1;
    else peak=-1;

    begin_time+=d1*GetDelta();

    return true;
}


// Find the position of max|amp| around given time.
void EvenSampledSignal::FindPeakAround(const double &t, const double &w, const bool &positiveOnly){
    std::size_t d1=LocateTime(t-w),d2=LocateTime(t+w);
    ++d2;

    peak=d1;

    if (positiveOnly) {
        double Max=GetAmp()[d1];
        for (std::size_t i=d1+1;i<d2;++i){
            if (Max<GetAmp()[i]) {
                Max=GetAmp()[i];
                peak=i;
            }
        }
    }
    else {
        double Max=fabs(GetAmp()[d1]);
        for (std::size_t i=d1+1;i<d2;++i){
            if (Max<fabs(GetAmp()[i])) {
                Max=fabs(GetAmp()[i]);
                peak=i;
            }
        }
    }
}

const std::vector<double> &EvenSampledSignal::GetTime() const {
    std::vector<double> &time=const_cast<std::vector<double> &> (aux_time);
    if (time.size()!=Size()) time.resize(Size());
    for (std::size_t i=0;i<Size();++i) time[i]=BeginTime()+i*GetDelta();
    return aux_time;
}

// cos (-pi,pi) shaped taper at two ends.
void EvenSampledSignal::HannTaper(const double &wl){
    if (wl*2>SignalDuration()) throw std::runtime_error("Hanning window too wide.");
    for (std::size_t i=0;i<Size();++i){
        double len=std::min(i,Size()-1-i)*GetDelta();
        if (len<wl) amp[i]*=0.5-0.5*cos(len/wl*M_PI);
    }
}

// Return a std::size_t between [0,Size()-1]
std::size_t EvenSampledSignal::LocateTime(const double &t) const{
    if (t<BeginTime()) return 0;
    if (t>EndTime()) return Size()-1;
    std::size_t ans=(t-BeginTime())/GetDelta();
    if (ans+1==Size()) return ans;
    if (t-(BeginTime()+ans*GetDelta())<(BeginTime()+(ans+1)*GetDelta())-t) return ans;
    else return ans+1;
}

// Print some info.
void EvenSampledSignal::PrintInfo() const {
    DigitalSignal::PrintInfo();
    std::cout << "Sampling rate: " << GetDelta() << '\n';
}

// remove drift and DC.
std::pair<double,double> EvenSampledSignal::RemoveTrend(){
    return ::RemoveTrend(amp,GetDelta(),BeginTime());
}

// area under the curve, with different modes according to different operators.
double EvenSampledSignal::SumArea(const double &t1, const double &t2, const size_t &mode) const {
    if (t1>t2) throw std::runtime_error("In SumArea, t2<t1 ...");
    std::size_t p1=LocateTime(t1),p2=LocateTime(t2);
    if (p1==p2) return 0;

    double ans=0;

    switch (mode) {

        case 1: // sum(|p|)
            for (std::size_t i=p1;i<p2;++i) ans+=fabs(amp[i]);
            break;

        case 2: // sum(p*p)
            for (std::size_t i=p1;i<p2;++i) ans+=amp[i]*amp[i];
            break;

        default: // sum(p)
            for (std::size_t i=p1;i<p2;++i) ans+=amp[i];
            break;
    }

    return ans/(p2-p1);
}

// taper window half-length is wl, zero half-length is zl.
void EvenSampledSignal::ZeroOutHannTaper(const double &wl, const double &zl){
    if ((wl+zl)*2>SignalDuration()) throw std::runtime_error("ZeroOutHanning window too wide.");
    for (std::size_t i=0;i<Size();++i){
        double len=std::min(i,Size()-1-i)*GetDelta();
        if (len<zl) amp[i]=0;
        else if (len<zl+wl) amp[i]*=0.5-0.5*cos((len-zl)/wl*M_PI);
    }
}

// End of virtual functions.

// First appear and non-virtual functions.

// Take absolute amplitude then do the integral.
// Calculate the energy.
double EvenSampledSignal::AbsIntegral() const {
    auto f=[](const double &s){return (s>0?s:-s);};
    return ::SimpsonRule(GetAmp().begin(),GetAmp().end(),GetDelta(),f);
}

// Try to add the signal s2; shift s2 before the addition for optional input: dt.
// Will only alter the overlapping part.
// Sampling rate should be the same.
// Difference from operator+ : not as strict as operator +, signal length/begin time can be different.
void EvenSampledSignal::AddSignal(const EvenSampledSignal &s2, const double &dt) {
    AddStripSignal(s2,dt,true);
}

void EvenSampledSignal::AddStripSignal(const EvenSampledSignal &s2, const double &dt, const bool &flag){
    
    if (fabs(GetDelta()-s2.GetDelta())>1e-5)
        throw std::runtime_error("Tried to "+std::string(flag?"add":"strip")+" signal with different sampling rate.");
    double t1=s2.BeginTime()+dt,t2=s2.EndTime()+dt;
    if (EndTime()<t1 || t2<BeginTime())
        return;
    t1=std::max(t1,BeginTime());
    t2=std::min(t2,EndTime());
    std::size_t S2Begin=s2.LocateTime(t1-dt);
    std::size_t S1Begin=LocateTime(t1);

    int mul=(flag?1:-1);
    for (std::size_t i=S1Begin;i<=LocateTime(t2);++i) {
        size_t index=S2Begin+i-S1Begin;
        if (0<=index && index<s2.Size())
            amp[i]+=s2.amp[index]*mul;
    }
    return;
}

// butterworth filter.
void EvenSampledSignal::Butterworth(const double &f1, const double &f2,
                                    const int &order, const int &passes){
    ::Butterworth(amp,GetDelta(),f1,f2,order,passes);
}

// Cross correlate current signal with input signal within their own window.
// Notice we are returning the time shift (in second)
std::pair<double,double> EvenSampledSignal::CrossCorrelation(const double &t1, const double &t2,
                                                             const EvenSampledSignal &S2, const double &h1, const double &h2,
                                                             const int &Flip, const std::pair<int,int> &ShiftLimit) const {
    // Check window position.
    if (!CheckWindow(t1,t2)) {
        std::cerr << "CrossCorrelation window on signal 1 is not proper." << std::endl;
        //throw std::runtime_error("CrossCorrelation window on signal 1 is not proper.");
        return {};
    }
    if (!S2.CheckWindow(h1,h2)) {
        std::cerr << "CrossCorrelation window on signal 2 is not proper." << std::endl;
        // throw std::runtime_error("CrossCorrelation window on signal 2 is not proper.");
        return {};
    }


    auto res=::CrossCorrelation(GetAmp().begin()+LocateTime(t1),
                              GetAmp().begin()+LocateTime(t2)+1,
                              S2.GetAmp().begin()+S2.LocateTime(h1),
                              S2.GetAmp().begin()+S2.LocateTime(h2)+1,false,Flip,ShiftLimit);
    return std::make_pair(res.first.first*GetDelta(),res.first.second);
}


// Convolve with another signal s2, truncated to keep s1's size.
// The place to truncated depends on S2'peak position.
// This is acausal convolution: I was trying to keep the original peak's position.
void EvenSampledSignal::Convolve(const EvenSampledSignal &s2){
    std::size_t OrignalSize=Size();
    amp=::Convolve(GetAmp(),s2.GetAmp());
    std::rotate(amp.begin(),amp.begin()+s2.GetPeak(),amp.end());
    amp.resize(OrignalSize);
}

// Differentiation (from displacement to velocity).
// Notice this will decrease the length of the signal by 1.
void EvenSampledSignal::Diff(){
    if (Size()<=1) return;
    auto NewAmp=::Diff(amp);
    std::swap(amp,NewAmp);
    *this/=GetDelta();
}

std::pair<EvenSampledSignal,EvenSampledSignal>
EvenSampledSignal::FFT(const bool &ReturnAmpAndPhase) const {

    if (Size()==1) return {};

    auto res=::FFT(GetAmp(),GetDelta(),ReturnAmpAndPhase);
    return {EvenSampledSignal(res.first,1.0/2/GetDelta()/(res.first.size()-1),0),
            EvenSampledSignal(res.second,1.0/2/GetDelta()/(res.second.size()-1),0)};
}


// FRS at the given time.
// If the given time is outside of the signal, do nothing.
// Cut the signal at the given time, then
// flip the first part, reverse the first part and sum it to the second part.
// (as a result the given time becomes the begin time.)
// Will maintain the maximum valid length.
// Will do a time shift such that the FRS traces starts at t=0.
void EvenSampledSignal::FlipReverseSum(const double &t) {
    if (t<BeginTime() || t>EndTime()) {
        std::cerr << "In FRS, the given time is invalid." << std::endl;
        return;
    }
    std::size_t l=LocateTime(t),new_npts=std::min(l+1,Size()-l);
    std::vector<double> new_amp(new_npts,0);
    for (std::size_t i=0;i<new_npts;++i)
        new_amp[i]=GetAmp()[l+i]-GetAmp()[l-i];

    EvenSampledSignal new_signal(new_amp,GetDelta());
    std::swap(*this,new_signal);
    return;
}

// gaussian blur.
// changes: amp(value change).
void EvenSampledSignal::GaussianBlur(const double &sigma){
    ::GaussianBlur(amp,GetDelta(),sigma);
}

// Integrate (from velocity to displacement).
void EvenSampledSignal::Integrate(){
    std::partial_sum(amp.begin(),amp.end(),amp.begin());
    *this*=GetDelta();
}

// Interpolate to certain sampling rate.
void EvenSampledSignal::Interpolate(const double &dt){
    *this=EvenSampledSignal (*this,dt);
}

// Measure SNR.
double EvenSampledSignal::SNR(const double &nt1, const double &nt2,
                              const double &st1, const double &st2) const{
    if (!CheckWindow(nt1,nt2) || !CheckWindow(st1,st2))
        throw std::runtime_error("SNR measuring error: window not suitable.");

    std::size_t n1=LocateTime(nt1),n2=LocateTime(nt2),s1=LocateTime(st1),s2=LocateTime(st2);
    return ::SNR(GetAmp(),n1,n2-n1,s1,s2-s1);
}

// Stretch the signal horizontally and vertically.
// Keep sampling rate the same, keep peak time the same, which means updates:
// begin_time, peak,
EvenSampledSignal EvenSampledSignal::Stretch(const double &h) const{

    if (GetPeak()>=Size())
        throw std::runtime_error("In stretching, peak not defined.");

    EvenSampledSignal ans;
    if (h==1) {
        ans=*this;
        return ans;
    }

    // Stretch the signal.
    ans.amp=::StretchSignal(GetAmp(),h);

    // Set times.
    double OldPeakTime=PeakTime(),OldBeginTime=BeginTime();
    ans.filename=GetFileName();
    ans.amp_multiplier=GetAmpMultiplier();
    ans.delta=GetDelta();
    ans.begin_time=OldPeakTime-(OldPeakTime-OldBeginTime)*h;
    ans.peak=(std::size_t)((OldPeakTime-ans.BeginTime())/ans.GetDelta());
    ans.FindPeakAround(PeakTime(),0.5);

    return ans;
}

// Try to strip the signal s2; shift s2 before the strip if dt is given.
// Will only alter the overlapping part.
// Sampling rate should be the same.
// Difference from operator- : not as strict as operator -, signal length/begin time can be different.
void EvenSampledSignal::StripSignal(const EvenSampledSignal &s2, const double &dt) {
    AddStripSignal(s2,dt,false);
}

// Make a t* operator-convolved waveform.
// Keep sampling rate the same, keep data length the same.
// Notice: peak amplitude and position could be changed.
EvenSampledSignal EvenSampledSignal::Tstar(const double &ts, const double &tol) const{
    EvenSampledSignal ans(*this);
    if (ts<=0) return ans;

    // Create a t* operator.
    auto res=::TstarOperator(ts,GetDelta(),tol);
    EvenSampledSignal Ts(res.first,GetDelta(),-GetDelta()*res.second);         // Ts has peak at ~0.
    Ts.FindPeakAround(0,1);                                                       // Find Ts's peak.
    ans.Convolve(Ts);
    return ans;
}

// Notice: no pre-processing (such as remove trend or taper).
// Changes:
// amp(signal length change),peak(=Size()/2),begin_time(relative to peak time)
void EvenSampledSignal::WaterLevelDecon(const EvenSampledSignal &source, const double &wl) {
    if (fabs(GetDelta()-source.GetDelta())>1e-5)
        throw std::runtime_error("Signal inputs of decon have different sampling rate.");

    amp=::WaterLevelDecon(GetAmp(),GetPeak(),source.GetAmp(),source.GetPeak(),GetDelta(),wl);
    peak=Size()/2;
    begin_time=-GetDelta()*(GetPeak()-1);
}

// Stack two same sampling rate, same begin time signal.
EvenSampledSignal &EvenSampledSignal::operator+=(const EvenSampledSignal &item){

    if (Size()==0) {
        *this=item;
        delta=item.GetDelta();
        filename="--StackResult";
        amp_multiplier=1;
        peak=-1;
        return *this;
    }

    if (fabs(GetDelta()-item.GetDelta())>1e-5)
        throw std::runtime_error("Tried to stack two signals with different "
                                 "sampling rate: "+std::to_string(GetDelta())+" v.s. "
                                +std::to_string(item.GetDelta()));

    if (fabs(BeginTime()-item.BeginTime())>1e-5)
        throw std::runtime_error("Tried to stack two signals with different "
                                 "begin time: "+std::to_string(BeginTime())+" v.s. "
                                +std::to_string(item.BeginTime()));

    if (Size()!=item.Size())
        throw std::runtime_error("Tried to stack two signals with different "
                                 "number of points: "+std::to_string(Size())+" v.s. "
                                +std::to_string(item.Size()));

    for (std::size_t i=0;i<Size();++i) amp[i]+=item.GetAmp()[i];

    return *this;
}

// Subtract two same sampling rate, same begin time signal.
EvenSampledSignal &EvenSampledSignal::operator-=(const EvenSampledSignal &item){

    if (Size()==0) {
        *this=item;
        delta=item.GetDelta();
        filename="--SubtractResult";
        amp_multiplier=1;
        peak=-1;
        *this*=-1;
        return *this;
    }

    if (fabs(GetDelta()-item.GetDelta())>1e-5)
        throw std::runtime_error("Tried to subtract two signals with different "
                                 "sampling rate: "+std::to_string(GetDelta())+" v.s. "
                                +std::to_string(item.GetDelta()));

    if (fabs(BeginTime()-item.BeginTime())>1e-5)
        throw std::runtime_error("Tried to subtract two signals with different "
                                 "begin time: "+std::to_string(BeginTime())+" v.s. "
                                +std::to_string(item.BeginTime()));

    if (Size()!=item.Size())
        throw std::runtime_error("Tried to subtract two signals with different "
                                 "number of points: "+std::to_string(Size())+" v.s. "
                                +std::to_string(item.Size()));

    for (std::size_t i=0;i<Size();++i) amp[i]-=item.GetAmp()[i];

    return *this;
}


/* -----------------------------------------------------------------------------
End of member function/operator definitions. -----------------------------------
----------------------------------------------------------------------------- */

// Put operator define before other non-member function to avoid confusion.
// Overload operator ">>" to read a signal from a two-columned input (stdin/file/etc.)
std::istream &operator>>(std::istream &is, EvenSampledSignal &item){

    item.Clear();
    double x,y,dt=-1,CurEndTime=0;
    while (is >> x >> y) {
        if (item.Size()==0)
            item.begin_time=x;
        else {
            // Check the ascending of Time serise.
            if (x<=CurEndTime) {
                is.setstate(std::ios::failbit);
                return is;
            }

            // Check even sampling.
            double new_dt=x-CurEndTime;
            if (dt<0)
                dt=new_dt;
            else if (new_dt<=dt*0.99 || dt*1.01<=new_dt) {
                is.setstate(std::ios::failbit);
                return is;
            }
            else
                dt=(x-item.begin_time)/item.Size();
        }
        CurEndTime=x;
        item.amp.push_back(y);
    }

    item.delta=(CurEndTime-item.begin_time);
    if (item.Size()>1) item.delta/=(item.Size()-1);

    item.peak=-1;
    item.amp_multiplier=1;

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
        os << item.BeginTime()+item.GetDelta()*i << '\t'
           << item.GetAmp()[i] << (i+1==item.Size()?"":"\n");

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
EvenSampledSignal operator-(const EvenSampledSignal &s1,const EvenSampledSignal &s2){
    EvenSampledSignal ans(s1);
    ans-=s2;
    return ans;
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

// Other non-member functions.

// Compare two signals around their peaks.
// t1 t2 are time window relative to their own peaks (in seconds, default: t1=-5, t2=5)
SignalCompareResults CompareSignal(const EvenSampledSignal &S1, const EvenSampledSignal &S2,
                                   const double &t1=-5, const double &t2=5,
                                   const double &AmpLevel=0.1){
    if (fabs(S1.GetDelta()-S2.GetDelta())>0.01*S1.GetDelta())
        throw std::runtime_error("Comparing two differently sampled signals.");

    if (S1.GetPeak()==(std::size_t)-1 || S2.GetPeak()==(std::size_t)-1)
        throw std::runtime_error("Comparing two signals, but their peaks are not defined.");

    auto s1=S1/fabs(S1.GetAmp()[S1.GetPeak()]);
    auto s2=S2/fabs(S2.GetAmp()[S2.GetPeak()]);
    return ::CompareSignal(s1.GetAmp(),s1.GetPeak(),s2.GetAmp(),s2.GetPeak(),
                           s1.GetDelta(),t1,t2,AmpLevel);
}

std::pair<EvenSampledSignal,EvenSampledSignal>
StackSignals(const std::vector<EvenSampledSignal> &Signals,
             const std::vector<double> &Weights=std::vector<double> ()) {

    // Check size.
    std::size_t m=Signals.size();
    if (m==0) return {};
    if (!Weights.empty() && Weights.size()!=m)
        throw std::runtime_error("Input signals nubmer is different from weight number.");

    // Check signal length, sampling rate and begin time.
    std::size_t n=Signals[0].Size();
    double dt=Signals[0].GetDelta(),bt=Signals[0].BeginTime();
    for (const auto &item:Signals) {
        if (item.Size()!=n)
            throw std::runtime_error("Input signals have different lengths.");
        if (fabs(item.GetDelta()-dt)>1e-10)
            throw std::runtime_error("Input signals have different sampling rates.");
        if (fabs(item.BeginTime()-bt)>1e-10)
            throw std::runtime_error("Input signals have different begin times.");
    }

    // Make stack.
    std::vector<double> V(m,0),S(n,0),STD(n,0);

    for (std::size_t i=0;i<n;++i) {
        for (std::size_t j=0;j<m;++j)
            V[j]=Signals[j].GetAmp()[i];
        auto res=AvrStd(V,Weights);
        S[i]=res.first;
        STD[i]=res.second;
    }
    return {EvenSampledSignal(S,dt,bt),EvenSampledSignal(STD,dt,bt)};
}

EvenSampledSignal IFFT(const EvenSampledSignal &amp,const EvenSampledSignal &phase) {
    auto res=::IFFT(amp.GetAmp(),phase.GetAmp(),amp.GetDelta());
    return EvenSampledSignal(res,1.0/2/amp.GetTime().back());
}

// I guess "this" pointer will have dynamic binding?
// Therefore you need to define another function for EvenSampledSignal to call
// its version of "<<" operator.
void EvenSampledSignal::OutputToFile(const std::string &s) const {
    std::ofstream fpout(s);
    fpout << (*this) << std::endl;
    fpout.close();
}

#endif
