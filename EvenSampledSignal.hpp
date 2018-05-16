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
        EvenSampledSignal () = default;

        // Construct signal from two-column file.
        EvenSampledSignal (const std::string &s) {
            std::ifstream fpin(s);
            fpin >> *this;
            fpin.close();
            this->FileName=s;
        }

        // Construct signal from a common signal, interpolate to target sampling rate.
        EvenSampledSignal (const DigitalSignal &item, const double &delta) {
            // check DigitalSignal even sampling.
            auto xx=CreateGrid(item.bt(),item.et(),delta,1);
            // Interpolation.
            this->Amp=Interpolate(item.Time,item.Amp,xx);
            this->Dt=delta;
            this->BeginTime=xx[0];
            this->EndTime=xx.back();
            this->FileName=item.FileName;
        }

        // Construct signal from another even-sampled signal, interpolate to target sampling rate.
        EvenSampledSignal (const EvenSampledSignal &item, const double &delta) {

            auto x=CreateGrid(item.bt(),item.et(),item.size(),0);
            auto xx=CreateGrid(item.bt(),item.et(),delta,1);
            // Interpolation.
            this->Amp=Interpolate(x,item.Amp,xx);
            this->Dt=delta;
            this->BeginTime=xx[0];
            this->EndTime=xx.back();
            this->FileName=item.FileName;
        }

        // Construct signal from a vector.
        template<class T>
        EvenSampledSignal (const std::vector<T> &item, const double &delta, const double &begintime=0, const std::string &filename="") {
            this->Amp.resize(item.size());
            for (size_t i=0;i<this->size();++i) this->Amp[i]=item[i];
            this->Dt=delta;
            this->BeginTime=this->EndTime=begintime;
            if (this->size()>1) this->EndTime=this->BeginTime+this->Dt*(this->size()-1);
            this->FileName=filename;
        }

        // virtual functions.
        virtual ~EvenSampledSignal () = default;

        // overrided and virtual functions.
        virtual void clear() override {Amp.clear();}

        // overrided but final functions.
        virtual double length() const override final {return EndTime-BeginTime;}
        virtual double bt() const override final {return BeginTime;}
        virtual double et() const override final {return EndTime;}
        virtual double PeakTime() const override final {return BeginTime+Peak*Dt;}
        virtual void FindPeakAround(const double &,const double & =5) override final;
        virtual void HannTaper(const double &w=0.05) override final;
        virtual std::pair<double,double> RemoveTrend() override final;
        virtual void ShiftTime(const double &t) override final {
            BeginTime+=t;
            EndTime+=t;
            return;
        }
        virtual void PrintInfo() const override final{
            this->DigitalSignal::PrintInfo();
            std::cout << "Sampling rate: " << dt() << '\n';
            return;
        }

        // Original final functions.
        double dt() const {return Dt;};
        void WaterLevelDecon(const EvenSampledSignal &, const double & =0.1);
        void GaussianBlur(const double & =1);
        void Butterworth(const double &, const double &);

    // non-member friends declearation.
    friend std::istream &operator>>(std::istream &, EvenSampledSignal &);
    friend std::ostream &operator<<(std::ostream &, const EvenSampledSignal &);
    friend class SACSignals;
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


// Find the position of max|amp| around some time.
void EvenSampledSignal::FindPeakAround(const double &t, const double &w){
    size_t n=this->size();
    std::size_t WB=std::max(0.0,(t-w-BeginTime)/Dt),WE=std::min(n*1.0,ceil((t+w-BeginTime+Dt/2)/Dt));

    double AbsMax=-1;
    for (std::size_t i=WB;i<WE;++i){
        if (AbsMax<fabs(Amp[i])) {
            AbsMax=fabs(Amp[i]);
            Peak=i;
        }
    }
    return;
}

// cos (-pi,pi) shaped taper at two ends.
void EvenSampledSignal::HannTaper(const double &w){

    size_t n=this->size();
    if (w>0.5) throw std::runtime_error("Hanning window width too long.");
    double WL=w*this->length();
    for (size_t i=0;i<n;++i){
        double dt=std::min(i,n-i-1)*this->Dt;
        if (dt<WL) Amp[i]*=0.5-0.5*cos(dt/WL*M_PI);
    }
    return;
}

// remove drift and DC.
std::pair<double,double> EvenSampledSignal::RemoveTrend(){
    return ::RemoveTrend(this->Amp,this->Dt,this->BeginTime);
}

// gaussian blur.
// changes: Amp(value change).
void EvenSampledSignal::GaussianBlur(const double &sigma){
    std::vector<std::vector<double>> In{this->Amp};
    ::GaussianBlur(In,this->Dt,sigma);
    this->Amp=In[0];
    return;
}

// butterworth filter.
// changes: Amp(value change).
void EvenSampledSignal::Butterworth(const double &f1, const double &f2){
    std::vector<std::vector<double>> In{this->Amp};
    ::Butterworth(In,this->Dt,f1,f2);
    this->Amp=In[0];
    return;
}

// Changes:
// Amp(signal length change),Peak(=Amp.size()/2),BeginTime(relative to PeakTime=0),EndTime(relative to PeakTime=0)
void EvenSampledSignal::WaterLevelDecon(const EvenSampledSignal &source, const double &wl) {
    if (fabs(this->Dt-source.Dt)>1e-5)
        throw std::runtime_error("Signal inputs of decon have different sampling rate.");
    std::vector<std::vector<double>> In{this->Amp};
    std::vector<std::size_t> P{this->Peak};
    auto ans=::WaterLevelDecon(In,P,source.Amp,source.Peak,this->Dt,wl);

    this->Amp=ans[0];
    this->Peak=this->size()/2;
    this->BeginTime=(this->Peak-1)*this->Dt*-1;
    this->EndTime=this->BeginTime+(this->size()-1)*this->Dt;

    return;
}

#endif
