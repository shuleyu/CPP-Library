#ifndef ASU_SACSIGNALS
#define ASU_SACSIGNALS

#define MAXL 500000

#include<vector>
#include<string>
#include<cstring>
#include<fstream>

extern "C"{
#include<sacio.h>
#include<sac.h>
}

#include<Interpolate.hpp>
#include<EvenSampledSignal.hpp>

struct SACMetaData{
    std::string stnm,netwk;
    double gcarc;
    SACMetaData (const std::string &s, const std::string &n, const double &g) : 
        stnm(s.substr(0,s.find_last_not_of(" \n\r\t")+1)),
        netwk(n.substr(0,n.find_last_not_of(" \n\r\t")+1)),
        gcarc(g) {}
};

class SACSignals {
    private:
        std::vector<EvenSampledSignal> Data;
        std::vector<SACMetaData> MData;
        std::string FileName="";
        bool SameSamplingRate=false;
    public:
        SACSignals () = default;
        SACSignals (const std::string &s){
            std::ifstream fpin(s);
            fpin >> *this;
            fpin.close();
            FileName=s;
        }

    void clear() {Data.clear();MData.clear();}
    std::size_t size() const {return Data.size();}
    std::string filename() const {return FileName;};

    void PrintInfo() const {
        PrintLessInfo();
        std::cout << "====== \n";
        for (size_t i=0;i<size();++i) {
            std::cout << "Network: |" << MData[i].netwk << "|\n";
            std::cout << "StationName: |" << MData[i].stnm << "|\n";
            std::cout << "Gcarc: |" << MData[i].gcarc << "|\n";
            Data[i].PrintInfo();
            std::cout << "------ \n";
        }
    }
    void PrintLessInfo() const {
        std::cout << "Read from file: " << filename() << '\n';
        std::cout << "Current valid trace nubmer: " << size() << '\n';
    }

    void CheckDist(const double &d1=-1, const double &d2=181){
        std::vector<std::size_t> BadIndices;
        for (size_t i=0;i<size();++i)
            if (MData[i].gcarc<d1 || MData[i].gcarc>d2) BadIndices.push_back(i); 
        RemoveRecords(BadIndices);
    }

    void RemoveRecords(const std::vector<std::size_t> &b){
        for (const auto &index:b){
            std::swap(MData[index],MData.back());
            MData.pop_back();
            std::swap(Data[index],Data.back());
            Data.pop_back();
        }
    }
    void Interpolate(const double &delta) {
        std::vector<EvenSampledSignal> Old_Data;
        std::swap(Data,Old_Data);
        for (std::size_t i=0;i<Old_Data.size();++i){
            EvenSampledSignal Tmp(Old_Data[i],delta);
            Data.push_back(Tmp);
        }
        SameSamplingRate=true;
    }

    // non-member friends declearation.
    friend std::istream &operator>>(std::istream &, SACSignals &);
    friend class SACMetaData;
};

// Overload operator ">>" to read a list of SAC filenames from a one-columned input (stdin/file/etc.)
std::istream &operator>>(std::istream &is, SACSignals &item){

    item.clear();
    std::string filename;
    char file[300],stnm[20],netwk[20],st[6]="kstnm",kt[7]="knetwk",gc[6]="gcarc";
    int rawnpts,maxl=MAXL,nerr;
    float rawbeg,rawdel,rawdata[MAXL],gcarc;
    std::vector<double> D;

    while (is >> filename){
        strcpy(file,filename.c_str());
        rsac1(file,rawdata,&rawnpts,&rawbeg,&rawdel,&maxl,&nerr,strlen(file));
        if (nerr!=0) continue;
        getkhv(st,stnm,&nerr,5,20);
        getkhv(kt,netwk,&nerr,6,20);
        getfhv(gc,&gcarc,&nerr,5);
        item.MData.push_back(SACMetaData(std::string(stnm),std::string(netwk),gcarc));

        D.resize(rawnpts);
        for (int i=0;i<rawnpts;++i) D[i]=rawdata[i];

        EvenSampledSignal Tmp(D,rawdel,rawbeg,filename);
        item.Data.push_back(Tmp);
    }

    return is;
}

#endif
