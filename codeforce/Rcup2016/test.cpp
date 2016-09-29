#include <iostream>

using namespace std;

int main(){
	cout << 1 << " " << 10000 << endl;
	cout << 10000 << " ";
	for(int i = 0; i < 10000; i++){
		cout << 10001;
		if(i != 9999) cout << " ";
	}

	cout << endl;
	cout << 0 << endl;
}