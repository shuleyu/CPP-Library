#ifndef ASU_DIGITALSIGNAL
#define ASU_DIGITALSIGNAL

#include<iostream>
#include<fstream>
#include<iomanip>
#include<string>
#include<vector>
#include<algorithm>
#include<cmath>

class DigitalSignal{
    private:
        std::vector<double> Time;
    protected:
        std::vector<double> Amp;
        std::size_t Peak=0;
    public:
        DigitalSignal () = default;
        DigitalSignal (const std::string &s) {
            std::ifstream fpin(s);
            fpin >> *this;
            fpin.close();
        }

        virtual ~DigitalSignal () = default;

        virtual void clear() {Time.clear();Amp.clear();}
        virtual double length() const {return et()-bt();}
        virtual double bt() const {if (Time.empty()) return 0; else return Time[0];}
        virtual double et() const {if (Time.empty()) return 0; else return Time.back();}
        virtual double PeakTime() const {return Time[Peak];}
        virtual void FindPeakAround(const double &,const double & =5);
//         virtual void HannTaper(const double &w);

        std::size_t size() const {return Amp.size();}
        std::size_t peak() const {return Peak;}



    // non-member friends declearation.
    friend std::istream &operator>>(std::istream &, DigitalSignal &);
    friend std::ostream &operator<<(std::ostream &, const DigitalSignal &);
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
std::cout << "This is base." << std::endl;
    for (size_t i=0;i<Time.size();++i){
        if (fabs(Time[i]-t)<=w && AbsMax<fabs(Amp[i])) {
            AbsMax=fabs(Amp[i]);
            Peak=i;
        }
    }
    return;
}
#endif
