#include<iostream>
#include<fstream>

#include<StreamAddDelimiter.hpp>

using namespace std;

int main(){

    ifstream fpin("data/StreamAddDelimiter_infile");
    StreamAddDelimiter(fpin,"\t,\n");
    string a,b,c,d,e;
    fpin >> a >> b >> c >> d >> e;
    fpin.close();

    cout << a << '\n' << b << '\n' << c << '\n' << d << '\n' << e << endl;

    cout << "By default: \n";
    StreamDefaultDelimiter(fpin);
    fpin.open("data/StreamAddDelimiter_infile");
    fpin >> a >> b >> c >> d >> e;
    fpin.close();

    cout << a << '\n' << b << '\n' << c << '\n' << d << '\n' << e << endl;

    return 0;
}
