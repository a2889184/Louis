import rake
import sys
import operator
import csv
import enchant

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


def return_noun(input_text, counter, d, type_):
    modify_string = ""

    words = input_text.split()

    for word in words:
    	if(d.check(word)):
    		if (type_ == 1):
    			counter[word] += 1.5
    		elif (type_ == 2):
    			counter[word] += 1
    		else :
    			counter[word] += 0.5
        #if(noun_dict.has_key(word)):
            #modify_string += word
            #modify_string += " "
    return counter

input1_name = sys.argv[1]
input2_name = sys.argv[2]
input3_name = sys.argv[3]
input4_name = sys.argv[4]
output_name = sys.argv[5]

noun_name = "91K nouns.txt"
d = enchant.Dict("en_US")
noun_dict = load_words(noun_name)

tag_list = {}

counter = Counter()

text_rank = {}

with open(output_name, 'w') as p:
	writer = csv.writer(p)
	line = ["id", "tags", "nums"]
	writer.writerow(line)
	with open(input1_name, 'r') as f:
		reader = csv.DictReader(f)
		for row in reader:
			counter = Counter()
			tag_list[row["id"]] = return_noun(row["tags"], counter, d, 1)
			text_rank[row["id"]] = row["tags"]
	with open(input2_name, 'r') as f:
		reader = csv.DictReader(f)
		for row in reader:
			tag_list[row["id"]] = return_noun(row["tags"], tag_list[row["id"]], d, 2)
			# origin_tag = tag_list[row["id"]]
			# compute_tag = return_noun(row["tags"], noun_dict)
			# tag_list.update({row["id"]: (origin_tag + compute_tag)})
	with open(input3_name, 'r') as f:
		reader = csv.DictReader(f)
		for row in reader:
			tag_list[row["id"]] = return_noun(row["tags"], tag_list[row["id"]], d, 2)
			# origin_tag = tag_list[row["id"]]
			# compute_tag = return_noun(row["tags"], noun_dict)
			# tag_list.update({row["id"]: (origin_tag + compute_tag)})

	with open(input4_name, 'r') as f:
		reader = csv.DictReader(f)
		for row in reader:
			tag_list[row["id"]] = return_noun(row["tags"], tag_list[row["id"]], d, 3)
			# origin_tag = tag_list[row["id"]]
			# compute_tag = return_noun(row["tags"], noun_dict)
			# tag_list.update({row["id"]: (origin_tag + compute_tag)})
			sorted_tags = tag_list[row["id"]].most_common(len(tag_list[row["id"]]))
			tags = ""
			times = ""
			for word, num in sorted_tags:
				if(num > 2.5):
					tags += (word + " ")
					times += (str(num) + " ")
			if tags == "":
				words = text_rank[row["id"]].split()
				i = 0;
				for word in words:
					tags += (word + " ")
					i += 1
					if (i == 2) :
						break

			line = [row["id"], tags, times]
			writer.writerow(line)




