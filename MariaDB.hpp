#ifndef ASU_MARIADB
#define ASU_MARIADB

#include<iostream>
#include<cstdlib>
#include<cmath>
#include<algorithm>
#include<vector>

extern "C" {
// Current version: mariadb-connector-c-3.0.4
#include<mariadb/mysql.h>
}

#include<SortWithIndex.hpp>
#include<ReorderUseIndex.hpp>

/*************************************************
 * This is a wrapper for mariadb-connector-c
 *
 * Shule Yu
 * Aug 14 2018
 *
 * Key words: mariadb, mysql
*************************************************/

namespace MariaDB {

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


    void UpdateTable(const std::string &t1, const std::string &t2, const std::string &fieldname) {

/*
           1. if t2 doesn't exist, do nothing.
           2. if t1 doesn't exist, copy t2 to t1 (create table t1 like t2; insert t1 select * from t2;)
           3. fieldname should be unique in t1 and t2. (fieldname is preferred to be primary key).
              -- if field name doesn't exist or is not unique, do nothing.

           4. update t1 values from t2, using fieldname as key (row count in t1 will not change):
              a. if t1 doesn't have the column(s) in t2, paste the colums(s) from t2 to t1.
                  -- for rows in t1 doesn't appear in t2, use NULL for new columns(s).
                  -- for rows in t2 doesn't appear in t1, discard them.
              b. if t1 have the column(s) in t2, update the values in t1 using the values from t2.
                  -- for rows in t1 doesn't appear in t2, do nothing.
*/

        auto ID=mysql_init(NULL);

        if (!mysql_real_connect(ID,"127.0.0.1","","",NULL,0,NULL,0)) {
            std::cerr << mysql_error(ID) << std::endl;
            throw std::runtime_error("Connet failed...");
        }

        // 1. Check t2 exist.
        std::string t2db=t2.substr(0,t2.find_first_of('.'));
        std::string t2table=t2.substr(t2.find_first_of('.')+1);

        std::string cmd="select * from information_schema.tables where table_schema='"+t2db+"' and table_name='"+t2table+"' limit 1";
        if (mysql_real_query(ID,cmd.c_str(),cmd.size())!=0) {
            std::cerr << mysql_error(ID) << std::endl;
            throw std::runtime_error("Select failed...");
        }
        auto res=mysql_store_result(ID);
        if (mysql_num_rows(res)==0) return;


        // 2. Check t1 exist.
        std::string t1db=t1.substr(0,t1.find_first_of('.'));
        std::string t1table=t1.substr(t1.find_first_of('.')+1);
        cmd="select * from information_schema.tables where table_schema='"+t1db+"' and table_name='"+t1table+"' limit 1";
        if (mysql_real_query(ID,cmd.c_str(),cmd.size())!=0) {
            std::cerr << mysql_error(ID) << std::endl;
            throw std::runtime_error("Select failed...");
        }
        res=mysql_store_result(ID);
        if (mysql_num_rows(res)==0) {
            Query("create table "+t1+" like "+t2);
            Query("insert "+t1+" select * from "+t2);
            return;
        }


        // 3. check fieldname exists and not repeated.
        enum class Q1I{END};
        enum class Q1S{column_name,netwk,END};
        enum class Q1F{END};
        auto Q1R=Select<Q1I,Q1S,Q1F>("column_name from information_schema.columns where table_schema='"+t1db+"' and table_name='"+t1table+"'");
        int Cnt=0;
        for (const auto &item: Q1R[Q1S::column_name])
            if (item==fieldname) ++Cnt;
        if (Cnt==0)
            throw std::runtime_error("Table "+t1+" doesn't have field called "+fieldname+"...");
        else if (Cnt>1)
            throw std::runtime_error("Table "+t1+" have multiple fields called "+fieldname+"...");

        enum class Q2I{END};
        enum class Q2S{column_name,column_type,netwk,END};
        enum class Q2F{END};
        auto Q2R=Select<Q2I,Q2S,Q2F>("column_name,column_type from information_schema.columns where table_schema='"+t2db+"' and table_name='"+t2table+"'");
        Cnt=0;
        for (const auto &item: Q2R[Q2S::column_name])
            if (item==fieldname) ++Cnt;
        if (Cnt==0)
            throw std::runtime_error("Table "+t2+" doesn't have field called "+fieldname+"...");
        else if (Cnt>1)
            throw std::runtime_error("Table "+t2+" have multiple fields called "+fieldname+"...");

        
        // 4. update.
        std::vector<std::string> &T1N=Q1R[Q1S::column_name],&T2N=Q2R[Q2S::column_name];
        sort(T1N.begin(),T1N.end());
        auto id=SortWithIndex(T2N.begin(),T2N.end());
        ReorderUseIndex(Q2R[Q2S::column_type].begin(),Q2R[Q2S::column_type].end(),id);

        std::vector<std::string> NeedUpdate(T1N.size());
        auto it=std::set_intersection(T1N.begin(),T1N.end(),T2N.begin(),T2N.end(),NeedUpdate.begin());
        NeedUpdate.resize(it-NeedUpdate.begin());
        NeedUpdate.erase(std::remove(NeedUpdate.begin(),NeedUpdate.end(),fieldname),NeedUpdate.end());

        std::vector<std::string> NeedAdd(T2N.size());
        auto it2=std::set_difference(T2N.begin(),T2N.end(),T1N.begin(),T1N.end(),NeedAdd.begin());
        NeedAdd.resize(it2-NeedAdd.begin());

        // update same columns.

        for (const auto &item: NeedUpdate) {
            Query("update "+t1+", "+t2+" set "+t1+"."+item+"="+t2+"."+item+" where "+t1+"."+fieldname+"="+t2+"."+fieldname);
        }


        // add new columns to t1.
        for (const auto &item:NeedAdd)
            for (size_t i=0;i<T2N.size();++i) {
                if (T2N[i]==item){
                    Query("alter table "+t1+" add column "+item+" "+Q2R[Q2S::column_type][i]);
                    break;
                }
            }

        // dump t2 data into t1.
        cmd=" set ";
        for (const auto &item:NeedAdd)
            cmd+=(t1+"."+item+"="+t2+"."+item+",");
        cmd.pop_back();
        Query("update "+t1+" inner join "+t2+" on "+t1+"."+fieldname+"="+t2+"."+fieldname+cmd);

        return;
    }

}

#endif
