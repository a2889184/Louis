import rake
import sys
import operator
import csv
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

verb_name = "31K verbs.txt"
noun_name = "91K nouns.txt"

verb_dict = load_words(verb_name)
noun_dict = load_words(noun_name)
with open(out_name, 'wb') as p:
	with open(input_file, 'rb') as f:
		reader = csv.DictReader(f)
		writer = csv.writer(p)
		line = ["id", "title", "content", "tags"]
		writer.writerow(line)
	    	for row in reader:
	        	line = [row["id"], rake.make_word_consistent(row["title"]), rake.make_word_consistent(row["content"]), rake.single_type(row["tags"], noun_dict, verb_dict)]
	        	writer.writerow(line)
