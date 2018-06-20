#include<iostream>

#include<ReadParameters.hpp>

using namespace std;

int main(int argc, char **argv){

    enum PIenum{I1,I2,FLAG1};
    enum PSenum{ShowName,Character,FLAG2};
    enum Penum{Rate,Funny,Overall,FLAG3};
    auto Para=ReadParameters<PIenum,PSenum,Penum>(argc,argv,cin,FLAG1,FLAG2,FLAG3);

    cout << Para[ShowName] << endl;

    return 0;
}
