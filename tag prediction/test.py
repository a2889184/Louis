import rake
import sys
import operator
import csv
from pattern.en import pluralize, singularize, lemma
import polyglot
from polyglot.text import Text, Word

import re

input_name= sys.argv[1]
output_name = sys.argv[2]
contraint = sys.argv[3]

rake_object = rake.Rake("SmartStoplist.txt", 3, int(contraint), 1)

with open(output_name, 'w') as p:
	with open(input_name, 'r') as f:
		reader = csv.DictReader(f)
		writer = csv.writer(p)
		line = ["id", "tags", "num"]
		writer.writerow(line)
		for row in reader:
			text = row["title"] + " " + row["content"]
			keywords = rake_object.run(text)
			keyword_list = ""
			sizes = 0
			print (row['id'])
			if(len(keywords) > 4):
				sizes = 4
				for i in range(0, 4):
					keyword_list += keywords[i][0]
					keyword_list += " "
			else:
				sizes = len(keywords)
				for i in range(0, len(keywords)):
					keyword_list += keywords[i][0]
					keyword_list += " "

			line = [row["id"], keyword_list, sizes]
			writer.writerow(line)
			

#keywords = rake_object.run(train)
#print (keywords)
#text = Text("I want to eat something, but I failed.")

#for word, tag in text.pos_tags:
	#print (word)
   #print(u"{:<16}{:>2}".format(word, tag))
