#ifndef ASU_MARIADB
#define ASU_MARIADB

#include<iostream>
#include<cstdlib>
#include<cmath>
#include<algorithm>
#include<vector>
#include<map>
#include<unistd.h>

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
        mysql_close(ID);
        return;
    }


    class Select {

        size_t ICnt=0,SCnt=0,DCnt=0,m,n;
        std::map<std::string,int> NameToIndex,TypeCheck;
        std::vector<std::vector<int>> PI;
        std::vector<std::vector<std::string>> PS;
        std::vector<std::vector<double>> PF;


    public:

        const std::vector<int> &GetInt(const std::string &s) const {
            if (NameToIndex.find(s)==NameToIndex.end())
                throw std::runtime_error("Field "+s+" does not exist ...");
            if (TypeCheck.at(s)!=0)
                throw std::runtime_error("Field "+s+" is not integer ...");
            return PI[NameToIndex.at(s)];
        }

        const std::vector<std::string> &GetString(const std::string &s) const {
            if (NameToIndex.find(s)==NameToIndex.end())
                throw std::runtime_error("Field "+s+" does not exist ...");
            if (TypeCheck.at(s)!=1)
                throw std::runtime_error("Field "+s+" is not string ...");
            return PS[NameToIndex.at(s)];
        }

        const std::vector<double> &GetDouble(const std::string &s) const{
            if (NameToIndex.find(s)==NameToIndex.end())
                throw std::runtime_error("Field "+s+" does not exist ...");
            if (TypeCheck.at(s)!=2)
                throw std::runtime_error("Field "+s+" is not float ...");
            return PF[NameToIndex.at(s)];
        }

        void Clear() {
            PI.clear();
            PS.clear();
            PF.clear();
            NameToIndex.clear();
            TypeCheck.clear();
        }

        const size_t &NCol() const {return n;}
        const size_t &NRow() const {return m;}

        Select()=default;
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
            mysql_close(ID);

            m=mysql_num_rows(res),n=mysql_num_fields(res);
            auto fields=mysql_fetch_fields(res);
            std::vector<size_t> FieldIndexToIndex(n);

            for (size_t i=0;i<m;++i) {

                auto row=mysql_fetch_row(res);

                for (size_t j=0;j<n;++j) {

                    if (fields[j].type==FIELD_TYPE_FLOAT || fields[j].type==FIELD_TYPE_DOUBLE) {
                        if (i==0) {
                            NameToIndex[std::string(fields[j].name)]=DCnt;
                            TypeCheck[std::string(fields[j].name)]=2;
                            FieldIndexToIndex[j]=DCnt;
                            PF.push_back(std::vector<double> ());
                            ++DCnt;
                        }
                        PF[FieldIndexToIndex[j]].push_back(row[j]?atof(row[j]):0.0/0.0);
                    }
                    else if (IS_NUM(fields[j].type)){
                        if (i==0) {
                            NameToIndex[std::string(fields[j].name)]=ICnt;
                            TypeCheck[std::string(fields[j].name)]=0;
                            FieldIndexToIndex[j]=ICnt;
                            PI.push_back(std::vector<int> ());
                            ++ICnt;
                        }
                        PI[FieldIndexToIndex[j]].push_back(row[j]?atoi(row[j]):std::numeric_limits<int>::max());
                    }
                    else {
                        if (i==0) {
                            NameToIndex[std::string(fields[j].name)]=SCnt;
                            TypeCheck[std::string(fields[j].name)]=1;
                            FieldIndexToIndex[j]=SCnt;
                            PS.push_back(std::vector<std::string> ());
                            ++SCnt;
                        }
                        PS[FieldIndexToIndex[j]].push_back(row[j]?std::string(row[j]):"NULL");
                    }
                }
            }
            mysql_free_result(res);
        }
    };


    /***********************************************************************************************
     * 1. if t2 doesn't exist, do nothing.
     * 2. if t1 doesn't exist, copy t2 to t1 (create table t1 like t2; insert t1 select * from t2;)
     * 3. fieldname should be unique in t1 and t2. (fieldname is preferred to be primary key).
     *    -- if field name doesn't exist or is not unique, do nothing.
     *
     * 4. update t1 values from t2, using fieldname as key (row count in t1 will not change):
     *    a. if t1 doesn't have the column(s) in t2, paste the colums(s) from t2 to t1.
     *        -- for rows in t1 doesn't appear in t2, use NULL for new columns(s).
     *        -- for rows in t2 doesn't appear in t1, discard them.
     *    b. if t1 have the column(s) in t2, update the values in t1 using the values from t2.
     *        -- for rows in t1 doesn't appear in t2, do nothing.
    ***********************************************************************************************/
    void UpdateTable(const std::string &t1, const std::string &t2, const std::string &fieldname) {


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
        mysql_close(ID);


        // 3. check fieldname exists and not repeated.
        auto res1=Select("column_name from information_schema.columns where table_schema='"+t1db+"' and table_name='"+t1table+"'");
        int Cnt=0;
        for (const auto &item: res1.GetString("column_name"))
            if (item==fieldname) ++Cnt;
        if (Cnt==0)
            throw std::runtime_error("Table "+t1+" doesn't have field called "+fieldname+"...");
        if (Cnt>1)
            throw std::runtime_error("Table "+t1+" have multiple fields called "+fieldname+"...");

        auto res2=Select("column_name,column_type,column_comment from information_schema.columns where table_schema='"+t2db+"' and table_name='"+t2table+"'");
        Cnt=0;
        for (const auto &item: res2.GetString("column_name"))
            if (item==fieldname) ++Cnt;
        if (Cnt==0)
            throw std::runtime_error("Table "+t2+" doesn't have field called "+fieldname+"...");
        if (Cnt>1)
            throw std::runtime_error("Table "+t2+" have multiple fields called "+fieldname+"...");


        // 4. update.
        auto T1N=res1.GetString("column_name"),T2N=res2.GetString("column_name");
        sort(T1N.begin(),T1N.end());
        auto id=SortWithIndex(T2N.begin(),T2N.end());
        auto T2TP=res2.GetString("column_type");
        auto T2CM=res2.GetString("column_comment");
        ReorderUseIndex(T2TP.begin(),T2TP.end(),id);
        ReorderUseIndex(T2CM.begin(),T2CM.end(),id);

        std::vector<std::string> NeedUpdate(T1N.size());
        auto it=std::set_intersection(T1N.begin(),T1N.end(),T2N.begin(),T2N.end(),NeedUpdate.begin());
        NeedUpdate.resize(it-NeedUpdate.begin());
        NeedUpdate.erase(std::remove(NeedUpdate.begin(),NeedUpdate.end(),fieldname),NeedUpdate.end());

        std::vector<std::string> NeedAdd(T2N.size());
        auto it2=std::set_difference(T2N.begin(),T2N.end(),T1N.begin(),T1N.end(),NeedAdd.begin());
        NeedAdd.resize(it2-NeedAdd.begin());

        // create a tmp table.
        std::string tmptable=t1db+".tmptable"+std::to_string(getpid());
        Query("create table "+tmptable+" like "+t1);

        // add new columns to tmp table.
        for (const auto &item:NeedAdd) {
            for (size_t i=0;i<T2N.size();++i) {
                if (T2N[i]==item){
                    Query("alter table "+tmptable+" add column "+item+" "+T2TP[i]+(T2CM[i].empty()?"":(" comment \""+T2CM[i]+"\"")));
                    break;
                }
            }
        }

        // dump t1, t2 data into tmp table.
        cmd="";
        std::string cmd2="";
        for (const auto &item:res1.GetString("column_name")) {
            cmd2+=(t1+"."+item+",");
            if (std::find(NeedUpdate.begin(),NeedUpdate.end(),item)==NeedUpdate.end())
                cmd+=(t1+"."+item+",");
            else 
                cmd+=(t2+"."+item+",");
        }
        for (const auto &item:NeedAdd) {
            cmd+=(t2+"."+item+",");
            cmd2+="NULL,";
        }
        cmd.pop_back();
        cmd2.pop_back();

        Query("insert "+tmptable+" select "+cmd+" from "+t1+" join "+t2+" on "+t1+"."+fieldname+"="+t2+"."+fieldname);
        Query("insert "+tmptable+" select "+cmd2+" from "+t1+" left join "+t2+" on "+t1+"."+fieldname+"="+t2+"."+fieldname+" where "+t2+"."+fieldname+" is null");

        // rename tmp table to t1.
        Query("drop table "+t1);
        Query("rename table "+tmptable+" to "+t1);

        return;
    }
}

#endif
