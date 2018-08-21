#include<iostream>

#include<MariaDB.hpp>

using namespace std;

int main(){

/*

    enum class Q1I{END};
    enum class Q1S{PreEQ,END};
    enum class Q1F{lat,END};
    auto res=MariaDB::Select<Q1I,Q1S,Q1F>("concat(\"a_\",EQ) as PreEQ,lat from playground.T1");

    for (size_t i=0;i<res.NRow();++i)
        cout << res[Q1S::PreEQ][i] << " " << res[Q1F::lat][i] << endl;
    res.Clear();

*/



//     MariaDB::Query("create table playground.TT(id int, name text)");


    enum class Q1I{Blah,END};
    enum class Q1S{name,END};
    enum class Q1F{END};
    auto res=MariaDB::Select<Q1I,Q1S,Q1F>("id as Blah,name from playground.TT");

    for (size_t i=0;i<res.NRow();++i)
        cout << res[Q1I::Blah][i] << " " << res[Q1S::name][i] << endl;
    res.Clear();

    return 0;
}
