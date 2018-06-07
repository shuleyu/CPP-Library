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
    DigitalSignal (const DigitalSignal &) = default;
    DigitalSignal (const std::string &infile);
    virtual ~DigitalSignal () = default; // Base class destructor need to be virtual.

    // Virtual function declarations.
    virtual double bt() const {if (Time.empty()) return 0; else return Time[0];}
    virtual void clear() {*this=DigitalSignal ();}
    virtual double et() const {if (Time.empty()) return 0; else return Time.back();}
    virtual double pt() const {return Time[Peak];}
    virtual double length() const {return et()-bt();}

    virtual bool CheckAndCutToWindow(const double &t1, const double &t2);
    virtual void FindPeakAround(const double &t,const double &w=5);
    virtual void HannTaper(const double &wl);
    virtual std::size_t LocateTime(const double &t) const;
    virtual void PrintInfo() const;
    virtual std::pair<double,double> RemoveTrend();
    virtual void ShiftTime(const double &t);
    virtual void ZeroOutHannTaper(const double &wl, const double &zl);

    // member operators declearation.
    virtual DigitalSignal &operator+=(const double &a);
    virtual DigitalSignal &operator*=(const double &a);
    virtual DigitalSignal &operator-=(const double &a);
    virtual DigitalSignal &operator/=(const double &a);


    // Original (and final) functions.
    std::string filename() const {return FileName;}
    std::size_t peak() const {return Peak;}
    std::size_t size() const {return Amp.size();}
    bool CheckWindow(const double &t1, const double &t2) const {
        if (t1>=t2) throw std::runtime_error("Window length <=0.");
        if (t1<bt() || t2>et()) return false;
        else return true;
    }
    void NormalizeToSignal() {AmpMultiplier*=::Normalize(Amp);}
    double OriginalAmp() const {return AmpMultiplier;}
    void SetBeginTime(const double &t) {ShiftTime(-bt()+t);}
    void ShiftTimeRelativeToPeak() {ShiftTime(-pt());}
    void NormalizeToPeak();

    // non-member friends/operators declearation.
    friend class EvenSampledSignal;
    friend std::istream &operator>>(std::istream &is, DigitalSignal &item);
    friend std::ostream &operator<<(std::ostream &os, const DigitalSignal &item);
    friend DigitalSignal operator+(const DigitalSignal &item,const double &a);
    friend DigitalSignal operator+(const double &a,const DigitalSignal &item);
    friend DigitalSignal operator-(const DigitalSignal &item,const double &a);
    friend DigitalSignal operator*(const DigitalSignal &item,const double &a);
    friend DigitalSignal operator*(const double &a,const DigitalSignal &item);
    friend DigitalSignal operator/(const DigitalSignal &item,const double &a);
};

// Constructors/Destructors definition.
DigitalSignal::DigitalSignal (const std::string &infile) {
    std::ifstream fpin(infile);
    fpin >> *this;
    fpin.close();
    FileName=infile;
}


// Member function/operators definitions.

// Cut the data within a window. If cut failed, don't cut and return false.
bool DigitalSignal::CheckAndCutToWindow(const double &t1, const double &t2){

    if (!CheckWindow(t1,t2)) return false;

    // Cut.
    std::size_t d1=distance(Time.begin(),std::lower_bound(Time.begin(),Time.end(),t1));
    std::size_t d2=distance(Time.begin(),std::upper_bound(Time.begin(),Time.end(),t2));

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
    for (std::size_t i=0;i<size();++i){
        if (fabs(Time[i]-t)<=w && AbsMax<fabs(Amp[i])) {
            AbsMax=fabs(Amp[i]);
            Peak=i;
        }
    }
}

// cos (-pi,pi) shaped taper at two ends.
void DigitalSignal::HannTaper(const double &wl){

    if (wl*2>length()) throw std::runtime_error("Hanning window too wide.");
    for (std::size_t i=0;i<size();++i){
        double dt=std::min(Time[i]-Time[0],Time.back()-Time[i]);
        if (dt<wl) Amp[i]*=0.5-0.5*cos(dt/wl*M_PI);
    }
}

