import urllib
import json
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
import csv
import numpy as np


teamstats = csv.reader(open('teamstats2015.csv', 'rb')) 
                                 # first line which is a header
data=[]                          # Create a variable called 'data'.
for row in teamstats:      # Run through each row in the csv file,
    data.append(row)       # adding each row to the data variable
data = np.array(data) 	

name1 = data[0::, 1]

for i in range(name1.size):
	string = name1[i]
	if (string[len(string) - 1] == '*'):
		name1[i] = string[:-1]


#np.savetxt('test.csv',data, delimiter=',',fmt="%s")


opstats = csv.reader(open('opstats2015.csv', 'rb'))
data2=[]                          # Create a variable called 'data'.
for row in opstats:      # Run through each row in the csv file,
    data2.append(row)       # adding each row to the data variable
data2 = np.array(data2) 

name2 = data2[0::, 1]

for i in range(name2.size):
	string = name2[i]
	if (string[len(string) - 1] == '*'):
		name2[i] = string[:-1]

out = np.chararray((31, 3), 25)

for i in range(name1.size):
	for j in range(name2.size):
		if (name2[j] == name1[i]):
			out[i, 0] = name1[i]
			out[i, 1] = data[i, 25]
			out[i, 2] = data2[j, 25]

#np.savetxt('out.csv', out, delimiter=',',fmt="%s")

'''win = csv.reader(open('win2015.csv', 'rb'))
data3=[]                          # Create a variable called 'data'.
for row in win:      # Run through each row in the csv file,
    data3.append(row)       # adding each row to the data variable
data3 = np.array(data3) 

for i in range(len(out)):
	if (i == 0):
		out[i, 3] = "WIN"
	for j in range(len(data3)):
		if (out[i, 1] == data3[j, 1]):
			out[i, 3] = data3[j, 0]
			#if (out[i, 2] == data3[j, 2]):

'''
np.savetxt('out.csv', out, delimiter=',',fmt="%s")