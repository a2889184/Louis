import rake
import sys
import operator
import csv
import re
from gensim.models import Word2Vec
import enchant
from collections import defaultdict, OrderedDict, Counter
import main_2
import math

model = Word2Vec.load_word2vec_format('cooking_trans.bin', binary=True)
stop_words = {'a', "a's", 'able', 'about', 'above', 'according', 'accordingly',
              'across', 'actually', 'after', 'afterwards', 'again', 'against',
              "ain't", 'all', 'allow', 'allows', 'almost', 'alone', 'along',
              'already', 'also', 'although', 'always', 'am', 'among', 'amongst',
              'an', 'and', 'another', 'any', 'anybody', 'anyhow', 'anyone',
              'anything', 'anyway', 'anyways', 'anywhere', 'apart', 'appear',
              'appreciate', 'appropriate', 'are', "aren't", 'aren', 'around', 'as',
              'aside', 'ask', 'asking', 'associated', 'at', 'available', 'away',
              'awfully', 'b', 'be', 'became', 'because', 'become', 'becomes',
              'becoming', 'been', 'before', 'beforehand', 'behind', 'being',
              'believe', 'below', 'beside', 'besides', 'best', 'better',
              'between', 'beyond', 'both', 'brief', 'but', 'by', 'c', "c'mon",
              "c's", 'came', 'can', "can't", 'cannot', 'cant', 'cause',
              'causes', 'certain', 'certainly', 'changes', 'clearly', 'co',
              'com', 'come', 'comes', 'concerning', 'consequently', 'consider',
              'considering', 'contain', 'containing', 'contains',
              'corresponding', 'could', "couldn't", 'couldn', 'course', 'currently', 'd',
              'definitely', 'described', 'despite', 'did', "didn't", 'didn',
              'different', 'do', 'does', "doesn't", 'doesn', 'doing', "don't", 'don', 'done',
              'down', 'downwards', 'during', 'e', 'each', 'edu', 'eg', 'eight',
              'either', 'else', 'elsewhere', 'enough', 'entirely', 'especially',
              'et', 'etc', 'even', 'ever', 'every', 'everybody', 'everyone',
              'everything', 'everywhere', 'ex', 'exactly', 'example', 'except',
              'f', 'far', 'few', 'fifth', 'first', 'five', 'followed',
              'following', 'follows', 'for', 'former', 'formerly', 'forth',
              'four', 'from', 'further', 'furthermore', 'g', 'get', 'gets',
              'getting', 'given', 'gives', 'go', 'goes', 'going', 'gone', 'got',
              'gotten', 'greetings', 'h', 'html', 'http', 'had', "hadn't", 'hadn', 'happens', 'hardly',
              'has', "hasn't", 'have', "haven't", 'haven', 'having', 'he', "he's",
              'hello', 'help', 'hence', 'her', 'here', "here's", 'hereafter',
              'hereby', 'herein', 'hereupon', 'hers', 'herself', 'hi', 'him',
              'himself', 'his', 'hither', 'hopefully', 'how', 'howbeit',
              'however', 'i', "i'd", "i'll", "i'm", "i've", 've', 'ie', 'if',
              'ignored', 'immediate', 'in', 'inasmuch', 'inc', 'indeed',
              'indicate', 'indicated', 'indicates', 'inner', 'insofar',
              'instead', 'into', 'inward', 'is', "isn't", 'it', "it'd", "it'll", 'll',
              "it's", 'its', 'itself', 'j', 'just', 'k', 'keep', 'keeps',
              'kept', 'know', 'knows', 'known', 'l', 'last', 'lately', 'later',
              'latter', 'latterly', 'least', 'less', 'lest', 'let', "let's",
              'like', 'liked', 'likely', 'little', 'look', 'looking', 'looks',
              'ltd', 'm', 'mainly', 'many', 'may', 'maybe', 'me', 'mean',
              'meanwhile', 'merely', 'might', 'more', 'moreover', 'most',
              'mostly', 'much', 'must', 'my', 'myself', 'n', 'name', 'namely',
              'nd', 'near', 'nearly', 'necessary', 'need', 'needs', 'neither',
              'never', 'nevertheless', 'new', 'next', 'nine', 'no', 'nobody',
              'non', 'none', 'noone', 'nor', 'normally', 'not', 'nothing',
              'novel', 'now', 'nowhere', 'o', 'obviously', 'of', 'off', 'often',
              'oh', 'ok', 'okay', 'old', 'on', 'once', 'one', 'ones', 'only',
              'onto', 'or', 'other', 'others', 'otherwise', 'ought', 'our',
              'ours', 'ourselves', 'out', 'outside', 'over', 'overall', 'own',
              'p', 'particular', 'particularly', 'per', 'perhaps', 'placed',
              'please', 'plus', 'possible', 'presumably', 'probably',
              'provides', 'q', 'que', 'quite', 'qv', 'r', 'rather', 'rd', 're',
              'really', 'reasonably', 'regarding', 'regardless', 'regards',
              'relatively', 'respectively', 'right', 's', 'said', 'same', 'saw',
              'say', 'saying', 'says', 'second', 'secondly', 'see', 'seeing',
              'seem', 'seemed', 'seeming', 'seems', 'seen', 'self', 'selves',
              'sensible', 'sent', 'serious', 'seriously', 'seven', 'several',
              'shall', 'she', 'should', "shouldn't", 'since', 'six', 'so',
              'some', 'somebody', 'somehow', 'someone', 'something', 'sometime',
              'sometimes', 'somewhat', 'somewhere', 'soon', 'sorry',
              'specified', 'specify', 'specifying', 'still', 'sub', 'such',
              'sup', 'sure', 't', "t's", 'take', 'taken', 'tell', 'tends', 'th',
              'than', 'thank', 'thanks', 'thanx', 'that', "that's", 'thats',
              'the', 'their', 'theirs', 'them', 'themselves', 'then', 'thence',
              'there', "there's", 'thereafter', 'thereby', 'therefore',
              'therein', 'theres', 'thereupon', 'these', 'they', "they'd",
              "they'll", "they're", "they've", 'think', 'third', 'this',
              'thorough', 'thoroughly', 'those', 'though', 'three', 'through',
              'throughout', 'thru', 'thus', 'to', 'together', 'too', 'took',
              'toward', 'towards', 'tried', 'tries', 'truly', 'try', 'trying',
              'twice', 'two', 'u', 'un', 'under', 'unfortunately', 'unless',
              'unlikely', 'until', 'unto', 'up', 'upon', 'us', 'use', 'used',
              'useful', 'uses', 'using', 'usually', 'uucp', 'v', 'value',
              'various', 'very', 'via', 'viz', 'vs', 'w', 'want', 'wants',
              'was', "wasn't", 'wasn', 'way', 'we', "we'd", "we'll", "we're", 've', "we've",
              'welcome', 'well', 'went', 'were', "weren't", 'what', "what's",
              'whatever', 'when', 'whence', 'whenever', 'where', "where's",
              'whereafter', 'whereas', 'whereby', 'wherein', 'whereupon',
              'wherever', 'whether', 'which', 'while', 'whither', 'who',
              "who's", 'whoever', 'whole', 'whom', 'whose', 'why', 'will',
              'willing', 'wish', 'with', 'within', 'without', "won't", 'won', 'wonder',
              'would', 'would', "wouldn't", 'wouldn', 'x', 'y', 'yes', 'yet', 'you',
              "you'd", "you'll", "you're", "you've", 'your', 'yours',
              'yourself', 'yourselves', 'z', 'zero', ''}

