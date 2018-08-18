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

    // Get certain field (int).
    // To dos: Marker1 may be there's a generic way to select integer, string and double.
    std::unordered_map<std::string,std::vector<int>>
    SelectInteger(const std::string &fields, const std::string &cmd){
        auto ID=mysql_init(NULL);
        if (!mysql_real_connect(ID,"127.0.0.1","","",NULL,0,NULL,0)) {
            std::cerr << mysql_error(ID) << std::endl;
            throw std::runtime_error("Connet failed...");
        }

        std::string command="select "+fields+" "+cmd;
        if (mysql_real_query(ID,command.c_str(),command.size())!=0) {
            std::cerr << mysql_error(ID) << std::endl;
            throw std::runtime_error("Select failed...");
        }

        auto res=mysql_store_result(ID);
        size_t m=mysql_num_rows(res),n=mysql_num_fields(res);;

        std::vector<std::vector<int>> V(n,std::vector<int> (m));
        auto fi=mysql_fetch_fields(res);

        std::unordered_map<std::string,std::vector<int>> ans;
        for (size_t i=0;i<n;++i)
            ans[std::string(fi[i].name)]=std::vector<int> ();

        for (size_t i=0;i<m;++i) {
            auto row=mysql_fetch_row(res);
            for(size_t j=0;j<n;++j)
                V[j][i]=(row[j]?atoi(row[j]):std::numeric_limits<int>::max());
        }

        for (size_t i=0;i<n;++i)
            swap(V[i],ans[std::string(fi[i].name)]);

        mysql_free_result(res);
        mysql_close(ID);
        return ans;
    }

    // Get certain field (char *, string).
    std::unordered_map<std::string,std::vector<std::string>>
    SelectString(const std::string &fields, const std::string &cmd){
        auto ID=mysql_init(NULL);
        if (!mysql_real_connect(ID,"127.0.0.1","","",NULL,0,NULL,0)) {
            std::cerr << mysql_error(ID) << std::endl;
            throw std::runtime_error("Connet failed...");
        }

        std::string command="select "+fields+" "+cmd;
        if (mysql_real_query(ID,command.c_str(),command.size())!=0) {
            std::cerr << mysql_error(ID) << std::endl;
            throw std::runtime_error("Select failed...");
        }

        auto res=mysql_store_result(ID);
        size_t m=mysql_num_rows(res),n=mysql_num_fields(res);;

        std::vector<std::vector<std::string>> V(n,std::vector<std::string> (m));
        auto fi=mysql_fetch_fields(res);

        std::unordered_map<std::string,std::vector<std::string>> ans;
        for (size_t i=0;i<n;++i)
            ans[std::string(fi[i].name)]=std::vector<std::string> ();

        for (size_t i=0;i<m;++i) {
            auto row=mysql_fetch_row(res);
            for(size_t j=0;j<n;++j)
                V[j][i]=(row[j]?std::string(row[j]):"NULL");
        }

        for (size_t i=0;i<n;++i)
            swap(V[i],ans[std::string(fi[i].name)]);

        mysql_free_result(res);
        mysql_close(ID);
        return ans;
    }

    // Get certain field (float, double).
    std::unordered_map<std::string,std::vector<double>>
    SelectDouble(const std::string &fields, const std::string &cmd){
        auto ID=mysql_init(NULL);
        if (!mysql_real_connect(ID,"127.0.0.1","","",NULL,0,NULL,0)) {
            std::cerr << mysql_error(ID) << std::endl;
            throw std::runtime_error("Connet failed...");
        }

        std::string command="select "+fields+" "+cmd;
        if (mysql_real_query(ID,command.c_str(),command.size())!=0) {
            std::cerr << mysql_error(ID) << std::endl;
            throw std::runtime_error("Select failed...");
        }

        auto res=mysql_store_result(ID);
        size_t m=mysql_num_rows(res),n=mysql_num_fields(res);;

        std::vector<std::vector<double>> V(n,std::vector<double> (m));
        auto fi=mysql_fetch_fields(res);

        std::unordered_map<std::string,std::vector<double>> ans;
        for (size_t i=0;i<n;++i)
            ans[std::string(fi[i].name)]=std::vector<double> ();

        for (size_t i=0;i<m;++i) {
            auto row=mysql_fetch_row(res);
            for(size_t j=0;j<n;++j)
                V[j][i]=(row[j]?atof(row[j]):0.0/0.0);
        }

        for (size_t i=0;i<n;++i)
            swap(V[i],ans[std::string(fi[i].name)]);

        mysql_free_result(res);
        mysql_close(ID);
        return ans;
    }
*/
}

#endif
