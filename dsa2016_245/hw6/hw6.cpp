#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <sstream>
#include <string>
#include <set>
#include <cstdio>
#include <unordered_set>
#include <cctype>
#include <vector>
#include <cmath>
#include <iomanip>
#include <stack>
using namespace std;
float **memory;
float QBSH(int pitch_index, int note_index, float *pitch_array, float *note_array);
int main(){
	/*get the input*/
	int number_pitch;
	int number_note;
	if(scanf("%d\n", &number_pitch) != 1){
		printf("wrong input\n");
	}   //number of pitch
	float *pitch_array = new float[number_pitch];  //make the array of pitch
	int k = 0;
	while(k < number_pitch && scanf("%f ", pitch_array + k)){
		k++;
	}
	if(scanf("%d\n", &number_note) != 1){
		printf("wrong input\n");
	} 

	float *note_array = new float [number_note];
	k = 0;
	while(k < number_note && scanf("%f ", note_array + k)){
		k++;
	}

	memory = new float*[number_note];
	for(int i = 0; i < number_note; i++){
		memory[i] = new float[number_pitch];
	}

	for(int i = 0; i < number_note; i++){
		for(int j = 0; j < number_pitch; j++){
			if(i > j) memory[i][j] = numeric_limits<float>::infinity();
			else memory[i][j] = -1;
		}
	}

	/*dynamic programming*/
	QBSH(number_pitch, number_note, pitch_array, note_array);
	int min_index;
	float min = numeric_limits<float>::infinity();
	for(int i = 0; i < number_note; i++){
		if(memory[i][number_pitch - 1] > 0 && memory[i][number_pitch - 1] < min){
			min = memory[i][number_pitch - 1];
			min_index = i;
		}
	}
	cout << memory[min_index][number_pitch - 1] << endl;
	int i = min_index;
	int j = number_pitch - 1;
	stack<float> s;
	cout << 0 << " ";
	while(i > 0 && j > 0){
		if(memory[i - 1][j - 1] <= memory[i][j - 1]) {
			s.push(j);
			i--;
			j--;
		}else{
			j--;
		}
	}
	while(!s.empty()){
		cout << s.top() << " ";
		s.pop();
	}
	/*cout << endl;
	for(int j = 0; j < number_pitch; j++){
		cout << setw(13) << left << j;
	}
	cout << endl;
	for(int i = number_note - 1; i >= 0; i--){
		for(int j = 0; j < number_pitch ; j++){
			if(memory[i][j] == numeric_limits<float>::infinity()) cout << setw(12) << left << -0.00001;
			else cout << setw(12) << left << memory[i][j] << " ";
		}
		cout << endl;
	}*/

}

float QBSH(int pitch_index, int note_index, float *pitch_array, float *note_array){
	
	//float difference = fabs(pitch_array[pitch_index] - note_array[note_index]);
	//cout << pitch_index << " " << note_index << endl;
	
	/*if(memory[note_index][pitch_index] < 0){
		if(pitch_index == 0 && note_index == 0) {
			//starting_index.push_back(pitch_index);
			memory[note_index][pitch_index] = difference;
		}else if(pitch_index == 0){
			memory[note_index][pitch_index] = numeric_limits<float>::infinity(); //infinity
		}else if(note_index == 0){
	        memory[note_index][pitch_index] = difference + QBSH(pitch_index - 1, note_index, pitch_array, note_array);
		}else{
			memory[note_index][pitch_index] = difference + min(QBSH(pitch_index - 1, note_index - 1, pitch_array, note_array), 
				QBSH(pitch_index - 1, note_index, pitch_array, note_array));
		}
	}
	return memory[note_index][pitch_index];*/
	for(int i = 0; i < note_index; i++){
		for(int j = 0; j < pitch_index; j++){
			float difference = fabs(pitch_array[j] - note_array[i]);
			if(memory[i][j] < 0){
				if(i == 0 && j == 0) {
					memory[i][j] = difference;
				}else if(j == 0){
					memory[i][j] = numeric_limits<float>::infinity(); //infinity
				}else if(i == 0){
			        memory[i][j] = difference + memory[i][j - 1];
				}else{
					memory[i][j] = difference + min(memory[i - 1][j - 1], memory[i][j - 1]);
				}
			}
		}
	}
}