#include <iostream>
#include <map>  
#include <utility>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <fstream>
using namespace std;

int aa[550000][3];
int main(){
	int a, b, c;
	int count = 0;
	fstream fin1, fin2, fin3, fin4, fin5, output;
	fin1.open("B01504044-R05944014/test2/pred1.txt", ios::in);
	while(!fin1.eof()){
		fin1 >> a >> b >> c;
		aa[count][0] = a;
		aa[count][1] = b;
		aa[count][2] = c;
		count++;
	}

	fin2.open("test2/pred.txt", ios::in);
	count = 0;
	int different = 0;
	while(!fin2.eof()){
		fin2 >> a >> b >> c;
		if(aa[count][2] != c) different++;
		count++;

	}

	cout << (double)(different) / (double)(count) << endl;
	
}
