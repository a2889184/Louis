#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;



int main(){
	int city, road, flour;

	cin >> city >> road >> flour;
	cout << city << " " << road << " " << flour << endl;
	srand(time(0));
	for(int i = 0; i < road; i++){
		cout << rand() % city + 1 << " " << rand() % city + 1 << " " << 1000000000 << endl;
	}

	for(int i = 0; i < flour; i++){
		cout << rand() % city + 1;
		if(i != flour - 1) cout << " ";
	}

	cout << endl;

}