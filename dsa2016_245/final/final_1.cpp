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

using namespace std;
class node{
public:
	node(){}
	node(string sentence_input){
		sentence = sentence_input;
		isWord = 0;
		frequency = 0;
		//fill(next, next + 27, NULL);
		

		for(int i = 0; i < 27; i++){
			next[i] = NULL;
		}
	}

	void setWord(bool TrueOrFaulse){
		isWord = TrueOrFaulse;
	}
	void setFrequency(string frequency_input){
		frequency = stoll(frequency_input, 0, 0);
	}
	//string character;
	string sentence;
	bool isWord;
	node *next[27];
	long long frequency;
};

class dict_tries{
public:
	dict_tries(){
		for(int i = 0; i < 27; i++){
			head[i] = NULL;
		}
	}
	void addword(string word, string frequency){
		stack<string> alphabets = parseWord(word);

		int size = alphabets.size();
		node *current;

		string sentence;
		for(int i = 0; i < size; i++){
			string alphabets_top = alphabets.top();

			sentence += alphabets_top;
			
			if(i == 0){
				if(alphabets_top.compare(" ") == 0){
					if(head[26] == NULL){
						head[26] = new node(sentence);
						current = head[26];
					}else{
						current = head[26];
					}
				}else{
					if(head[*(alphabets_top.c_str()) - 'a'] == NULL){
						head[*(alphabets_top.c_str()) - 'a'] = new node(sentence);
						current = head[*(alphabets_top.c_str()) - 'a'];
					}else{
						current = head[*(alphabets_top.c_str()) - 'a'];
					}
				}
			}else{
				if(alphabets_top.compare(" ") == 0){
					if(current->next[26] == NULL){
						current->next[26] = new node(sentence);
						current = current->next[26];
					}else{
						current = current->next[26];
					}
				}else{
					if(current->next[*(alphabets_top.c_str()) - 'a'] == NULL){
						current->next[*(alphabets_top.c_str()) - 'a'] = new node(sentence);
						current = current->next[*(alphabets_top.c_str()) - 'a'];
					}else{
						current = current->next[*(alphabets_top.c_str()) - 'a'];
					}
				}
			}

			if(i == size - 1){
				current->setWord(1);
				current->setFrequency(frequency);
			}
			//cout << current->sentence << endl;
			alphabets.pop();
		}
	}
	stack<string> parseWord(string word){
		int index = 0;
		int size = word.size();
		stack<string> alphabets;
		for(int i = 1; i <= size; i++){
			//cout << word.back();
			string word_back(&(word.back()));
			alphabets.push(word_back);
			word.pop_back();
		}
		//cout << endl;
		return alphabets;
	}

	node* find(string word){
		stack<string> alphabets = parseWord(word);
		int size = alphabets.size();
		node *current;

		for(int i = 0; i < size; i++){
			string alphabets_top = alphabets.top();
			if(i == 0){
				if(alphabets_top.compare(" ") == 0){
					if(head[26] == NULL){
						while(!alphabets.empty()){
							alphabets.pop();
						}
						return NULL;
					}else{
						current = head[26];
					}
				}else{
					if(head[*(alphabets_top.c_str()) - 'a'] == NULL){
						while(!alphabets.empty()){
							alphabets.pop();
						}
						return NULL;
					}else {
						current = head[*(alphabets_top.c_str()) - 'a'];
					}
				}
			}else{
				if(alphabets_top.compare(" ") == 0){
					if(current->next[26] == NULL){
						while(!alphabets.empty()){
							alphabets.pop();
						}
						return NULL;
					}else{
						current = current->next[26];
					}
				}else{
					if(current->next[*(alphabets_top.c_str()) - 'a'] == NULL){
						while(!alphabets.empty()){
							alphabets.pop();
						}
						return NULL;
					}else {
						current = current->next[*(alphabets_top.c_str()) - 'a'];
					}
				}
			}
			alphabets.pop();
		}
		//cout << endl;
		return current;
	}
private:
	node *head[27];
};

int main(){
	dict_tries *dict = new dict_tries();
	fstream BigData;
	char* inputLine = new char[100];
	int index = 5;
	int i = 0;
	while(index <= 5){
		if(index == 2) BigData.open("/tmp2/dsa2016_project/2gm.small.txt", ios::in);
		if(index == 3) BigData.open("/tmp2/dsa2016_project/3gm.small.txt", ios::in);
		if(index == 4) BigData.open("4gm.small.txt", ios::in);
		if(index == 5) BigData.open("5gm.small.txt", ios::in);

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
					if(index > 3) cout << dict_string << endl;
					dict->addword(dict_string, inputElement);
				}
				element_index++;
			}
		}
		index++;
		BigData.close();
	}
	
	node *node_word;
	
	if((node_word = dict->find("zyrtectablets deliver males prescriptiononlinerx delivered")) != NULL){
		//cout << "123";
		if(node_word->isWord == true)
			cout << "1189  :" << node_word->frequency << endl;
	}
	if((node_word = dict->find("zyrtec prescription zyrtec effect zyrtec")) != NULL){
		cout << "1211  :" << node_word->frequency << endl;
	}
	if((node_word = dict->find("zyrtec zyrtec d zyrtec")) != NULL){
		cout << "1397  :" << node_word->frequency << endl;
	}
	if((node_word = dict->find("zyrtec side effect zye")) == NULL){
		if(node_word->isWord == true){
			cout << node_word->frequency << endl;
		}else{
			cout << "cant find" << endl;
		}
	}
}