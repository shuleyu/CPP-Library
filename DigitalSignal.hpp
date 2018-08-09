#ifndef ASU_DIGITALSIGNAL
#define ASU_DIGITALSIGNAL

#include<iostream>
#include<fstream>
#include<iomanip>
#include<string>
#include<vector>
#include<algorithm>
#include<cmath>

#include<Amplitude.hpp>
#include<SortWithIndex.hpp>
#include<ReorderUseIndex.hpp>

class DigitalSignal{

private:   // private part never get inherited.

    std::vector<double> time;

protected: // inherit mode is "private"               --> "private".
           // inherit mode is "protected" or "public" --> "protected".

    std::vector<double> amp;
    std::size_t peak;
    std::string filename;
    double amp_multiplier;

public:    // inherit mode is "private"   --> "private".
           // inherit mode is "protected" --> "protected".
           // inherit mode is "public"    --> "public".

    // Constructor/Destructors.
    DigitalSignal ();
    DigitalSignal (const DigitalSignal &item) = default;
    DigitalSignal (const std::string &infile);                         // Read from a 2-column file.
    DigitalSignal (const std::vector<double> &ti, const std::vector<double> &am);   // By 2 vectors.
    virtual ~DigitalSignal () = default;                // Base class destructor need to be virtual.

    // Declaration of virtual functions/operators. They will be overwritten in drived class.
    // If function/operator is also defined here, they will be "in-line".
    // Use interface as much as possible to avoid nasty updates in the future.

    virtual double BeginTime() const {return (GetTime().empty()?0.0/0.0:GetTime()[0]);}
    virtual void Clear() {*this=DigitalSignal ();}
    virtual double EndTime() const {return (GetTime().empty()?0.0/0.0:GetTime().back());}
    virtual const std::vector<double> &GetTime() const {return time;}
    virtual double PeakTime() const {return (GetTime().empty()?0.0/0.0:GetTime()[GetPeak()]);}
    virtual double SignalDuration() const {return EndTime()-BeginTime();}

    virtual bool CheckAndCutToWindow(const double &t1, const double &t2);          // t1, t2 in sec.
    virtual void FindPeakAround(const double &t,const double &w=5);                // t, w   in sec.
    virtual void HannTaper(const double &wl);                                      // wl is  in sec.
    virtual std::size_t LocateTime(const double &t) const;                         // t      in sec.
    virtual void PrintInfo() const;
    virtual std::pair<double,double> RemoveTrend();
    virtual void ShiftTime(const double &t){                   // t in sec. t>0: shift to the right.
        for (std::size_t i=0;i<Size();++i)
            time[i]+=t;
    }
    virtual void ZeroOutHannTaper(const double &wl, const double &zl);             // wl, zl in sec.


    // Declaration of first appear and final functions/operators.
    // They are not virtual to increase performance.
    // Therefore in drived class, care is needed not to define these function again.
    // Also, because they are not virtual with final flag,
    // you need to guarantee they behaves well for all derived classes.
    // Because they are intended unchangeable, only protected and public memebers can appear here(?)

    const std::vector<double> &GetAmp() const {return amp;}
    double GetAmpMultiplier() const {return amp_multiplier;}
    const std::string &GetFileName() const {return filename;}
    std::size_t GetPeak() const {return peak;}

    bool CheckWindow(const double &t1, const double &t2) const {     // t1, t2 in sec.
        if (t1>=t2) throw std::runtime_error("Window length <=0.");
        if (t1<BeginTime() || t2>EndTime()) return false;
        else return true;
    }
    double MaxAmp() const {
        return fabs(*std::max_element(GetAmp().begin(),GetAmp().end(),
            [](const double &a, const double &b){
                return a<fabs(b);
            }));
    }
    double MaxVal() const {return *std::max_element(GetAmp().begin(),GetAmp().end());}
    double MinVal() const {return *std::min_element(GetAmp().begin(),GetAmp().end());}
    void NormalizeToPeak() {*this/=fabs(GetAmp()[GetPeak()]);}
    void NormalizeToSignal() {*this/=MaxAmp();}
    void SetBeginTime(const double &t) {ShiftTime(-BeginTime()+t);}
    void ShiftTimeReferenceToPeak() {ShiftTime(-PeakTime());}
    std::size_t Size() const {return GetAmp().size();}

