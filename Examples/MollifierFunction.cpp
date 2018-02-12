#include<iostream>

#include<ASU_tools.hpp>

using namespace std;

int main(){

    double x,delta,dt;

    dt=0.1;
    delta=0.6;
    for (x=-1;x<1;x=x+dt){
        // Use Function:
        printf("%.10lf\n",MollifierFunction(x,delta));
    }

    return 0;
}