std::size_t DigitalSignal::LocateTime(const double &t) const {
    if (t<bt() || t>et()) return -1;
    auto it=std::lower_bound(Time.begin(),Time.end(),t);
    if (it==Time.begin()) return 0;
    if (fabs(*std::prev(it)-t)<fabs(*it-t)) return distance(Time.begin(),std::prev(it));
    else return distance(Time.begin(),it);
}

// Print metadata.
void DigitalSignal::PrintInfo() const{
    std::cout << "FileName: " << filename() << '\n';
    std::cout << "Length: " << length() << '\n';
    std::cout << "Signal size: " << size() << '\n';
    std::cout << "BeginTime: " << bt() << '\n';
    std::cout << "EndTime: " << et() << '\n';
    std::cout << "PeakTime: " << pt() << '\n';
}

// remove drift and DC.
std::pair<double,double> DigitalSignal::RemoveTrend(){

    std::size_t n=size();
    if (n<=1) return {0,0};

    double sumx=0,sumx2=0,sumy=0,sumxy=0,avx;
    for (std::size_t i=0;i<n;++i){
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
    for (std::size_t i=0;i<n;++i)
        Amp[i]-=(intercept+Time[i]*slope);

    return {slope,intercept};
}

// Positive shift: shift right.
void DigitalSignal::ShiftTime(const double &t) {
    for (std::size_t i=0;i<size();++i)
        Time[i]+=t;
}

// taper window half-length is wl, zero half-length is zl.
void DigitalSignal::ZeroOutHannTaper(const double &wl, const double &zl){
    if ((wl+zl)*2>length()) throw std::runtime_error("ZeroOutHanning window too wide.");
    for (std::size_t i=0;i<size();++i){
        double dt=std::min(Time[i]-Time[0],Time.back()-Time[i]);
        if (dt<zl) Amp[i]=0;
        else if (dt<zl+wl) Amp[i]*=0.5-0.5*cos((dt-zl)/wl*M_PI);
    }
}

// virtual member operators definition.
DigitalSignal &DigitalSignal::operator+=(const double &a){
    for (std::size_t i=0;i<size();++i) Amp[i]+=a;
    return *this;
}
DigitalSignal &DigitalSignal::operator*=(const double &a){
    for (std::size_t i=0;i<size();++i) Amp[i]*=a;
    return *this;
}
DigitalSignal &DigitalSignal::operator-=(const double &a){
    *this+=(-a);
    return *this;
}
DigitalSignal &DigitalSignal::operator/=(const double &a){
    if (a==0) throw std::runtime_error("Dividing amplitude with zero.");
    *this*=(1.0/a);
    return *this;
}

// Normalize Amp to the peak ampltude.
void DigitalSignal::NormalizeToPeak() {
    double x=Amp[Peak];
    for (std::size_t i=0;i<size();++i) Amp[i]/=x;
    AmpMultiplier*=x;
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

// Overload operator "+,-" to ShiftDC.
// Not changing AmpMultiplier.
DigitalSignal operator+(const DigitalSignal &item,const double &a){
    DigitalSignal ans(item);
    ans+=a;
    return ans;
}
DigitalSignal operator+(const double &a,const DigitalSignal &item){
    return item+a;
}
DigitalSignal operator-(const DigitalSignal &item,const double &a){
    return item+(-a);
}

// Overload operator "*,/" to Scale.
// Not changing AmpMultiplier.
DigitalSignal operator*(const DigitalSignal &item,const double &a){
    DigitalSignal ans(item);
    ans*=a;
    return ans;
}
DigitalSignal operator*(const double &a,const DigitalSignal &item){
    return item*a;
}
DigitalSignal operator/(const DigitalSignal &item,const double &a){
    DigitalSignal ans(item);
    ans/=a;
    return ans;
}

#endif
