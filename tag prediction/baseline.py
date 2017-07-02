import rake
import sys
import operator
import csv
import re
from collections import Counter
def load_words(stop_word_file):
    """
    Utility function to load stop words from a file and return as a list of words
    @param stop_word_file Path and file name of a file containing stop words.
    @return list A list of stop words.
    """
    Dict_words = {}
    for line in open(stop_word_file):
        for word in line.split():  # in case more than one per line
            Dict_words[word] = 1
    return Dict_words


input_file = sys.argv[1]
out_name = sys.argv[2]

noun_name = "91K nouns.txt"
stoplist_name = "SmartStoplist.txt"

noun_dict = load_words(noun_name)
stoplist_dict = load_words(stoplist_name)
sentence_delimiters = re.compile(u'[\\[\\]\n.!?,;:\t\\-\\"\\(\\)\\\'\u2019\u2013]')

title_dict={}
content_dict={}


with open(out_name, 'wb') as p:
    with open(input_file, 'rb') as f:
        reader = csv.DictReader(f)
        writer = csv.writer(p)
        line = ["id", "tags"]
        writer.writerow(line)
        for row in reader:
            tags = Counter()
            title_phrases = sentence_delimiters.split(row["title"])
            for phrase in title_phrases:
                words = phrase.split()
                for word in words:
                    word = word.lower()
                    if(stoplist_dict.has_key(word)):
                        continue
                    #if(noun_dict.has_key(word)):
                        #title_dict[word] = 1
                    tags[word] += 1

            content_phrases = sentence_delimiters.split(row["content"])
            for phrase in content_phrases:
                words = phrase.split()
                for word in words:
                    word = word.lower()
                    if(stoplist_dict.has_key(word)):
                        continue
                    tags[word] += 1
                    # if(noun_dict.has_key(word)):
                        
                    #     if(title_dict.has_key(word)):
                    #         tags[word] += 1
                    #         #tags += word.encode('utf-8')
                    #         #tags += " "
                    #     else:
                    #         if(content_dict.has_key(word)):
                    #             tags[word] += 1

                    #     content_dict[word] = 1

            sorted_tags = tags.most_common(len(tags))
            tags_string = ""
            for word, num in sorted_tags:
                if(num > 2):
                    if(word.isalpha()):
                        tags_string += word
                        tags_string += " "
            title_dict.clear()
            line = [row["id"], tags_string]
            writer.writerow(line)

	        	