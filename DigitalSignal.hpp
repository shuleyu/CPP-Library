#ifndef ASU_DIGITALSIGNAL
#define ASU_DIGITALSIGNAL

#include<iostream>
#include<fstream>
#include<iomanip>
#include<string>
#include<vector>
#include<algorithm>
#include<cmath>

#include<Normalize.hpp>

class DigitalSignal{

private:
    std::vector<double> Time;
protected:
    std::vector<double> Amp;
    std::size_t Peak=0;
    std::string FileName="";
    double AmpMultiplier=1.0;

public:

    // Constructor/Destructors.
    DigitalSignal () = default;
    DigitalSignal (const std::string &);
    virtual ~DigitalSignal () = default; // Base class destructor need to be virtual.

    // Virtual function declarations.
    virtual double bt() const {if (Time.empty()) return 0; else return Time[0];}
    virtual void clear() {Amp.clear();Time.clear();}
    virtual double et() const {if (Time.empty()) return 0; else return Time.back();}
    virtual double length() const {return et()-bt();}
    virtual double PeakTime() const {return Time[Peak];}

    virtual bool CheckAndCutToWindow(const double &t, const double &t1, const double &t2);
    virtual void FindPeakAround(const double &,const double & =5);
    virtual void HannTaper(const double &);
    virtual void PrintInfo() const;
    virtual std::pair<double,double> RemoveTrend();
    virtual void ShiftTime(const double &);

    // Original (and final) functions.
    std::string filename() const {return FileName;}
    std::size_t peak() const {return Peak;}
    std::size_t size() const {return Amp.size();}
    void NormalizeToSignal() {AmpMultiplier*=::Normalize(Amp);}
    double OriginalAmp() const {return AmpMultiplier;}
    void ShiftTimeRelativeToPeak() {ShiftTime(-PeakTime());}

    void NormalizeToPeak();
    void Scale(const double &);
    void ShiftDC(const double &);

    // non-member friends declearation.
    friend std::istream &operator>>(std::istream &, DigitalSignal &);
    friend std::ostream &operator<<(std::ostream &, const DigitalSignal &);
    friend class EvenSampledSignal;
};

// Constructors/Destructors definition.
DigitalSignal::DigitalSignal (const std::string &s) {
    std::ifstream fpin(s);
    fpin >> *this;
    fpin.close();
    FileName=s;
}


// Member function definitions.

// Cut the data within a window. If cut failed, don't cut and return false.
bool DigitalSignal::CheckAndCutToWindow(const double &t, const double &t1, const double &t2){
    // Check window position.
    if (t1>=t2) throw std::runtime_error("Cut window length <=0.");
    if (t+t1<bt() || t+t2>et()) return false;

    // Cut.
    std::size_t d1=distance(Time.begin(),std::lower_bound(Time.begin(),Time.end(),t+t1));
    std::size_t d2=distance(Time.begin(),std::upper_bound(Time.begin(),Time.end(),t+t2));

    std::vector<double> NewTime(Time.begin()+d1,Time.begin()+d2),NewAmp(Amp.begin()+d1,Amp.begin()+d2);
    std::swap(NewTime,Time);
    std::swap(NewAmp,Amp);
    if (Peak>=d1) Peak-=d1;
    else Peak=0;
    if (Peak>size()) Peak=0;
    return true;
}

// Find the position of max|amp| around some time.
void DigitalSignal::FindPeakAround(const double &t, const double &w){
    double AbsMax=-1;
    for (size_t i=0;i<size();++i){
        if (fabs(Time[i]-t)<=w && AbsMax<fabs(Amp[i])) {
            AbsMax=fabs(Amp[i]);
            Peak=i;
        }
    }
}

// cos (-pi,pi) shaped taper at two ends.
void DigitalSignal::HannTaper(const double &wl){

    if (wl*2>length()) throw std::runtime_error("Hanning window too wide.");
    for (size_t i=0;i<size();++i){
        double dt=std::min(Time[i]-Time[0],Time.back()-Time[i]);
        if (dt<wl) Amp[i]*=0.5-0.5*cos(dt/wl*M_PI);
    }
}

// Print metadata.
void DigitalSignal::PrintInfo() const{
    std::cout << "FileName: " << filename() << '\n';
    std::cout << "Length: " << length() << '\n';
    std::cout << "Signal size: " << size() << '\n';
    std::cout << "BeginTime: " << bt() << '\n';
    std::cout << "EndTime: " << et() << '\n';
    std::cout << "PeakTime: " << PeakTime() << '\n';
}

// remove drift and DC.
std::pair<double,double> DigitalSignal::RemoveTrend(){

    size_t n=size();
    if (n<=1) return {0,0};

    double sumx=0,sumx2=0,sumy=0,sumxy=0,avx;
    for (size_t i=0;i<n;++i){
        sumx+=Time[i];
        sumx2+=Time[i]*Time[i];
        sumy+=Amp[i];
        sumxy+=Time[i]*Amp[i];
    }
    avx=sumx/n;

    // calculate the fitting line's slope and intercept value.
    double slope=(sumxy-sumy*avx)/(sumx2-n*avx*avx);
    double intercept=sumy/n-slope*avx;

    // remove the trend and average for input data points.
    for (size_t i=0;i<n;++i)
        Amp[i]-=(intercept+Time[i]*slope);

    return {slope,intercept};
}

// Positive shift: shift right.
void DigitalSignal::ShiftTime(const double &t) {
    for (std::size_t i=0;i<size();++i)
        Time[i]+=t;
}

// Normalize Amp to the peak ampltude.
void DigitalSignal::NormalizeToPeak() {
    double x=Amp[Peak];
    for (std::size_t i=0;i<size();++i) Amp[i]/=x;
    AmpMultiplier*=x;
}

// Scaling the Amp without changing AmpMultiplier.
void DigitalSignal::Scale(const double &s){
    for (std::size_t i=0;i<size();++i) Amp[i]*=s;
}

// Add an DC to the signal. The meaning of AmpMultiplier becomes ambiguous.
void DigitalSignal::ShiftDC(const double &s){
    for (std::size_t i=0;i<size();++i) Amp[i]+=s;
}

// Non-member functions.

// Overload operator ">>" to read a signal from a two-columned input (stdin/file/etc.)
std::istream &operator>>(std::istream &is, DigitalSignal &item){

    item.clear();
    double x,y;
    // Choose to read in this way to keep Time.size()==Amp.size()
    while (is >> x >> y){

        // Check the ascending of Time serise.
        if (!item.Time.empty() && x<=item.Time.back())
            throw std::runtime_error("Time serise is either not increasing or has repeating values.");

        item.Time.push_back(x);
        item.Amp.push_back(y);
    }
    return is;
}

// Overload operator "<<" to print a signal in a two-columned format.
std::ostream &operator<<(std::ostream &os, const DigitalSignal &item){

//     // record original format.
//     std::ios state(NULL);
//     state.copyfmt(os);
//
//     os << std::fixed << std::scientific << std::setprecision();
    for (std::size_t i=0;i<item.size();++i)
        os << item.Time[i] << '\t' << item.Amp[i] << '\n';

//     // restore print format.
//     os.copyfmt(state);
    return os;
}

#endif
