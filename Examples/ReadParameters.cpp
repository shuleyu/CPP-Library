#include<iostream>

#include<ReadParameters.hpp>

using namespace std;

int main(int argc, char **argv){

    enum PI{I1,I2,FLAG1};
    enum PS{ShowName,Character,FLAG2};
    enum PF{Rate,Funny,Overall,FLAG3};
    auto Para=ReadParameters<PI,PS,PF>(argc,argv,cin,FLAG1,FLAG2,FLAG3);

    Para[I2]=4;
    cout << Para[I2] << endl;
    cout << Para[ShowName] << endl;

    return 0;
}
