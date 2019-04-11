#ifndef ASU_READPARAMETERS
#define ASU_READPARAMETERS

#include<iostream>
#include<sstream>
#include<vector>

/*************************************************************
 * This C++ template class returns a ReadParameters class.
 *
 * input(s):
 * const int     argc    ----  Main function number of arguments.
 * const char    **argv  ----  Main function arguments. argv[0] is the name of the program.
 * istream &is     ----  Input array.
 *
 * output(s):
 * ReadParameters Para  ----  A ReadParameters class. see below for details.
 *
 * Shule Yu
 * Jun 19 2018
 *
 * Key words: arguments, parameters.
*************************************************************/

template<typename T1,typename T2,typename T3>
class ReadParameters{

    std::vector<int> PI;
    std::vector<std::string> PS;
    std::vector<double> P;
    int IntCnt,StringCnt,FloatCnt;

public:

    int &operator[](const T1 &i){
        if (i+1<=(int)PI.size()) return PI[i];
        throw std::runtime_error("Integer use error!");
    };

    std::string &operator[](const T2 &i){
        if (i+1<=(int)PS.size()) return PS[i];
        throw std::runtime_error("String use error!");
    };

    double &operator[](const T3 &i){
        if (i+1<=(int)P.size()) return P[i];
        throw std::runtime_error("Float use error!");
    };
    ReadParameters()=default;
    ReadParameters(const int &argc, const char * const * const &argv, std::istream &is,
                   const T1 &FLAG1, const T2 &FLAG2, const T3 &FLAG3){

        if (argc!=4)
            throw std::runtime_error("Argument Error!");

        IntCnt=atoi(argv[1]);
        StringCnt=atoi(argv[2]);
        FloatCnt=atoi(argv[3]);

        // Check argument number and number of parameter names.
        if (FLAG1!=IntCnt)
            throw std::runtime_error("Integers naming error!");
        if (FLAG2!=StringCnt)
            throw std::runtime_error("String naming error!");
        if (FLAG3!=FloatCnt)
            throw std::runtime_error("Float naming error!");


        std::string tmpstr;
        int tmpint,Cnt;
        double tmpval;

        Cnt=0;
        while (getline(is,tmpstr)){

            ++Cnt;

            std::stringstream ss{tmpstr};

            if (Cnt<=IntCnt){
                if (ss >> tmpint && ss.eof())
                    PI.push_back(tmpint);
                else
                    throw std::runtime_error("Integers read error!");
            }
            else if (Cnt<=IntCnt+StringCnt)
                PS.push_back(tmpstr);
            else if (Cnt<=IntCnt+StringCnt+FloatCnt){
                if (ss >> tmpval && ss.eof())
                    P.push_back(tmpval);
                else
                    throw std::runtime_error("Floats read error!");
            }
            else
                throw std::runtime_error("Redundant inputs!");
        }

        if (Cnt!=IntCnt+StringCnt+FloatCnt)
            throw std::runtime_error("Inadequate inputs!");
    }

};

#endif
