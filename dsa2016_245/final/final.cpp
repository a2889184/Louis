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
	map<string, node*> next;
	long long frequency;
};

class dict_tries{
public:
	dict_tries(){}
	void addword(string word, string frequency){
		stack<string> alphabets = parseWord(word);

		int size = alphabets.size();
		node *node_word;
		map<string, node*>::iterator it;
		pair<map<string, node*>::iterator,bool> ret;
		string sentence;
		for(int i = 0; i < size; i++){
			string alphabets_top = alphabets.top();

			sentence += alphabets_top;
			//cout << sentence << endl;
			if(i == 0){
				it = start.find(alphabets_top);
				if(it != start.end()){
					node_word = it->second;
				}else{
					node *node_new = new node(sentence);
					ret = start.insert(pair<string, node*>(alphabets_top, node_new));
					node_word = ret.first->second;
				}
			}else{
				it = (node_word->next).find(alphabets_top);
				if(it != (node_word->next).end()){
					node_word = it->second;
				}else{
					node *node_new = new node(sentence);
					ret = (node_word->next).insert(pair<string, node*>(alphabets_top, node_new));
					node_word = ret.first->second;
				}
			}

			if(i == size - 1){
				node_word->setWord(1);
				node_word->setFrequency(frequency);
			}

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
		node *node_word;
		node *node_addr;
		map<string, node*>::iterator it;
		for(int i = 0; i < size; i++){
			if(i == 0){
				it = start.find(alphabets.top());
				if(it != start.end()){
					node_word = it->second;
				}else {
					while(!alphabets.empty()){
						alphabets.pop();
					}
					return NULL;
				}
			}else if(i == size - 1){
				it = (node_word->next).find(alphabets.top());
				if(it == (node_word->next).end()){
					while(!alphabets.empty()){
						alphabets.pop();
					}
					return NULL;
				}
				node_word = it->second;

			}else{
				it = (node_word->next).find(alphabets.top());
				if(it != (node_word->next).end()){
					node_word = it->second;
				}else {
					while(!alphabets.empty()){
						alphabets.pop();
					}
					return NULL;
				}
			}
			alphabets.pop();
		}
		//cout << endl;
		return node_word;
	}
private:
	map<string, node*> start;
};

int main(){
	dict_tries *dict = new dict_tries();
	fstream BigData;
	char* inputLine = new char[100];
	int index = 2;
	int i = 0;
	while(index <= 5){
		if(index == 2) BigData.open("/tmp/b01504044/out2.txt", ios::in);
		if(index == 3) BigData.open("/tmp/b01504044/out3.txt", ios::in);
		if(index == 4) BigData.open("/tmp/b01504044/out4.txt", ios::in);
		if(index == 5) BigData.open("/tmp/b01504044/out5.txt", ios::in);

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