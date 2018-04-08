#include<iostream>
#include<cmath>

#include<VectorAngle.hpp>

using namespace std;

int main(){

    pair<double,double> p{23.23123,1.12321};

    for (double theta=0;theta<=370;theta+=10) {

        double t=theta*M_PI/180;
        pair<double,double> q;
        q.first=p.first*cos(t)-p.second*sin(t);
        q.second=p.first*sin(t)+p.second*cos(t);

        q.first*=32.41;
        q.second*=32.41;

        // Use function.
        cout << theta << " " << VectorAngle(p,q)*180/M_PI << endl;;
        cout << endl;
    }


    return 0;    
}
