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
    public:
        DigitalSignal () = default;

        // Construct signal from two-column file.
        DigitalSignal (const std::string &s) {
            std::ifstream fpin(s);
            fpin >> *this;
            fpin.close();
            FileName=s;
        }

        // virtual functions.
        virtual ~DigitalSignal () = default;
        virtual void clear() {Amp.clear();Time.clear();}
        virtual double length() const {return et()-bt();}
        virtual double bt() const {if (Time.empty()) return 0; else return Time[0];}
        virtual double et() const {if (Time.empty()) return 0; else return Time.back();}
        virtual double PeakTime() const {return Time[Peak];}
        virtual void FindPeakAround(const double &,const double & =5);
        virtual void HannTaper(const double &w=0.05);
        virtual std::pair<double,double> RemoveTrend();
        virtual void ShiftTime(const double &t) {
            for (std::size_t i=0;i<size();++i) Time[i]+=t;
            return;
        }
        virtual void PrintInfo() const{
            std::cout << "FileName: " << filename() << '\n';
            std::cout << "Length: " << length() << '\n';
            std::cout << "Signal size: " << size() << '\n';
            std::cout << "BeginTime: " << bt() << '\n';
            std::cout << "EndTime: " << et() << '\n';
            std::cout << "PeakTime: " << PeakTime() << '\n';
            return;
        }

        // Original final functions.
        std::size_t size() const {return Amp.size();}
        std::size_t peak() const {return Peak;}
        std::string filename() const {return FileName;}
        double NormalizeToGlobal() {return ::Normalize(Amp);};
        double NormalizeToPeak() {
            double x=Amp[Peak];
            for (std::size_t i=0;i<size();++i) Amp[i]/=x;
            return x;
        }
        void ShiftTimeRelativeToPeak() {return ShiftTime(-PeakTime());}

    // non-member friends declearation.
    friend std::istream &operator>>(std::istream &, DigitalSignal &);
    friend std::ostream &operator<<(std::ostream &, const DigitalSignal &);
    friend class EvenSampledSignal;
};


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

// Find the position of max|amp| around some time.
void DigitalSignal::FindPeakAround(const double &t, const double &w){
    double AbsMax=-1;
    for (size_t i=0;i<this->size();++i){
        if (fabs(Time[i]-t)<=w && AbsMax<fabs(Amp[i])) {
            AbsMax=fabs(Amp[i]);
            Peak=i;
        }
    }
    return;
}

// cos (-pi,pi) shaped taper at two ends.
void DigitalSignal::HannTaper(const double &w){

    if (w>0.5) throw std::runtime_error("Hanning window width too long.");
    double WL=w*this->length();
    for (size_t i=0;i<this->size();++i){
        double dt=std::min(Time[i]-Time[0],Time.back()-Time[i]);
        if (dt<WL) Amp[i]*=0.5-0.5*cos(dt/WL*M_PI);
    }
    return;
}

// remove drift and DC.
std::pair<double,double> DigitalSignal::RemoveTrend(){

    size_t n=this->size();
    if (n<=1) return {0,0};

    double sumx=0,sumx2=0,sumy=0,sumxy=0,avx;
    for (size_t i=0;i<n;++i){
        sumx+=this->Time[i];
        sumx2+=this->Time[i]*this->Time[i];
        sumy+=this->Amp[i];
        sumxy+=this->Time[i]*this->Amp[i];
    }
    avx=sumx/n;

    // calculate the fitting line's slope and intercept value.
    double slope=(sumxy-sumy*avx)/(sumx2-n*avx*avx);
    double intercept=sumy/n-slope*avx;

    // remove the trend and average for input data points.
    for (size_t i=0;i<n;++i)
        this->Amp[i]-=(intercept+this->Time[i]*slope);

    return {slope,intercept};
}

#endif
