#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

using namespace std;
int m[3500000];
int main(int argc, char**argv){
	
	int a;
	fstream afile(argv[1]);
	fstream bfile(argv[2]);
	int counta = 0, countb = 0;
	while(afile >> a){
		m[counta] = a;
		counta++;
	}
	int i = 0;
	int correct = 0;
	while(bfile >> a){
		if(a == m[i]) correct++;
		countb++;
		i++;			
	}

	//cout << correct << endl;
	cout << counta << " " << countb << endl;
	cout << (double)correct * (double)100/(double)countb << endl;
	/*
	while(afile >> a >> f){
		cout <<a << " " << stod(f) << endl;
	}*/

}