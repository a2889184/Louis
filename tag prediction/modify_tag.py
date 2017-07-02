import rake
import sys
import operator
import csv
import re

input_file = sys.argv[1]
out_name = sys.argv[2]
tag_num = sys.argv[3]
with open(out_name, 'wb') as p:
	with open(input_file, 'rb') as f:
		reader = csv.DictReader(f)
		writer = csv.writer(p)
		line = ["id", "tags", "num"]
		word_delimiters = re.compile(u' ')
		writer.writerow(line)
	    	for row in reader:
	    		#print (row["id"])
	    		words = word_delimiters.split(row["tags"])

	    		if (row["num"] < tag_num):
	    			line = [row["id"], row["tags"], row["num"]]
	    		else:
	    			tag = ""
		    		for i in range(0, int(tag_num)):
		    			tag += words[i]
		    			tag += " "
		    		line = [row["id"], tag, tag_num]

	        	writer.writerow(line)