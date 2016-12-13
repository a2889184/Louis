#include <fstream>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <unordered_map>
#include <algorithm>
#include <set>
#include <cstdlib>
#include <ctime>
using namespace std;
#define TABLE unordered_map< int, multimap<double, int> >
struct User{
	int id;
	vector<int> relation;
	set<int> item;
};

struct Item{
	int id;
	int link;
	set<int> user;
	set<int> category;
};

struct Category{
	int id;
	set<int> item;
};

struct Y{
	int id, user, item;
	int itemindex;
	int fUF, fIO, fCP, gOI, gFI, gOF, gCC;
	float h;
};



vector<User> AllUser;
map<int, Item> AllItem;
map<int, Category> AllCategory;
TABLE possible_user;
int f_n = 6;

int Y[123780][8];
//float theta[123780][3];
int score[123780] = {0};
int response[123780][2];
float theta[8];
double learn[8];
double delta_theta[8];
pair<int, int> user_value[250000];
pair<int, int> sorted_user_value[250000];

set<int>::iterator sit;
map<int, Item>::iterator it;
map<int, Item>::iterator it2;
vector<int>::iterator vit;
set<int>::iterator sit2;
set<int>::iterator sit3;
map<int, Category>::iterator cit;

/******************************************************************************
* input:
* 	upper: the average features of the top like_number's users of specific item.
*	lower: the average features of other users of specific item. 
*	
*	comment: both input is a vector 
* return: 
*	delta_theta
*
*******************************************************************************/
double* compute_delta_theta(double *upper, double *lower){
	for(int i = 0; i < f_n; i++){
		delta_theta[i] = upper[i] - lower[i];
	}
}

//compare function

bool compare_function(pair<int, int> i, pair<int, int> j){
	return i.second > j.second;
}

inline double f1(int user, int item){
	return AllUser[user].item.size() + 1;
}

inline double f2(int user, int item){
	return AllUser[user].relation.size() + 1;
}
inline double f3(int user, int item){

	sit = AllUser[user].item.find(item);
	if(sit != AllUser[user].item.end()){
		return (1 + 1);
	}
	return (0 + 1);
}

inline double f4(int user, int item){
	int feature = 0;

	for(vit = AllUser[user].relation.begin(); vit != AllUser[user].relation.end(); vit++){
		it = AllItem.find(item);
		if(it == AllItem.end()) continue;
		sit = it->second.user.find(*vit);

		if(sit != it->second.user.end()){
			feature++;
		}
	}

	return feature + 1;
}

inline double f5(int user, int item){
	it = AllItem.find(item);
	int feature = 0;
	for(sit = it->second.category.begin(); sit != it->second.category.end(); sit++){
		cit = AllCategory.find(*sit);
		feature += (cit->second.item.size());
	}

	return feature + 1;
}

inline double f6(int user, int item){
	it = AllItem.find(item);
	int feature = 0;
	
	for(sit2 = it->second.category.begin(); sit2 != it->second.category.end(); sit2++){
		for(sit = AllUser[user].item.begin(); sit != AllUser[user].item.end(); sit++){
			it2 = AllItem.find(*sit);

			for(sit3 = it2->second.category.begin(); sit3 != it2->second.category.end(); sit3++){
				if(*sit2 == *sit3) {
					feature++;
				}
			}

		}
	}


	return feature + 1;
}
inline double compute_parameter(int user, int item){
	learn[0] = f1(user, item);
	learn[1] = f2(user, item);
	learn[2] = f3(user, item);
	learn[3] = f4(user, item);
	learn[4] = f5(user, item);
	learn[5] = f6(user, item);

	double out = 0;
	for(int i = 0; i < f_n; i++){
		out += log(learn[i]) * theta[i];
	}

	return out;
}


