#include<iostream>

#include<MariaDB.hpp>

using namespace std;

int main(){

    MariaDB::Query("drop table if exists playground.T1_BU");
    MariaDB::Query("create table playground.T1_BU(PairName varchar(30) primary key, Lat double comment \"Capital field name?\", lon double, flag boolean)");
    vector<string> pairnames{"2015_R32A","2014_R32A","2013_R32A","2012_R32A","2012_R34A"};
    vector<string> lats{"11","21","31","41","41"},lons{"322","333","333","333","333"};
    vector<string> F{"0","1","0","1","0"};
    MariaDB::LoadData("playground","T1_BU",vector<string> {"pairname","lat","lon","flag"},vector<vector<string>> {pairnames,lats,lons,F});


    MariaDB::Query("drop table if exists playground.T2_BU");
    MariaDB::Query("create table playground.T2_BU(Pairname varchar(20) primary key, neTwk varchar (20), Gcarc double, lat double)");
    pairnames=vector<string> {"2015_R32A","2014_R32A","2013_R32A","2011_R32A"};
    lats=vector<string> {"NULL","22","22","22"};
    vector<string> netwk{"ST","QT","CT","JT"};
    vector<string> gcarc{"43.22","123.1","21.5","7.6"};
    MariaDB::LoadData("playground","T2_BU",vector<string> {"pairname","lat","netwk","gcarc"},vector<vector<string>> {pairnames,lats,netwk,gcarc});


    MariaDB::Query("drop table if exists playground.T1");
    MariaDB::Query("create table playground.T1 like playground.T1_BU");
    MariaDB::Query("insert playground.T1 select * from playground.T1_BU");

    MariaDB::Query("drop table if exists playground.T2");
    MariaDB::Query("create table playground.T2 like playground.T2_BU");
    MariaDB::Query("insert playground.T2 select * from playground.T2_BU");

    cout << (MariaDB::CheckTableExists("playground","T1")?"T1 Exists":"T1 doesn't exist") << endl;

    cout << "T1: " << endl;
    auto res=MariaDB::Select("pairname as pn,lat,flag,lon from playground.T1");
    cout << "pairname lat flag lon" << endl;
    for (size_t i=0;i<res.NRow();++i)
        cout << res.GetString("pn")[i] << " " << res.GetDouble("lat")[i] << " " << res.GetInt("flag")[i] << " " << res.GetDouble("lon")[i] << endl;

    cout << "T2: " << endl;
    res=MariaDB::Select("pairname as pn,lat,netwk,gcarc from playground.T2");
    cout << "pairname lat netwk gcarc" << endl;
    for (size_t i=0;i<res.NRow();++i)
        cout << res.GetString("pn")[i] << " " << res.GetDouble("lat")[i] << " " << res.GetString("netwk")[i] << " " << res.GetDouble("gcarc")[i] << endl;

    MariaDB::UpdateTable("playground.T1","playground.T2","pairname");

    cout << "T1 Updated: " << endl;
    res=MariaDB::Select("pairname as pn,lat,flag,lon,netwk,gcarc from playground.T1");
    cout << "pairname lat flag lon netwk gcarc" << endl;
    for (size_t i=0;i<res.NRow();++i)
        cout << res.GetString("pn")[i] << " " << res.GetDouble("lat")[i] << " " << res.GetInt("flag")[i] << " " << res.GetDouble("lon")[i]  << " " << res.GetString("netwk")[i] << " " << res.GetDouble("gcarc")[i] << endl;

    return 0;
}

