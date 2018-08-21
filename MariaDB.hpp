#ifndef ASU_MARIADB
#define ASU_MARIADB

#include<iostream>
#include<cstdlib>
#include<unordered_map>
#include<cmath>
#include<algorithm>
#include<vector>

extern "C" {
// Current version: mariadb-connector-c-3.0.4
#include<mariadb/mysql.h>
}

/*************************************************
 * This is a wrapper for mariadb-connector-c
 *
 * Shule Yu
 * Aug 14 2018
 *
 * Key words: mariadb, mysql
*************************************************/

namespace MariaDB {

    template<class T1, class T2, class T3>
    class Select{

        std::vector<std::vector<int>> PI;
        std::vector<std::vector<std::string>> PS;
        std::vector<std::vector<double>> PF;

    public:

        std::vector<int> &operator[](const T1 &i){
            if ((size_t)i+1<=PI.size()) return PI[(size_t)i];
            throw std::runtime_error("Integer query error!");
        };

        std::vector<std::string> &operator[](const T2 &i){
            if ((size_t)i+1<=PS.size()) return PS[(size_t)i];
            throw std::runtime_error("String query error!");
        };

        std::vector<double> &operator[](const T3 &i){
            if ((size_t)i+1<=PF.size()) return PF[(size_t)i];
            throw std::runtime_error("Float query error!");
        };

        void Clear(){
            PI.clear();
            PS.clear();
            PF.clear();
        }

        size_t NCol(){
            return (size_t)T1::END+(size_t)T2::END+(size_t)T3::END;
        }

        size_t NRow(){
            if ((size_t)T1::END!=0) return PI[0].size();
            if ((size_t)T2::END!=0) return PS[0].size();
            return PF[0].size();
        }


        Select(const std::string &cmd) {
            auto ID=mysql_init(NULL);

            if (!mysql_real_connect(ID,"127.0.0.1","","",NULL,0,NULL,0)) {
                std::cerr << mysql_error(ID) << std::endl;
                throw std::runtime_error("Connet failed...");
            }

            if (mysql_real_query(ID,("select "+cmd).c_str(),("select "+cmd).size())!=0) {
                std::cerr << mysql_error(ID) << std::endl;
                throw std::runtime_error("Select failed...");
            }

            auto res=mysql_store_result(ID);
            size_t m=mysql_num_rows(res);

            PI.resize((int)T1::END);
            PS.resize((int)T2::END);
            PF.resize((int)T3::END);

            for (size_t i=0;i<m;++i) {
                auto row=mysql_fetch_row(res);
                size_t j=0;
                for (size_t k=0;k<(size_t)T1::END;++k) {
                    PI[k].push_back(row[j]?atoi(row[j]):std::numeric_limits<int>::max());
                    ++j;
                }
                for (size_t k=0;k<(size_t)T2::END;++k) {
                    PS[k].push_back(row[j]?std::string(row[j]):"NULL");
                    ++j;
                }
                for (size_t k=0;k<(size_t)T3::END;++k) {
                    PF[k].push_back(row[j]?atof(row[j]):0.0/0.0);
                    ++j;
                }
            }
        }

    };

/*
    template<class T>
    WriteColumn(const std::string &db, const std::string &table, const std::string &fieldname,
                const std::vector<T> &data) {
    }
*/
    
    void Query(const std::string &cmd){
        auto ID=mysql_init(NULL);

        if (!mysql_real_connect(ID,"127.0.0.1","","",NULL,0,NULL,0)) {
            std::cerr << mysql_error(ID) << std::endl;
            throw std::runtime_error("Connet failed...");
        }

        if (mysql_query(ID,cmd.c_str())!=0) {
            std::cerr << mysql_error(ID) << std::endl;
            throw std::runtime_error("Query failed...");
        }
        return;
    }
}

#endif
