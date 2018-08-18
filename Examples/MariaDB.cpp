#include<iostream>

#include<MariaDB.hpp>

using namespace std;

int main(){

    enum class Q1I{END};
    enum class Q1S{PreEQ,END};
    enum class Q1F{lat,END};
    auto res=MariaDB::Select<Q1I,Q1S,Q1F>("concat(\"Prefix\",EQ) as PreEQ,lat from playground.T1");

    for (size_t i=0;i<res.NRow();++i)
        cout << res[Q1S::PreEQ][i] << " " << res[Q1F::lat][i] << endl;
    res.Clear();

    return 0;
}