    DigitalSignal &operator+=(const double &a){
        for (std::size_t i=0;i<Size();++i) amp[i]+=a;
        return *this;
    }
    DigitalSignal &operator*=(const double &a){
        for (std::size_t i=0;i<Size();++i) amp[i]*=a;
        if (a!=0) amp_multiplier/=a;
        else amp_multiplier=1.0/0.0;
        return *this;
    }
    DigitalSignal &operator-=(const double &a){*this+=(-a);return *this;}
    DigitalSignal &operator/=(const double &a){
        if (a==0) throw std::runtime_error("Dividing amplitudes with zero.");
        *this*=(1.0/a);
        return *this;
    }


    // declaration of non-member class/function/operators as friend.
    // Input operator >> need access to the private/protected parts of this class,
    // therefore it needs to be friend.
    friend std::istream &operator>>(std::istream &is, DigitalSignal &item);

}; // End of class declaration.

// Constructors/Destructors definition.
DigitalSignal::DigitalSignal () {
    peak=-1;
    amp_multiplier=1;
    filename="";
}

DigitalSignal::DigitalSignal (const std::string &infile) {
    std::ifstream fpin(infile);
    fpin >> *this;
    fpin.close();
    peak=-1;
    amp_multiplier=1;
    filename=infile;
}

DigitalSignal::DigitalSignal (const std::vector<double> &ti, const std::vector<double> &am) {
    time=ti;
    amp=am;
    peak=-1;
    amp_multiplier=1;
    filename="";
}


// Member function/operators definitions.

// Cut the data within a window and return true.
// If cut failed, do nothing and return false.
bool DigitalSignal::CheckAndCutToWindow(const double &t1, const double &t2){

    if (!CheckWindow(t1,t2)) return false;

    // Cut.
    std::size_t d1=LocateTime(t1),d2=LocateTime(t2);
    if (d2!=Size()) ++d2;

    std::vector<double> time2(GetTime().begin()+d1,GetTime().begin()+d2);
    std::vector<double> amp2(GetAmp().begin()+d1,GetAmp().begin()+d2);
    std::swap(time,time2);
    std::swap(amp,amp2);

    if (GetPeak()!=(size_t)(-1) && GetPeak()>=d1) peak-=d1;
    else peak=-1;

    return true;
}

// Find the position of max|amp| around given time.
void DigitalSignal::FindPeakAround(const double &t, const double &w){

    if (w<0) return;
    if (t+w<BeginTime() || t-w>EndTime()) return;

    size_t X=0,Y=Size()-1;
    if (t-w>=BeginTime()) X=LocateTime(t-w);
    if (t+w<=EndTime()) Y=LocateTime(t+w);
    if (Y!=Size()) ++Y;

    auto it=std::max_element(GetAmp().begin()+X,GetAmp().begin()+Y,
        [](const double &a, const double &b){
            return a<fabs(b);
        }
    );
    peak=X+std::distance(GetAmp().begin(),it);
}

// cos (-pi,pi) shaped taper at two ends.
void DigitalSignal::HannTaper(const double &wl){
    if (wl*2>SignalDuration())
        throw std::runtime_error("Hanning window too wide.");
    for (std::size_t i=0;i<Size();++i){
        double len=std::min(GetTime()[i]-GetTime()[0],GetTime().back()-GetTime()[i]);
        if (len<wl) amp[i]*=0.5-0.5*cos(len/wl*M_PI);
    }
}

std::size_t DigitalSignal::LocateTime(const double &t) const {
    if (t<BeginTime() || t>EndTime()) {
        std::cerr <<  "Warning in " << __func__
                  << ": request time location outside of the signal ..." << std::endl;
        if (t<BeginTime()) return 0;
        else return Size()-1;
    }
    auto it=std::lower_bound(GetTime().begin(),GetTime().end(),t);
    if (it==GetTime().begin()) return 0;
    if (fabs(*std::prev(it)-t)<fabs(*it-t)) return std::distance(GetTime().begin(),std::prev(it));
    else return std::distance(GetTime().begin(),it);
}

// Print metadata.
void DigitalSignal::PrintInfo() const{
    std::cout << "File name  : " << GetFileName() << '\n';
    std::cout << "Duration   : " << SignalDuration() << '\n';
    std::cout << "NPTS       : " << Size() << '\n';
    std::cout << "BeginTime  : " << BeginTime() << '\n';
    std::cout << "EndTime    : " << EndTime() << '\n';
    std::cout << "PeakTime   : " << PeakTime() << '\n';
    std::cout << "MaxVal     : " << MaxVal() << '\n';
    std::cout << "MinVal     : " << MinVal() << '\n';
    std::cout << "MaxAmp     : " << MaxAmp() << '\n';
}

