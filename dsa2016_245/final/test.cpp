#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <sstream>
#include <string>
#include <cstdio>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <vector>


using namespace std;

class node{
public:
	node(string input_sentence, long long input_frequency){
		sentence = input_sentence;
		frequency = input_frequency;
	}
	string sentence;
	long long frequency;
};
void insert_noPre(int element_index, string *word, string *prepositions_array, vector<string> &possible_list,
	int times){
	if(element_index <= 5){
		string *new_word = new string[element_index];
		string sentence = "";
		for(int i = 0; i < element_index; i++){
			for(int j = 0; j < i; j++){
				new_word[j] = word[j];
			}
			for(int j = i ; j < element_index - 1; j++){
				new_word[j + 1] = word[j];
			}
			for(int k = 0; k < 20; k++){
				new_word[i] = prepositions_array[k];
				if(element_index + 1 <= 5 && times == 1) 
					insert_noPre(element_index + 1, new_word, prepositions_array, possible_list, times + 1);
				for(int l = 0; l < element_index; l++){
					sentence += new_word[l];
					if(l != element_index) sentence += " ";
				}
				sentence.pop_back();
				possible_list.push_back(sentence);
				sentence = "";
			}
		}
		delete [] new_word;
	}
}
void insert_string(unordered_map<string, long long> &dict, int start, int element_index, vector<int> &position,
	vector<string> *possible_combine, string sentence, string *word, int &output, int x,
	vector<int> &continue_prepos_num){
	unordered_map<string, long long>::iterator it;

	if(x == 0){
		for(int i = 0; i < position[x]; i++){
			sentence += word[i];
			sentence += " ";
		}
	}else if(x == position.size()){
		for(int i = position[x - 1] + 1; i < element_index; i++){
			sentence += word[i];
			sentence += " ";
		}

		sentence.pop_back();
		cout << sentence << endl;
	}else{
		for(int i = position[x - 1] + continue_prepos_num[x - 1]; i < position[x]; i++){
			sentence += word[i];
			sentence += " ";
		}
	}

	string pre = sentence;
	if(x < position.size()){
		for(int l = 0; l < possible_combine[x].size(); l++){
			if(possible_combine[x][l].size() == 0){
				insert_string(dict, 0, element_index, position, possible_combine, 
				sentence + possible_combine[x][l], word, output, x + 1, continue_prepos_num);
			}
			else insert_string(dict, 0, element_index, position, possible_combine, 
				sentence + possible_combine[x][l] + " ", word, output, x + 1, continue_prepos_num);
		}
	}

	sentence = pre;
			
}

int main(){
	char* inputLine = new char[100];
	unordered_map<string, long long> dict;

	set<node> suitable;
	unordered_set<string> prepositions = {"of" , "to", "in", "for", "with", "on", "at", "by", "from", "up",
	 "about", "than", "after", "before", "down", "between", "under", "since", "without", "near"};
	string prepositions_array[20] = {"of" , "to", "in", "for", "with", "on", "at", "by", "from", "up",
	 "about", "than", "after", "before", "down", "between", "under", "since", "without", "near"};
	string word[3] = {"123", "456", "789"};
	while(cin.getline(inputLine, 100)){
		string inputElement;
		stringstream ss(inputLine);
		int element_index = 0;
		string dict_string;
		int PreOrNot[5];
		fill(PreOrNot, PreOrNot + 5, -1);
		int havePre = 0;
		unordered_set<string>::iterator itt;
		string word[30];
		vector<string> possible_list;
		long long output = 0;
		vector<int> position;
		vector<int> continue_prepos_num;
		int pre_value = -110;
		while(ss >> inputElement){
			itt = prepositions.find(inputElement);
			if(itt != prepositions.end()){
				if(element_index == 0){
					position.push_back(element_index);
					continue_prepos_num.push_back(1);
				}else if(element_index == pre_value + 1){
					continue_prepos_num.back()++;
				}else{
					position.push_back(element_index);
					continue_prepos_num.push_back(1);
				}
				havePre ++;
				pre_value = element_index;
			}
			word[element_index] = inputElement;

			element_index++;
		}
		
		string original_sentence = "";
		for(int l = 0; l < element_index; l++){
			original_sentence += word[l];
			if(l != element_index - 1) original_sentence += " ";
		}
		possible_list.push_back(original_sentence);
		vector<string> *possible_combine = new vector<string>[position.size()];
		for(int i = 0; i < position.size(); i++){
			string need = "";
			for(int j = 0; j < continue_prepos_num[i]; j++){
				string pre = *(word + position[i] + j);
				
				for(int y = 0; y < 20; y++){
					
						*(word + position[i] + j) = prepositions_array[y];
						for(int l = 0; l < continue_prepos_num[i]; l++){
							need += *(word + position[i] + l);
							if(l != continue_prepos_num[i] - 1) need += " ";
						}
						possible_combine[i].push_back(need);
						need = "";
					
				}
				*(word + position[i] + j) = pre;

			}
		}
		for(int i = 0; i < position.size(); i++){
				for(int j = 0; j < continue_prepos_num[i]; j++){
					string need = "";
					for(int y = 0; y < continue_prepos_num[i]; y++){
						if(y == j){}
						else{
							need += *(word + position[i] + y);
							need += " ";
						}
					}
					if(need.size() > 0)	need.pop_back();
					possible_combine[i].push_back(need);
				}
			}

		for(int i = 0; i < position.size(); i++){
				insert_noPre(continue_prepos_num[i] + 1, word + position[i], prepositions_array, 
					possible_combine[i], 2);
			}
			
			int g = 0;
		insert_string(dict, 0, element_index,position, possible_combine, "", word, g, 0 ,continue_prepos_num);

	}
}
	