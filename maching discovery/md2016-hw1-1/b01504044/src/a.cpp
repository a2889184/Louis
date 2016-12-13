#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <utility>
#include <cmath>
#include <vector>
#include <string>

#define PCD pair<char, double>
#define VC vector<char>
#define PB push_back
#define MP make_pair
#define F first
#define S second
#define INF log(0)

using namespace std;
double bigram[40][40];
double encode[40][40];
fstream out;

PCD probable[5000][40];

int switch_to_pos(char in){
	if((int)in >= 97){
		return ((int)in - 97);
	}else if((int)in >= 48){
		return ((int)in - 48 + 26);
	}else if((int)in == 32){
		return 36;
	}
	cout << "out of range in switch" << endl;
	return -1;
}

void read(char* filename, double (*storage)[40]){
	ifstream file(filename);
	char in1, in2;
	char space1, space2, enter;
	double f;
	while(file >> noskipws >> in1 >> space1 >> in2 >> space2 >> f >> enter){
		storage[switch_to_pos(in1)][switch_to_pos(in2)] = log(f);
		//cout << in1 << endl;
		//cout << in2 << endl;
		//cout << f << endl;
	}

}

void init(){
	for(int i = 0; i < 5000; i++){
		for(int j = 0; j < 40; j++){
			probable[i][j] = MP('\0', INF);
		}
	}
}

char switch_to_char(int i){
	if(i <= 25){
		return (char)(i + 97);
	}else if(i <= 35){
		return (char)(i - 26 + 48);
	}else if(i == 36){
		return ' ';
	}
	cout << "out of range in switch" << endl;
	return -1;
}

void trace_back(string element, int elementsize){
	int pos = -1;
	int MAX;
	for(int i = 0; i < 37; i++){
		if(pos == -1 || (probable[elementsize - 1][i].S > MAX)){
			pos = i;
			MAX = probable[elementsize - 1][i].S;
		}
	}
	char now;
	VC v;
	now = switch_to_char(pos);
	int layer = elementsize - 1;
	while(now != '\0'){
		v.PB(now);
		now = probable[layer][switch_to_pos(now)].F;
		layer--;
	}

	int vectorsize = v.size();
	while(!v.empty()){
		out << v.back();
		v.pop_back();
	}

	//cout << " ";
}

int main(int argc, char **argv) {

	char c;
	int count = 0;
	while(cin.get(c)){
		if(c == ' '){
			count++;
		}
	}

	cout << count << endl;

		
}

