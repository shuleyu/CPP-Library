#ifndef ASU_EVENSAMPLEDSIGNAL
#define ASU_EVENSAMPLEDSIGNAL

#include<cmath>

#include<WaterLevelDecon.hpp>
#include<DigitalSignal.hpp>

class EvenSampledSignal : public DigitalSignal {
    protected:
        double Dt=0,BeginTime=0,EndTime=0;
    public:
        EvenSampledSignal () = default;
        EvenSampledSignal (const std::string &s) {
            std::ifstream fpin(s);
            fpin >> *this;
            fpin.close();
        }

        virtual ~EvenSampledSignal () = default;

        virtual void clear() override {Amp.clear();}
        virtual double length() const override {return EndTime-BeginTime;}
        virtual double bt() const override {return BeginTime;}
        virtual double et() const override {return EndTime;}
        virtual double PeakTime() const override {return BeginTime+Peak*Dt;}

        double dt() const {return Dt;};
        void WaterLevelDecon(const EvenSampledSignal &source, const double &);

        // Find the position of max|amp| around some time.
        template<class T1, class T2=double>
        void FindPeakAround(const T1 &t, const T2 &w=5){
            std::size_t WB=std::max(0.0,(t-w-BeginTime)/Dt),WE=std::min(Amp.size()*1.0,ceil((t+w-BeginTime+Dt/2)/Dt));

            double AbsMax=-1;
            for (std::size_t i=WB;i<WE;++i){
                if (AbsMax<fabs(Amp[i])) {
                    AbsMax=fabs(Amp[i]);
                    Peak=i;
                }
            }
            return;
        }


    // non-member friends declearation.
    friend std::istream &operator>>(std::istream &, EvenSampledSignal &);
    friend std::ostream &operator<<(std::ostream &, const EvenSampledSignal &);
};

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

// Changes:
// Amp(signal length change),Peak(=Amp.size()/2),BeginTime(relative to PeakTime=0),EndTime(relative to PeakTime=0)
void EvenSampledSignal::WaterLevelDecon(const EvenSampledSignal &source, const double &wl=0.1) {
    if (fabs(this->Dt-source.Dt)>1e-5)
        throw std::runtime_error("Signal inputs of decon have different sampling rate.");
    std::vector<std::vector<double>> In{this->Amp};
    std::vector<std::size_t> P{this->Peak};
    auto ans=::WaterLevelDecon(In,P,source.Amp,source.Peak,this->Dt,wl);

    this->Amp=ans[0];
    this->Peak=ans[0].size()/2;
    this->BeginTime=(this->Peak-1)*this->Dt*-1;
    this->FindPeakAround(0);
    this->BeginTime=(this->Peak-1)*this->Dt*-1;
    this->EndTime=this->BeginTime+(ans[0].size()-1)*this->Dt;

    return;
}

#endif
