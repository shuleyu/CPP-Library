#include<iostream>

#include<MariaDB.hpp>

using namespace std;

int main(){

    MariaDB::Query("drop table if exists playground.T1_BU");
    MariaDB::Query("create table playground.T1_BU(pairname varchar(30) primary key, lat double, lon double)");
    MariaDB::Query("insert into playground.T1_BU values('2015_R32A',11,333)");
    MariaDB::Query("insert into playground.T1_BU values('2014_R32A',21,333)");
    MariaDB::Query("insert into playground.T1_BU values('2013_R32A',31,333)");
    MariaDB::Query("insert into playground.T1_BU values('2012_R32A',41,333)");


    MariaDB::Query("drop table if exists playground.T2");
    MariaDB::Query("create table playground.T2(pairname varchar(20) primary key, netwk varchar (20), gcarc double, lat double)");
    MariaDB::Query("insert into playground.T2 values('2015_R32A','ST',43.22,NULL)");
    MariaDB::Query("insert into playground.T2 values('2014_R32A','QT',123.1,22)");
    MariaDB::Query("insert into playground.T2 values('2013_R32A','CT',21.5,22)");
    MariaDB::Query("insert into playground.T2 values('2011_R32A','JT',7.6,22)");


    MariaDB::Query("drop table if exists playground.T1");
    MariaDB::Query("create table playground.T1 like playground.T1_BU");
    MariaDB::Query("insert playground.T1 select * from playground.T1_BU");


    MariaDB::UpdateTable("playground.T1","playground.T2","pairname");


    auto res=MariaDB::Select("pairname as pn,netwk,abs(lat) as x from playground.T1");
    for (size_t i=0;i<res.NRow();++i)
        cout << res.GetString("pn")[i] << " " << res.GetDouble("x")[i] << endl;

    return 0;
}