// remove drift and DC.
std::pair<double,double> DigitalSignal::RemoveTrend(){

    if (Size()<=1) return {0,0};

    double sumx=0,sumx2=0,sumy=0,sumxy=0,avx;
    for (std::size_t i=0;i<Size();++i){
        sumx+=GetTime()[i];
        sumx2+=GetTime()[i]*GetTime()[i];
        sumy+=GetAmp()[i];
        sumxy+=GetTime()[i]*GetAmp()[i];
    }
    avx=sumx/Size();

    // calculate the fitting line's slope and intercept value.
    double slope=(sumxy-sumy*avx)/(sumx2-Size()*avx*avx);
    double intercept=sumy/Size()-slope*avx;

    // remove the trend and average for input data points.
    for (std::size_t i=0;i<Size();++i)
        amp[i]-=(intercept+GetTime()[i]*slope);

    return {slope,intercept};
}

// taper window half-length is wl, zero half-length is zl.
void DigitalSignal::ZeroOutHannTaper(const double &wl, const double &zl){
    if ((wl+zl)*2>SignalDuration()) throw std::runtime_error("ZeroOutHanning window too wide.");
    for (std::size_t i=0;i<Size();++i){
        double len=std::min(GetTime()[i]-GetTime()[0],GetTime().back()-GetTime()[i]);
        if (len<zl) amp[i]=0;
        else if (len<zl+wl) amp[i]*=0.5-0.5*cos((len-zl)/wl*M_PI);
    }
}


/* -----------------------------------------------------------------------------
End of member function/operator definitions. -----------------------------------
----------------------------------------------------------------------------- */

// Non-member functions/operators.

// Overload operator ">>" to read a signal from a two-columned input (stdin/file/etc.)
std::istream &operator>>(std::istream &is, DigitalSignal &item){

    item.Clear();
    double x,y;

    while (is >> x >> y){                                      // to ensure amp.size()==time.size().
        item.time.push_back(x);
        item.amp.push_back(y);
    }

    // Sort the time into ascending order.

    if (!std::is_sorted(item.GetTime().begin(),item.GetTime().end())) {    // if not weak ascending.
        auto res=::SortWithIndex(item.time.begin(),item.time.end());
        ::ReorderUseIndex(item.amp.begin(),item.amp.end(),res);
    }

    auto cmp=[](const double &a, const double &b){return a<=b;};     // strict ascending comparator.
    if (!std::is_sorted(item.GetTime().begin(),item.GetTime().end(),cmp)) {        // if not strict.
        // choose to change the time values at the repeated positions.
        size_t i=0;
        while (i+1<item.Size()){
            size_t j=i+1;
            while (j<item.Size() && item.GetTime()[j]==item.GetTime()[i]) ++j;
            if (j!=i+1) {
                if (i==0 && j==item.Size())
                    throw std::runtime_error("Reading from stream created a disaster.");
                double bt=(i==0?item.GetTime()[i]:item.GetTime()[i-1]);
                double et=(j==item.Size()?item.GetTime()[i]:item.GetTime()[j]);
                double dt=(et-bt)/(j-i+((i!=0 && j!=item.Size())?1:0));
                for (size_t k=(i==0?i+1:i);k<j;++k) item.time[k]=item.GetTime()[k-1]+dt;
            }
            i=j;
        }
    }
    return is;
}

// Overload operator "<<" to print a signal in a two-columned format.
// Customize this section code to define other printing format.
std::ostream &operator<<(std::ostream &os, const DigitalSignal &item){

    /*

    // record original format.
    std::ios state(NULL);
    state.copyfmt(os);

    // set new printing format.
    os << std::fixed << std::scientific << std::setprecision();

                                    */

    for (std::size_t i=0;i<item.Size();++i)
        os << item.GetTime()[i] << '\t' << item.GetAmp()[i] << (i+1==item.Size()?"":"\n");

    /*

    // restore previous format.
    os.copyfmt(state);

                                    */
    return os;
}

// Overload operator "+,-" to ShiftDC.
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
