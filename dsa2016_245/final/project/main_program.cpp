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
					if(l != element_index - 1) sentence += " ";
				}
				possible_list.push_back(sentence);
				sentence = "";
			}
		}
		delete [] new_word;
	}
}

void insert_havePre(int element_index, string *word, string *prepositions_array, vector<string> &possible_list,
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
					if(l != element_index - 1) sentence += " ";
				}
				//sentence.pop_back();
				possible_list.push_back(sentence);
				sentence = "";
			}
		}
		delete [] new_word;
	}
}

class mycomparison
{
public:
	mycomparison(){}
	bool operator() (const node &lhs,const node &rhs)
	{
		if(lhs.frequency < rhs.frequency) return 0;
		else if(lhs.frequency > rhs.frequency) return 1;
		else{
			if(lhs.sentence.compare(rhs.sentence) < 0) return 1;
			else return 0;
		}
	}
};
void insert_string(unordered_map<string, long long> &dict, int start, int element_index, vector<int> &position,
	vector<string> *possible_combine, string sentence, string *word, long long &output, int x, vector<int> &continue_prepos_num,
	set<node, mycomparison> &suitable){
	unordered_map<string, long long>::iterator it;
	if(x == 0){
		for(int i = 0; i < position[x]; i++){
			sentence += word[i];
			sentence += " ";
		}
	}else if(x == position.size()){
		for(int i = position[x - 1] + continue_prepos_num[x - 1]; i < element_index; i++){
			sentence += word[i];
			sentence += " ";
		}

		sentence.pop_back();
		it = dict.find(sentence);
		//cout << sentence << endl;
		if(it != dict.end()){
			output++;
			node new_node(it->first, it->second);
			suitable.insert(new_node);
		}
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
					sentence + possible_combine[x][l], word, output, x + 1, continue_prepos_num, suitable);
			}
			else insert_string(dict, 0, element_index, position, possible_combine, 
					sentence + possible_combine[x][l] + " ", word, output, x + 1, continue_prepos_num, suitable);
		}
	}

	sentence = pre;
}


int main(){
	unordered_map<string, long long> dict;
	dict.rehash(35273529);
	fstream BigData;
	char* inputLine = new char[100];
	int index = 2;
	int i = 0;
	
	while(index <= 5){
		/*
		if(index == 2) {
			BigData.open("/tmp2/dsa2016_project/2gm.small.txt", ios::in);
		}
		if(index == 3) {
			BigData.open("/tmp2/dsa2016_project/3gm.small.txt", ios::in);
		}
		if(index == 4) {
			BigData.open("/tmp2/dsa2016_project/4gm.small.txt", ios::in);
		}
		if(index == 5) {
			BigData.open("/tmp2/dsa2016_project/5gm.small.txt", ios::in);
		}
		*/
		if(index == 2) {
			BigData.open("../2gm.small.txt", ios::in);
		}
		if(index == 3) {
			BigData.open("../3gm.small.txt", ios::in);
		}
		if(index == 4) {
			BigData.open("../4gm.small.txt", ios::in);
		}
		if(index == 5) {
			BigData.open("../5gm.small.txt", ios::in);
		}

			string inputElement;
			int element_index = 0;
			string dict_string;
			while(BigData >> inputElement){
				//cerr << inputElement << endl;
				if(element_index != index){
					dict_string += inputElement;
					if(element_index != index - 1) dict_string += " ";
				}else{
				}
				element_index++;

				if(element_index == index + 1) {
					dict.insert(pair<string,long long>(dict_string, stoll(inputElement, 0, 0))); 
					element_index = 0;
					dict_string = "";
				}
			
		}
		index++;
		BigData.close();
	}

/*
	while(index <= 5){
		//tmp/b01504044
		if(index == 2) BigData.open("out2.txt", ios::in);
		if(index == 3) BigData.open("out3.txt", ios::in);
		if(index == 4) BigData.open("out4.txt", ios::in);
		if(index == 5) BigData.open("out5.txt", ios::in);

		while(BigData.getline(inputLine, 100)){
			string inputElement;
			stringstream ss(inputLine);
			int element_index = 0;
			string dict_string;
			while(ss >> inputElement){
				if(element_index != index){
					dict_string += inputElement;
					if(element_index != index - 1) dict_string += " ";
				}else{
					//cout << dict_string << endl;
					dict.insert(pair<string,long long>(dict_string, stoll(inputElement, 0, 0))); 
				}
				element_index++;
			}
		}
		index++;
		BigData.close();
	}
*/
	unordered_set<string> prepositions = {"of" , "to", "in", "for", "with", "on", "at", "by", "from", "up",
	 "about", "than", "after", "before", "down", "between", "under", "since", "without", "near"};
	string prepositions_array[20] = {"of" , "to", "in", "for", "with", "on", "at", "by", "from", "up",
	 "about", "than", "after", "before", "down", "between", "under", "since", "without", "near"};
	set<node, mycomparison> suitable;
	mycomparison myobject;

	while(cin.getline(inputLine, 100)){
		string inputElement;
		stringstream ss(inputLine);
		int element_index = 0;
		string dict_string;
		int PreOrNot[5];
		fill(PreOrNot, PreOrNot + 5, -1);
		int havePre = 0;
		unordered_set<string>::iterator itt;
		unordered_map<string, long long>::iterator it;
		string word[100];
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
		if(havePre == 0){
			
			possible_list.push_back(original_sentence);
			insert_noPre(element_index + 1, word, prepositions_array, possible_list, 1);
			while(!possible_list.empty()){
				string temp = possible_list.back();
				it = dict.find(temp);
				if(it != dict.end()){
					output++;
					node new_node(it->first, it->second);
					suitable.insert(new_node);
				}
				possible_list.pop_back();
			}
			
		}else{
			vector<string> *possible_combine = new vector<string>[position.size()];

			//insertion
			for(int i = 0; i < position.size(); i++){
				insert_havePre(continue_prepos_num[i] + 1, word + position[i], prepositions_array, 
					possible_combine[i], 2);
			}

			//deletion
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

			//substitute
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
			
			insert_string(dict, 0, element_index,position, possible_combine, "", word, output, 0 ,continue_prepos_num, 
				suitable);
			for(int i = 0; i < position.size(); i++){
				possible_combine[i].clear();
			}

		}


		//sort(suitable.begin(), suitable.end(), myobject);
		cout << "query: " << original_sentence << endl;
		if(suitable.size() >= 10) cout << "output: 10" << endl;
		else cout << "output: " << suitable.size() << endl;
		/*while(!suitable.empty()){
			cout << suitable.back()->sentence << "\t" << suitable.back()->frequency << endl;
			suitable.pop_back();
		}*/
		int x = 0;
		for (set<node, mycomparison>::iterator ittt=suitable.begin(); ittt!=suitable.end(); ++ittt){
			cout << (ittt)->sentence << "\t" << (ittt)->frequency << endl;
			x++;
			if(x >= 10) break;
		}
		suitable.clear();

	}
}

