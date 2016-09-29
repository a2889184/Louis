#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <sstream>
#include <string>
#include <map>
#include <cstdio>
#include <stack>
#include <ostream>

using namespace std;

int main(){
	fstream BigData;
	char* inputLine = new char[100];
	int index = 2;
	int i = 0;
	ofstream outfile;
	/*
	ofstream outfile2("out2.txt",ios::out|ios::trunc);
	ofstream outfile3("out3.txt",ios::out|ios::trunc);
	ofstream outfile("out4.txt",ios::out|ios::trunc);
	ofstream outfile("out5.txt",ios::out|ios::trunc);*/
	while(index <= 5){
		if(index == 2) {
			outfile.open("out2.txt",ios::out|ios::trunc);
			BigData.open("2gm.small.txt", ios::in);
		}
		if(index == 3) {
			outfile.open("out3.txt",ios::out|ios::trunc);
			BigData.open("3gm.small.txt", ios::in);
		}
		if(index == 4) {
			outfile.open("out4.txt",ios::out|ios::trunc);
			BigData.open("4gm.small.txt", ios::in);
		}
		if(index == 5) {
			outfile.open("out5.txt",ios::out|ios::trunc);
			BigData.open("5gm.small.txt", ios::in);
		}
			bool de = false;
			string inputElement;
			int element_index = 0;
			string dict_string;
			while(BigData >> inputElement){
				//cerr << inputElement << endl;
				if(element_index != index){
					dict_string += inputElement;
					if(inputElement.size() >= 15) de = true;
					if(element_index != index - 1) dict_string += " ";
					else{
						if(dict_string.size() >= 65) de = true;
					}
				}else{
					dict_string += "\t";
					dict_string += inputElement;
				}
				element_index++;

				if(element_index == index + 1) {
					if(de == false) outfile << dict_string << endl;
					element_index = 0;
					dict_string = "";
					de = false;
				}
			
			/*if(element_index != index + 1) de = true;
			if(de == false) outfile << inputLine << endl;*/
		}
		index++;
		outfile.close();
		BigData.close();
	}
}