#ifndef ASU_SEISSIGNAL
#define ASU_SEISSIGNAL

namespace ASUSEIS{

    class Signal{
        private:
            std::vector<double> data;
            double dt,BeginTime;
        public:
            Signal(const std::vector<double> &D, const double &T, const double &BT) : data(D),dt(T),BeginTime(BT) {}
            std::vector<double> GenerateTime();
            size_t FindTime(const double &Time);
    };

    std::vector<double> Signal::GenerateTime(){
        std::vector<double> ans(data.size(),BeginTime);
        for (size_t i=1;i<data.size();++i)
            ans[i]=ans[i-1]+dt;
        return ans;
    }

    size_t Signal::FindTime(const double &Time){
        if (Time<BeginTime) return -1;
        size_t ans=(Time-BeginTime+dt/2)/dt;
        if (ans>=data.size()) return -1;
        return ans;
    }
}
#include<iostream>
#include<algorithm>
#include<cmath>
#include<string>
#include<cstring>
#include<vector>
extern "C"{
#include<sacio.h>
#include<sac.h>
#include<ASU_tools.h>
}
#include<ASU_tools.hpp>

#define DMAXL 1000000

class SeisSignal{

    static float RAW[DMAXL];

    friend bool operator>>(const std::string &, SeisSignal&);

    public:
    std::string filename="";
    std::vector<double> data;
    std::vector<double> time;
    int NPTS=0;
    double Delta=0;
    double Amplitude=0;
    double GCARC=0;

    void normalize();
    void clean();
    void interpolate(double);
    bool cut(double,double);
    bool bwfilter(double,double,int);
};

bool operator>>(const std::string & in, SeisSignal& s){

    // Read in sac file.
    s.filename=in;

    float rawdata[1500000];
    int maxnpts=1500000;
    int nerr,rawnpts;
    float rawdelta,begt,tmpvar;
    char filename[200];
    char header[20];
    strcpy(filename,s.filename.c_str());

    rsac1(filename,rawdata,&rawnpts,&begt,&rawdelta,&maxnpts,&nerr,strlen(filename));
    if (nerr>0 || rawnpts<=0){
        std::cerr <<  "Warning in " << __func__ << ": Error Reading "<< s.filename << " ..." << std::endl;
        return false;
    }

    strcpy(header,"GCARC");
    getfhv(header,&tmpvar,&nerr,5);
    if (nerr>0 || rawnpts<=0){
        std::cerr <<  "Warning in " << __func__ << ": No Gcarc for "<< s.filename << " ..." << std::endl;
        return false;
    }
    else{
        s.GCARC=tmpvar;
    }

    s.NPTS=rawnpts;
    s.Delta=round(rawdelta*1000.0)/1000.0;

    s.data.clear();
    s.time.clear();
    for (int Cnt=0;Cnt<s.NPTS;++Cnt){
        s.data.push_back(rawdata[Cnt]);
        s.time.push_back(begt+Cnt*s.Delta);
    }
    auto result=std::minmax_element(s.data.begin(),s.data.end());
    s.Amplitude=fmax(fabs(*result.first),fabs(*result.second));

    if (s.Amplitude<1e-20){
        return false;
    }
    else{
        for (auto &item:s.data){
            item/=s.Amplitude;
        }
        return true;
    }
}

void SeisSignal::clean(){
    this->NPTS=0;
    this->Delta=0;
    this->filename="";

    this->Amplitude=0;
    this->data.clear();
    this->time.clear();
    return;
}


void SeisSignal::interpolate(double delta){

    this->NPTS=1+ceil((this->time[this->time.size()-1]-this->time[0])/delta);
    this->Delta=delta;

    double *x=&(this->time[0]);
    double *y=&(this->data[0]);

    double *xx=(double *)malloc(this->NPTS*sizeof(double));
    double *yy=(double *)malloc(this->NPTS*sizeof(double));

    for (int Cnt=0;Cnt<this->NPTS;++Cnt){
        xx[Cnt]=x[0]+Cnt*delta;
    }

    wiginterpd(x,y,this->data.size(),xx,yy,this->NPTS,0);

    std::vector<double> newtime(xx,xx+this->NPTS);
    std::vector<double> newdata(yy,yy+this->NPTS);

    this->time=newtime;
    this->data=newdata;

    this->normalize();

    free(xx);
    free(yy);

    return;
}

void SeisSignal::normalize(){

    auto result=std::minmax_element(this->data.begin(),this->data.end());
    double tmpamp=fmax(fabs(*result.first),fabs(*result.second));
    this->Amplitude*=tmpamp;
    for (auto &item:this->data){
        item/=tmpamp;
    }

    return;
}

bool SeisSignal::cut(double time1,double time2){

    if (this->NPTS<=0){
        return false;
    }

    if (this->time[this->time.size()-1]<=time1 || time2<=this->time[0]){
        return false;
    }

    decltype(this->time.size()) index;
    decltype(this->time.begin()) remove_begin;
    decltype(this->time.begin()) remove_end;
    decltype(this->time.begin()) remove_begin_1;
    decltype(this->time.begin()) remove_end_1;

    if (this->time[0]<time1){

        remove_begin=this->time.begin();
        remove_end=remove_begin;
        remove_begin_1=this->data.begin();
        remove_end_1=remove_begin_1;

        for (index=0;index<this->time.size();++index){
            if (this->time[index]>time1){
                break;
            }
            else{
                ++remove_end;
                ++remove_end_1;
            }
        }
        this->time.erase(remove_begin,remove_end);
        this->data.erase(remove_begin_1,remove_end_1);
    }

    if (time2<this->time[this->time.size()-1]){

        remove_end=this->time.end();
        remove_begin=remove_end;

        remove_end_1=this->data.end();
        remove_begin_1=remove_end_1;

        for (index=this->time.size()-1;index!=0;--index){
            if (this->time[index]<time2){
                break;
            }
            else{
                --remove_begin;
                --remove_begin_1;
            }
        }
        this->time.erase(remove_begin,remove_end);
        this->data.erase(remove_begin_1,remove_end_1);
    }

    return true;
}

bool SeisSignal::bwfilter(double f1,double f2,int filter_flag){

    if (this->NPTS<=0){
        return false;
    }

    if (f2<=0 && filter_flag==1){
        return false;
    }
    if (f1>f2 && filter_flag==2){
        return false;
    }
    if (f1<=0 && filter_flag==3){
        return false;
    }

    double *tmpp=&(this->data[0]);
    double **tmp=&tmpp;

    switch (filter_flag){
        case 1:
            butterworth_lp(tmp,1,this->NPTS,this->Delta,2,2,f2,tmp);
            break;
        case 2:
            butterworth_bp(tmp,1,this->NPTS,this->Delta,2,2,f1,f2,tmp);
            break;
        case 3:
            butterworth_hp(tmp,1,this->NPTS,this->Delta,2,2,f1,tmp);
            break;
        default:
            ;
    }
    return true;
}

#endif
