#ifndef ASU_EVENSAMPLEDSIGNAL
#define ASU_EVENSAMPLEDSIGNAL

#include<DigitalSignal.hpp>
#include<cmath>

class EvenSampledSignal : public DigitalSignal {
    protected:
        double Dt=0,BeginTime=0;
    public:
        EvenSampledSignal () = default;
        EvenSampledSignal (const std::string &s) {
            std::ifstream fpin(s);
            fpin >> *this;
            fpin.close();
        }

        virtual ~EvenSampledSignal () = default;

        virtual void clear() override {Amp.clear();}
        virtual double length() const override {return Dt*(Amp.size()-1);}
        virtual double bt() const override {return BeginTime;}


        double dt() const {return Dt;};

    // non-member friends declearation.
    friend std::istream &operator>>(std::istream &, EvenSampledSignal &);
    friend std::ostream &operator<<(std::ostream &, const EvenSampledSignal &);
};

// Overload operator ">>" to read a signal from a two-columned input (stdin/file/etc.)
std::istream &operator>>(std::istream &is, EvenSampledSignal &item){

    item.clear();
    double x,y,x2,dt=-1;
    while (is >> x >> y) {
        if (item.Amp.empty()) item.BeginTime=x;
        else {
            // Check the ascending of Time serise.
            if (x<=x2)
                throw std::runtime_error("Time serise is either not increasing or has repeating values.");

            // Check even sampling.
            if (dt<0) dt=x-x2;
            else if (x-x2<=dt*0.99 || dt*1.01<=x-x2)
                throw std::runtime_error("Input seems not even sampled.");
            else dt=(x-item.BeginTime)/item.Amp.size();
        }
        x2=x;

        item.Amp.push_back(y);
    }
    item.Dt=(x2-item.BeginTime);
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
    for (size_t i=0;i<item.size();++i)
        os << item.BeginTime+item.Dt*i << '\t' << item.Amp[i] << '\n';

//     // restore print format.
//     os.copyfmt(state);
    return os;
}

#endif
