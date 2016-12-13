#include <iostream>
#include <string>
#include <utility>
#include <vector>


using namespace std;


int score[500][500];
pair<int, int> tr_back[500][500];
//implementation of SW algorithm (homework 3 of Computer Architecture)
void SW(string &input1, string &input2){
	for(int i = 0; i < 500; i++){
		score[i][0] = 0;
		score[0][i] = 0;
		
	}

	//compute the score of array
	for(int i = 0; i < input1.size(); i++){
		for(int j = 0; j < input2.size(); j++){
			int score_up = score[i][j + 1] - 2;
			int score_left = score[i + 1][j] - 2;
			int score_up_left;
			if(input1[i] == input2[j]){
				score_up_left = score[i][j] + 3;
			}else{
				score_up_left = score[i][j] - 1;
			}
			if(i == 0 && j == 0){
				score[i + 1][j + 1] = score_up_left;
				tr_back[i + 1][j + 1] = make_pair(i, j);
			}
			else if(i == 0){
				score[i + 1][j + 1] = score_left;
				tr_back[i + 1][j + 1] = make_pair(i + 1, j);
			}else if(j == 0){
				score[i + 1][j + 1] = score_up;
				tr_back[i + 1][j + 1] = make_pair(i, j + 1);
				
			}else{
				if(score_up_left >= score_up && score_up_left >= score_left){
					score[i + 1][j + 1] = score_up_left;
					tr_back[i + 1][j + 1] = make_pair(i, j);
				}else if(score_up >= score_left && score_up >= score_up_left){
					score[i + 1][j + 1] = score_up;
					tr_back[i + 1][j + 1] = make_pair(i, j + 1);
				}else{
					score[i + 1][j + 1] = score_left;
					tr_back[i + 1][j + 1] = make_pair(i + 1, j);
				}
			}

			if(score[i + 1][j + 1] < 0) score[i + 1][j + 1] = 0; 

		}
	}

	/*for(int i = 0; i < input1.size(); i++){
		for(int j = 0; j < input2.size(); j++){
			cout << "(" << tr_back[i + 1][j + 1].first << ", "<< tr_back[i + 1][j + 1].second << ")  ";

		}
		cout << endl;
	}*/

	for(int i = 1; i <= input1.size(); i++){
		for(int j = 1; j <= input2.size(); j++){
			cout << score[i][j] << " ";
		}
		cout << endl;
	}


	vector<char> i_vec;
	vector<char> j_vec;
	//trace back
	int i, j;
	int pre_i, pre_j;
	i = input1.size();
	j = input2.size();

	cout << score[i][j] << endl;

	while(i != 0 && j != 0){
		pre_i = i;
		pre_j = j;
		i = tr_back[pre_i][pre_j].first;
		j = tr_back[pre_i][pre_j].second;
		/*cout << "(" << pre_i << ", " << pre_j << "); (" << i << ", " << j << ")" << endl;
		cout << input1[pre_i - 1] << " " << input2[pre_j - 1] << endl;*/
		if((pre_i - i) == 1 && (pre_j - j) == 1){

			i_vec.push_back(input1[pre_i - 1]);
			j_vec.push_back(input2[pre_j - 1]);
		}else if((pre_i - i) == 1){
			i_vec.push_back(input1[pre_i - 1]);
			j_vec.push_back('-');
		}else{
			i_vec.push_back('-');
			j_vec.push_back(input2[pre_j - 1]);
		}
	}
	//cout alignment input1
	while(!i_vec.empty()){
	
		cout << i_vec.back();
		i_vec.pop_back();
	}
	cout << endl;
	//cout alignment input2
	while(!j_vec.empty()){
		cout << j_vec.back();
		j_vec.pop_back();
	}
	cout << endl;
}

int main(){
	string input1, input2;

	cin >> input1 >> input2;

	SW(input1, input2);

}