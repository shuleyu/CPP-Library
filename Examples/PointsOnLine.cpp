#include<iostream>
#include<vector>

#include<ASU_tools.hpp>

using namespace std;

int main(){

	pair<double,double> x{0,1},y{1,2};
	vector<pair<double,double>> p{{0.5,1.5}};

    // Use function.
	auto ans=PointsOnLine(x,y,p);
    for (size_t i=0;i<p.size();++i)
        cout << p[i].first << " " << p[i].second << (ans[i]?" On":" Outof") << '\n';

    return 0;    
}