d = enchant.Dict("en_US")
in_file = open("cooling_modify_train_trans.csv")
reader = csv.DictReader(in_file)
 
allword = Counter()
for row in reader:
    
	text = row["title"] + ' ' + row["content"]
	for word in main_2.get_words(text):
		if word not in stop_words and word.isalpha():
			allword[word] += 1

input_file = sys.argv[1]
out_name = sys.argv[2]
with open(out_name, 'wb') as p:
	with open(input_file, 'rb') as f:
		reader = csv.DictReader(f)
		writer = csv.writer(p)
		line = ["id", "tags"]
		word_delimiters = re.compile(u' ')
		writer.writerow(line)
		for row in reader:
			words = word_delimiters.split(row["tags"])
			tags = ""
			print (row["id"])
			if(len(words) > 3):
				tttttt = model.most_similar(words[0:len(words) - 2], topn = len(allword) - 1)
				vvv = Counter()
				for word, val in tttttt:
					word = word.encode('utf-8');
					vvv[word] = math.exp(8 * val) * math.log(0.05*(allword[word] + 1))

				s = vvv.most_common(20)


				
				for word in words:
					tags += (word + ' ')

				tags += s[1][0]
				words = word_delimiters.split(tags)

			else:
				tags = row["tags"]
			line = [row["id"], tags]
			writer.writerow(line)