int main(){
	int count = 0;
	int count2 = 0;
	char cc;

	for(int i = 0; i < f_n; i++){
		theta[i] = 1;
	}
	/*for(int i = 0; i < 123780; i++){
		for(int j = 0; j < 3; j++){
			theta[i][j] = 1;
		}
	}*/
	fstream fin1, fin2, fin3, fin4, fin5, output;
	fin1.open("user.txt", ios::in);
	int a, b, c, d;
	while (!fin1.eof()){
		fin1 >> a;
		User u;
		u.id = a;
		AllUser.push_back(u);
	}
	fin2.open("relation.txt", ios::in);
	while (!fin2.eof()){
		fin2 >> a >> b;
		AllUser[a].relation.push_back(b);
	}
	int havefriend = 0;
	for (int i = 0; i < AllUser.size(); i++){
		if (AllUser[i].relation.size() != 0)
			havefriend++;
	}
	fin5.open("message.txt", ios::in);
	//cout << "num havefriend:  " << havefriend << endl;
	map<int, Item>::iterator it;

	while (!fin5.eof()){
		fin5 >> a >> b >> c >> d;
		it = AllItem.find(b);
		if(it == AllItem.end()){
			Item ii;
			ii.link = d;
			ii.id = b;
			ii.user.insert(a);
			ii.category.insert(c);
			AllItem.insert(make_pair(b, ii));
		}else{
			(*it).second.user.insert(a);
			(*it).second.category.insert(c);
		}

		cit = AllCategory.find(c);
		if(cit == AllCategory.end()){
			Category cat;
			cat.id = c;
			cat.item.insert(b);
			AllCategory.insert(make_pair(c, cat));
		}else{
			cit->second.item.insert(b);
		}

		AllUser[a].item.insert(b);
	}

	//compute value

/*
	for(int i = 0; i < AllUser.size(); i++){
		cout << user_value[i].first << "  " << user_value[i].second << endl;
	}*/
	set<int>::iterator sit;
	possible_user.rehash(131071);
	
	for(it = AllItem.begin(); it != AllItem.end(); it++){
		if((*it).second.link == 0) continue;

		//int number = 0;
		if(rand() % 500 != 0) continue;

		for(int i = 0; i < AllUser.size(); i++){
				/*user_value[number].first = i;
				user_value[number].second = compute_parameter(i, it->first);*/
				sorted_user_value[i].first = i;
				sorted_user_value[i].second = compute_parameter(i, it->first);

		}

		//sort

		sort(sorted_user_value, sorted_user_value + AllUser.size(), compare_function);

		//int possible_size = 10 * (*it).second.link;

		multimap<double, int> table;
		/*int owner_num = (*it).second.user.size();

		
		for(sit = (*it).second.user.begin(); sit != (*it).second.user.end(); sit++){
			int owner = *sit;
			double owner_value = compute_parameter(owner, it->first);

			table.insert(make_pair(owner_value, owner));
		}

		possible_size -= owner_num;*/

		for(int i = 0; i < AllUser.size(); i++){
			//sit = it->second.user.find(sorted_user_value[i].first);
			//if(sit == it->second.user.end()){
				table.insert(make_pair(sorted_user_value[i].second, sorted_user_value[i].first));
			//}
			//else{
			//	possible_size++;
			//}
		}
		cerr << it->first << endl;
		possible_user.insert(make_pair((*it).first, table));
		table.clear();
	}
	TABLE::iterator uit;

	/*for(uit = possible_user.begin(); uit != possible_user.end(); uit++){
		int iiii = uit->first;
		it = AllItem.find(iiii);
		int link_num = it->second.link;
		aaaa += link_num;
		dddd += uit->second.size();
		if(uit->second.size() != 10 * link_num) {
			cout << uit->second.size() << " " << it->second.user.size() << " " << link_num << endl;
		}
	}*/
	multimap<double, int>::iterator mit;
	double accum_delta_theta[8];
	double pre_accum_delta_theta[8];
	double proportion[8];
	count = 0;
	bool converge = 0;
	while(converge == 0 || count < 10){
		converge = 1;
		for(int i = 0; i < f_n; i++){
			accum_delta_theta[i] = 0;
		}

		//compute delta theta
		for(uit = possible_user.begin(); uit != possible_user.end(); uit++){
			//uit->first : item's id
			it = AllItem.find(uit->first);
			int link_num = it->second.link;

			//find upper avg
			double upper_avg[8] = {0};
			double lower_avg[8] = {0};
			mit = uit->second.end();
			mit--;
			for(int i = 0; i < link_num; i++, mit--){
				upper_avg[0] += log(f1(mit->second, uit->first));
				upper_avg[1] += log(f2(mit->second, uit->first));
				
				upper_avg[2] += log(f3(mit->second, uit->first));
				
				upper_avg[3] += log(f4(mit->second, uit->first));
				upper_avg[4] += log(f5(mit->second, uit->first));
				upper_avg[5] += log(f6(mit->second, uit->first));

			}
			//normalize
			for(int i = 0; i < f_n; i++){
				upper_avg[i] /= (double)link_num;
			}

			int possible_size = uit->second.size();

			//find lower_avg

			for(int i = link_num; i < possible_size; i++, mit--){
				lower_avg[0] += log(f1(mit->second, uit->first));
				lower_avg[1] += log(f2(mit->second, uit->first));
				lower_avg[2] += log(f3(mit->second, uit->first));
				lower_avg[3] += log(f4(mit->second, uit->first));
				lower_avg[4] += log(f5(mit->second, uit->first));
				lower_avg[5] += log(f6(mit->second, uit->first));
			}
			for(int i = 0; i < f_n; i++){
				lower_avg[i] /= ((double)(possible_size - link_num));
			}
			/*for(int i = 0; i < 3; i++){
				cout << upper_avg[i] << " " << lower_avg[i] << endl;
			}*/
			compute_delta_theta(upper_avg, lower_avg);
			/*for(int i = 0; i < 3; i++){
				theta[uit->first][i] += 0.003 * delta_theta[i];
			}*/
			for(int i = 0; i < f_n; i++){
				accum_delta_theta[i] += delta_theta[i];
			}
		}

		for(int i = 0; i < f_n; i++){
			if(pre_accum_delta_theta[i] != accum_delta_theta[i]){
				converge = 0;
				count = 0;
			}
			pre_accum_delta_theta[i] = accum_delta_theta[i];
		}

		for(int i = 0; i < f_n; i++){
			if(pre_accum_delta_theta[i] != accum_delta_theta[i]){
				converge = 0;
				count = 0;
			}
			pre_accum_delta_theta[i] = accum_delta_theta[i];
		}
		if(converge == 1){
			count++;
		}
		cout << count << endl;
		/*for(int i = 0; i < 3; i++){
			cout << log(accum_delta_theta[i]) << " ";
		}
		cout << endl;*/
		/*for(int i = 0; i < 3; i++){
			cout << theta[i] << " ";
		}
		cout << endl;*/
		//update theta
		double max = -1;

		for(int i = 0; i < f_n; i++){
			proportion[i] = 1 / theta[i];
			if(fabs(proportion[i]) > max){
				max = fabs(proportion[i]);
			}
		}

		double expand = 1 / max;

		for(int i = 0; i < f_n; i++){
			proportion[i] *= expand;
		}

		for(int i = 0; i < f_n; i++){
			cout << theta[i] << " ";
		}
		cout << endl;

		for(int i = 0; i < f_n; i++){
			cout << proportion[i] << " ";
		}
		cout << endl;

		for(int i = 0; i < f_n; i++){
			theta[i] += proportion[i] * accum_delta_theta[i];
		}
		//compute new score
		for(uit = possible_user.begin(); uit != possible_user.end(); uit++){
			mit = uit->second.begin();

			multimap<double, int> table;

			for(; mit != uit->second.end(); mit++){
				sit = AllUser[mit->second].item.find(uit->first); 
				int value; 
				if(sit != AllUser[mit->second].item.end()){
					value = compute_parameter(mit->second, uit->first);
				}else{
					value = compute_parameter(mit->second, uit->first);
				}

				table.insert(make_pair(value, mit->second));
			}

			uit->second.clear();
			uit->second = table;
		}
	}
	for(int i = 0; i < f_n; i++){
		cout << theta[i] << " ";
	}

	cout << endl;
	clear impossible user
	/*for(uit = possible_user.begin(); uit != possible_user.end(); uit++){
		//uit->first : item's id
		it = AllItem.find(uit->first);
		int link_num = it->second.link;
		mit = uit->second.end();
		mit--;
		for(int i = 0; i < link_num; i++, mit--){
		}
		mit++;
		uit->second.erase(uit->second.begin(), mit);
		//cout << uit->second.size() << " " << link_num << endl;
	}*/
	/*for(uit = possible_user.begin(); uit != possible_user.end(); uit++){
		mit = uit->second.begin();
		cout << uit->first << endl;
		for(; mit != uit->second.end(); mit++){
			cout << mit->first << " " << mit->second << endl;
		}
	}*/


	count = 0;
	count2 = 0;
	fin3.open("pred.id", ios::in);

	multimap<double, pair<int, int>> possible_score;
	while (!fin3.eof()){
		fin3 >> a >> b >> cc;
		
		Y[count][0] = a;
		Y[count][1] = b;
		sit = AllUser[a].item.find(b);
		if(sit == AllUser[a].item.end()){
			pair<int, int> aa = make_pair(a, b);
			//pair<double, pair<int, int>> bb = make_pair(AllUser[a].item.size() * theta[b][0] + AllUser[a].relation.size() * theta[b][1], aa);
			possible_score.insert(make_pair(compute_parameter(a, b), aa));
		}else{
			pair<int, int> aa = make_pair(a, b);
			//pair<double, pair<int, int>> bb = make_pair(AllUser[a].item.size() * theta[b][0] + AllUser[a].relation.size() * theta[b][1] + 1 * theta[b][2], aa);
			possible_score.insert(make_pair(compute_parameter(a, b), aa));
		}
		//check if the user like the item
		/*uit = possible_user.find(b);

		if(uit == possible_user.end()){
			Y[count][2] = 0;
		}else{
			mit = uit->second.begin();
			Y[count][2] = 0;
			for(; mit != uit->second.end(); mit++){
				if(mit->second == a){
					Y[count][2] = 1;
					break;
				}
			}
		}
*/
		count++;
	}
	multimap<double, pair<int, int>>::iterator xx = possible_score.end();
	xx--;

	/*for(int i = 0; i < possible_score.size();i++, xx--){
		cout << xx->first << " " << xx->second.first << " " << xx->second.second << endl;
	}*/

	for(int j = 0; j < count; j++, xx--){
		for(int i = 0; i < count; i++){
			if(Y[i][0] == xx->second.first && Y[i][1] == xx->second.second){
				if(j <= count / 2){
					Y[i][2] = 1;

				}else{
					Y[i][2] = 0;
				}

				break;
			}
		}
	}

	/*for(int i = 0; i < count; i++){
		cout << Y[i][0] << " " << Y[i][1] << " " << Y[i][2] << endl;
	}*/

	fin4.open("response.txt", ios::in);
	while (!fin4.eof()){
		fin4 >> response[count2][0] >> response[count2][1];
		count2++;
	}
	int correct = 0;
	int pre_correct = 0;
	for (int i = 0; i < count; i++){
		int exit = 0;
		for (int j = 0; j < count2; j++){
			if (Y[i][0] == response[j][0] && Y[i][1] == response[j][1]){
				exit = 1;
				if (Y[i][2] == 1){
					pre_correct++;
					correct++;
				}
				break;
			}
		}
		if (exit == 0){
			if (score[i] == 0)
				correct++;
		}
	}
	cerr << pre_correct << endl;
	cerr << (float)correct / (float)count << endl;
	//system("pause");
	return 0;
}