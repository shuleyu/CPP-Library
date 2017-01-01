#include<string>
#include<algorithm>

using namespace std;

string Decimal2Binary(int n){

	int sign=0;
	if (n<0){
		n+=2147483648;
		sign=-1;
	}

	string Ans;

	while(n>0){
		Ans+=('0'+n%2);
		n=n/2;
	}

	for (int i=Ans.size();i<31;i++) Ans+='0';
	Ans+=(sign==-1)?'1':'0';

	reverse(Ans.begin(),Ans.end());

	return Ans;
}
