import re
import math
import csv
import sys
from collections import defaultdict, OrderedDict
from sklearn.metrics import f1_score
def f1(x, y):
    score = f1_score(x, y, average='binary')
    return score

def get_words(text):
    word_split = re.compile('[^a-zA-Z0-9_\\+\\-/]')
    return [word.strip().lower() for word in word_split.split(text)]

def main(input):
    y_true = []
    y_pred = []
    alltag = []
    truetag = []
    predtag = []

    in1_file = open("ansv2_modify.csv")
    #in2_file = open("cooking.textrank.csv"
    testname = input
    in2_file = open(testname)
    reader1 = csv.DictReader(in1_file)
    reader2 = csv.DictReader(in2_file)
    te = 0
    for row1 in reader1:
        doc = int(row1["id"])
        tags = row1["tags"]
        alltag.append([])
        truetag.append([])
        for word in get_words(tags):
            if word not in alltag[te] and word.isalpha():
                alltag[te].append(word)
            truetag[te].append(word)
        te += 1

    te = 0
    for row2 in reader2:
        doc = int(row2["id"])
        tags = row2["tags"]
        predtag.append([])
        for word in get_words(tags):
            if word not in alltag[te] and word.isalpha():
                alltag[te].append(word)
            predtag[te].append(word)
        te += 1

    #print(predtag[0])
    score = 0.
    for n in range(te):
        del y_true[:]
        del y_pred[:]
        for true in alltag[n]:
            if true in truetag[n]:
                y_true.append(1)
            else:
                y_true.append(0)
            if true in predtag[n]:
                y_pred.append(1)
            else:
                y_pred.append(0)
       # print (y_true)
       # print (y_pred)
        score += f1(y_true, y_pred)
    
    score = score/te 
    print(score)



testname = sys.argv[1]

for i in range(0, 4):
    if (i == 0):
        print ("f1 score: {}".format(i))
        main(testname + ".csv")
    else:
        print ("f1 score: {}".format(i))
        main(testname + "_" + str(i) + ".csv")

