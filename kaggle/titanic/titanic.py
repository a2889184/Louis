import csv as csv
import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt
import pandas as pd


def pre(data):
	women = data[0::, 4] == "female"
	men = data[0::, 4] != "female"
	data[women, 4] = 1
	data[men, 4] = 0
	null_age = data[0::, 5] == ""
	data[null_age, 5] = 0


csv_file_object = csv.reader(open('train.csv', 'rb')) 
header = csv_file_object.next()  # The next() command just skips the 
                                 # first line which is a header
data=[]                          # Create a variable called 'data'.
for row in csv_file_object:      # Run through each row in the csv file,
    data.append(row)             # adding each row to the data variable
data = np.array(data) 	

pre(data)

print data[5]
