#include<iostream>

#include<MariaDB.hpp>

using namespace std;

int main(){

    MariaDB::Query("drop table if exists playground.T1_BU");
    MariaDB::Query("create table playground.T1_BU(PairName varchar(30) primary key, Lat double comment \"Capital field name?\", lon double, flag boolean)");
    MariaDB::Query("insert into playground.T1_BU values('2015_R32A',11,333,false)");
    MariaDB::Query("insert into playground.T1_BU values('2014_R32A',21,333,false)");
    MariaDB::Query("insert into playground.T1_BU values('2013_R32A',31,333,true)");
    MariaDB::Query("insert into playground.T1_BU values('2012_R32A',41,333,true)");
    MariaDB::Query("insert into playground.T1_BU values('2012_R34A',41,333,true)");


    MariaDB::Query("drop table if exists playground.T2");
    MariaDB::Query("create table playground.T2(Pairname varchar(20) primary key, neTwk varchar (20), Gcarc double, lat double)");
    MariaDB::Query("insert into playground.T2 values('2015_R32A','ST',43.22,NULL)");
    MariaDB::Query("insert into playground.T2 values('2014_R32A','QT',123.1,22)");
    MariaDB::Query("insert into playground.T2 values('2013_R32A','CT',21.5,22)");
    MariaDB::Query("insert into playground.T2 values('2011_R32A','JT',7.6,22)");


    MariaDB::Query("drop table if exists playground.T1");
    MariaDB::Query("create table playground.T1 like playground.T1_BU");
    MariaDB::Query("insert playground.T1 select * from playground.T1_BU");


    MariaDB::UpdateTable("playground.T1","playground.T2","pairname");

    cout << (MariaDB::CheckTableExists("playground","T1")?"Yes":"No") << endl;

    auto res=MariaDB::Select("pairname as pn,abs(lat) as x,flag from playground.T1");
    for (size_t i=0;i<res.NRow();++i)
        cout << res.GetString("pn")[i] << " " << res.GetDouble("x")[i] << " " << res.GetInt("flag")[i] << endl;

    return 0;
}

