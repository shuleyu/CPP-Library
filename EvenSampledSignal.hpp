#ifndef ASU_EVENSAMPLEDSIGNAL
#define ASU_EVENSAMPLEDSIGNAL

#include<vector>
#include<cmath>
#include<string>
#include<fstream>

#include<Interpolate.hpp>
#include<CreateGrid.hpp>
#include<GaussianBlur.hpp>
#include<Butterworth.hpp>
#include<RemoveTrend.hpp>
#include<WaterLevelDecon.hpp>
#include<DigitalSignal.hpp>

class EvenSampledSignal : public DigitalSignal {

protected:
    double Dt=0,BeginTime=0,EndTime=0;

public:

    // Constructor/Destructors.
    EvenSampledSignal () = default;
    EvenSampledSignal (const std::string &);
    EvenSampledSignal (const DigitalSignal &, const double &);
    EvenSampledSignal (const EvenSampledSignal &, const double &);
    template<class T> EvenSampledSignal (const std::vector<T> &, const double &, const double & =0, const std::string & ="");
    virtual ~EvenSampledSignal () = default; // Base class destructor need to be virtual. (this derived class could be a base for another derived class.)

    // Virtual (override) function declarations.
    virtual double bt() const override {return BeginTime;}
    virtual void clear() override {Amp.clear();}
    virtual double length() const override {return EndTime-BeginTime;}
    virtual double et() const override {return EndTime;}
    virtual double PeakTime() const override {return BeginTime+Peak*Dt;}
    virtual void ShiftTime(const double &t) override {BeginTime+=t;EndTime+=t;}

    virtual bool CheckAndCutToWindow(const double &, const double &, const double &) override;
    virtual void FindPeakAround(const double &, const double & =5) override;
    virtual void HannTaper(const double &) override;
    virtual void PrintInfo() const override;
    virtual std::pair<double,double> RemoveTrend() override;

    // Original (and final) functions.
    double dt() const {return Dt;}
    void Butterworth(const double &, const double &);
    void GaussianBlur(const double & =1);
    void Interpolate(const double &);
    void WaterLevelDecon(const EvenSampledSignal &, const double & =0.1);

    // non-member friends declearation.
    friend std::istream &operator>>(std::istream &, EvenSampledSignal &);
    friend std::ostream &operator<<(std::ostream &, const EvenSampledSignal &);
    friend class SACSignals;
};

// Constructors/Destructors definition.
EvenSampledSignal::EvenSampledSignal (const std::string &s) {
    std::ifstream fpin(s);
    fpin >> *this;
    fpin.close();
    FileName=s;
}

EvenSampledSignal::EvenSampledSignal (const DigitalSignal &item, const double &delta) {

    // Interpolation.
    auto xx=::CreateGrid(item.bt(),item.et(),delta,1);
    Amp=::Interpolate(item.Time,item.Amp,xx);
    Dt=delta;
    BeginTime=item.bt();
    EndTime=item.et();
    FileName=item.FileName;
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
    FileName=item.FileName;
}

template<class T>
EvenSampledSignal::EvenSampledSignal (const std::vector<T> &item, const double &delta, const double &begintime, const std::string &filename) {
    Amp.resize(item.size());
    for (size_t i=0;i<size();++i) Amp[i]=item[i];
    Dt=delta;
    BeginTime=EndTime=begintime;
    if (size()>1) EndTime=BeginTime+Dt*(size()-1);
    FileName=filename;
}

// Member function definitions.

// Cut the data within a window. If cut failed, don't cut and return false.
bool EvenSampledSignal::CheckAndCutToWindow(const double &t, const double &t1, const double &t2){
    // Check window position.
    if (t1>=t2) throw std::runtime_error("Cut window length <=0.");
    if (t+t1<bt() || t+t2>et()) return false;

    // Cut.
    std::size_t d1=ceil((t+t1-bt())/dt()),d2=1+floor((t+t2-bt())/dt());

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

// Print some info.
void EvenSampledSignal::PrintInfo() const {
    DigitalSignal::PrintInfo();
    std::cout << "Sampling rate: " << dt() << '\n';
}

// remove drift and DC.
std::pair<double,double> EvenSampledSignal::RemoveTrend(){
    return ::RemoveTrend(Amp,Dt,BeginTime);
}

// butterworth filter.
// changes: Amp(value change).
void EvenSampledSignal::Butterworth(const double &f1, const double &f2){
    std::vector<std::vector<double>> In{Amp};
    ::Butterworth(In,Dt,f1,f2);
    Amp=In[0];
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

// Changes:
// Amp(signal length change),Peak(=Amp.size()/2),BeginTime(relative to PeakTime=0),EndTime(relative to PeakTime=0)
void EvenSampledSignal::WaterLevelDecon(const EvenSampledSignal &source, const double &wl) {
    if (fabs(Dt-source.Dt)>1e-5)
        throw std::runtime_error("Signal inputs of decon have different sampling rate.");
    std::vector<std::vector<double>> In{Amp};
    std::vector<std::size_t> P{Peak};
    auto ans=::WaterLevelDecon(In,P,source.Amp,source.Peak,Dt,wl);

    Amp=ans[0];
    Peak=size()/2;
    BeginTime=(Peak-1)*Dt*-1;
    EndTime=BeginTime+(size()-1)*Dt;
}

// Non-member functions.

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

#endif
