#include<iostream>
#include<limits>

#include<Decimal2Binary.hpp>

using namespace std;

int main(){

    for (int index=-2;index<3;index++){
        cout << Decimal2Binary(index) << " " << index << endl;
    }
    cout << endl;

    for (int index=numeric_limits<int>::min();index<numeric_limits<int>::min()+3;index++){
        cout << Decimal2Binary(index) << " " << index << endl;
    }
    cout << endl;

    for (int index=numeric_limits<int>::max()-2;;index++){
        cout << Decimal2Binary(index) << " " << index << endl;
        if (index==numeric_limits<int>::max()) break;
    }

    return 0;
}
