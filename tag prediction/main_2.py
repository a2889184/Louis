import re
import csv
import math
from collections import defaultdict, OrderedDict, Counter
from gensim.models import Word2Vec, LdaModel
from gensim import corpora
from math import log10

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

temp = []
allword = Counter()
model = Word2Vec.load_word2vec_format('cooking_trans.bin', binary=True)

def clean_html(raw_html):
    cleanr = re.compile('<.*?-/,>')
    #cleanr = re.compile('<>')
    cleantext = re.sub(cleanr, '', raw_html)
    return cleantext


def get_words(text):
    word_split = re.compile('[^a-zA-Z0-9_\\+\\-/]')
    return [word.strip().lower() for word in word_split.split(text)]

def get_tags(text):
    word_split = re.compile(' ')
    return [word.strip().lower() for word in word_split.split(text)]

def process_text(doc, idf, text):
    tf = OrderedDict()
    word_count = 0.
    del temp[:]
    for word in get_words(text):
        if word not in stop_words and word.isalpha():
            temp.append(word)
            if word not in tf:
                tf[word] = 0
            tf[word] += 1
            idf[word].add(doc)
            word_count += 1.
            allword[word] += 1

    for word in tf:
        tf[word] = tf[word] / word_count

    return tf, word_count

def word2vec(text):
   # model = Word2Vec.load_word2vec_format('cooking.bin', binary=True)
    return model.similar_by_word(text, topn=200)

def search(word, wordlist):
    for pair in wordlist:
	if pair[0] == word:
	    return False
    return True

def main():
    in_file = open("cooling_modify_train_trans.csv")
    in1_file = open("ans.csv")
    in2_file = open("no_http_trans.pred_5.csv")
    out_file = open("word2vec_combine_test1.csv", "w")
    out1_file = open("all_string_trans.txt", "w")
    out2_file = open("ans_modify.csv", "w")
    
    reader = csv.DictReader(in_file)
    reader1 = csv.DictReader(in1_file)
    reader2 = csv.DictReader(in2_file)
    writer = csv.writer(out_file)
    writer.writerow(['id', 'tags'])
    writer1 = csv.writer(out2_file)
    writer1.writerow(['id', 'tags', 'num'])

    docs = []
    nums = []

    # Calculate TF and IDF per document
    idf = defaultdict(set)
    tf = {}
    word_counts = defaultdict(float)
    difftag = []

    print("Counting words..")
    
    for row in reader1:
	num = int(row['num'])
	nums.append(num)
    
    for row in reader:
        count = 0
        doc = int(row['id'])
        docs.append(doc)
		
        text = clean_html(row["title"]) + ' ' + clean_html(row["content"])
        tf[doc], word_counts[doc] = process_text(doc, idf, text)
#	out1_file.write(str(doc) + " ")
	for word in temp:
	    out1_file.write(word +" ")
	for word in get_words(row["tags"]):
	    count += 1
	writer1.writerow([doc, row["tags"], count-1])
    
    te = 0
    for row in reader2:
	tags = row["tags"]
	difftag.append([])
	for word in get_tags(tags):
	    if word.isalpha():
	    	difftag[te].append(word)
	te += 1

       # for word in get_words(row["tags"]):
        #    count+= 1
        #writer1.writerow([doc, row["tags"], count])
    in_file.close()
    in1_file.close()

    # Calculate TF-IDF
    nr_docs = len(docs)
    for doc in docs:

        for word in tf[doc]:
            tf[doc][word] *= math.log(nr_docs / len(idf[word]))

    # Write predictions
    print("Writing predictions..")
    answer = []
    finalanswer = []
   # maxvalue = allword[max(allword, key=allword.get)]
    total = len(allword)
   # sortword = OrderedDict(sorted(allword.items(), key=lambda t: t[0], reverse=True)[:int(total*0.005)])
    sortword = allword.most_common(total)
    band = sortword[int(total*0.05)][1]
    print band
    count = 0
    for doc in docs:
	del answer [:]
	del finalanswer [:]
#	print doc
#	print difftag[count]
        # Sort words with frequency from high to low.
#        pred_tags = sorted(tf[doc], key=tf[doc].get, reverse=True)[:1]
	pred_tags = difftag[count]
	new_tags = []
#	print pred_tags
	#for n in range(1,int(len(pred_tags)/2)+2):
	
#	while (len(pred_tags)>1):
#	delete = model.doesnt_match(pred_tags)
#	pred_tags.remove(delete)

#        print pred_tags
        # Write predictions
	for topword in pred_tags:
#	    if search(topword, answer):
	    answer.append((topword, allword[topword]))
	   # answer.append((topword, 1))
#	    if(topword not in finalanswer):
#	    finalanswer.append(topword)
	    similar = word2vec(topword)
	    n = 0
	    first = 0
	#    print similar
	    for findword in similar:
	        if (allword[findword[0]] > band and n < 15):
		    word = findword[0].encode("utf-8")
	#	    print word
		 #   if(word not in finalanswer and first >= 2):
		#	finalanswer.append(word)
		#	n += 1
		  #  first += 1
		  #  print topword
		    if search(word, answer) and (first >= 0):
		    	answer.append((word, findword[1] * allword[topword]))
			n += 1
		    first += 1
	#	    answer.append((findword[0], findword[1]))
#	    if (len(answer) == 0):
#		answer.append((topword, tf[doc][topword] * allword[topword]))
#	print answer
	answer = sorted(answer, key=lambda final: final[1], reverse=True)[:nums[count] + 2]
	for item in answer:
	    finalanswer.append(item[0])
#	print finalanswer
#	while (len(finalanswer)>3):
#	if(len(finalanswer)>2):
#	    for n in range(1):
 #  	        delete = model.doesnt_match(finalanswer)
#	        finalanswer.remove(delete)
	#:nums[count]*2
#	print finalanswer
        writer.writerow([doc, " ".join(sorted(finalanswer))])
	count += 1
    in_file.close()
    #out_file.close()
    out1_file.close()
    out2_file.close()


if __name__ == "__main__":
    print("Starting program.")
    main()
