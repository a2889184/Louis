import urllib
import json
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
import csv
import numpy as np
import sys
from matplotlib.mlab import PCA
from mpl_toolkits.mplot3d import Axes3D
from copy import copy

def read_file(Filename1):
	df = pd.read_csv(Filename1, header = 0)
	df = df.drop(['G', 'MP'], axis = 1)
	return df

def normalize_data(DataFrame):
	for i in range(2, DataFrame.shape[1]):
		column = DataFrame.columns.values[i]
		DataFrame[column] = DataFrame[column].astype(float)
		std_column = DataFrame[column].std()
		mean_column = DataFrame[column].mean()
		DataFrame[column] = (DataFrame[column] - mean_column) / std_column

	return DataFrame

def modify_standing(DataFrame):
	for i in range(DataFrame.shape[0]):
		string = list(DataFrame['Team'][i])
		while(string[-1] != '*' and (string[-1] < 'a' or string[-1] > 'z')):
			string.pop()
		DataFrame['Team'][i] = ''.join(string)

	return DataFrame

def concatenate_data(DataFrame1, DataFrame2, change=1):
	# index mapping
	
	t = np.arange(0, DataFrame1.shape[0])

	for i in range(DataFrame1.shape[0]):
		for j in range(DataFrame2.shape[0]):
			if (DataFrame1['Team'][i] == DataFrame2['o_Team'][j]):
				t[i] = j

	DataFrame2 = DataFrame2.drop(['o_Team'], axis = 1)
	temp_DataFrame = DataFrame2.reindex(t)
	temp_DataFrame = temp_DataFrame.set_index(np.arange(0, DataFrame1.shape[0]))

	DataFrame = pd.concat([DataFrame1, temp_DataFrame], axis = 1)
	

	if (change == 1):
	
		temp_name = copy(DataFrame.columns.values)
		temp_name[0], temp_name[1] = temp_name[1], temp_name[0]

		# DataFrame = DataFrame[temp_name]
		
		# DataFrame.columns = temp_name

		temp_name[2], temp_name[23] = temp_name[23], temp_name[2]

		DataFrame = DataFrame[temp_name]
		DataFrame.columns = temp_name
		
	elif (change == 0):
		temp_name = copy(DataFrame.columns.values)
		temp_name[3], temp_name[DataFrame.shape[1] - 1] = temp_name[DataFrame.shape[1] - 1], temp_name[3]
		DataFrame = DataFrame[temp_name]
		DataFrame.columns = temp_name


	return DataFrame



# read team and opponent stats
df1, df2 = read_file(sys.argv[1]), read_file(sys.argv[2])
df1, df2 = normalize_data(df1), normalize_data(df2)

df3 = pd.read_csv(sys.argv[3], header=0)
df3 = df3.drop(['W', 'L', 'GB', 'PS/G', 'PA/G', 'SRS'], axis=1)
df3 = modify_standing(df3)

temp_name = df3.columns.values[:]
temp_name[0] = 'o_' + temp_name[0]
df3.columns = temp_name

# add 'o_' to opponent stats title 
temp_name = 'o_' + df2.columns.values[:]
df2.columns = temp_name

concat_df = concatenate_data(df1, df2, 1)
concat_df = concatenate_data(concat_df, df3, 0)

tag = sys.argv[1][1:5] + '_'

concat_df['Team'] = tag + concat_df['Team']


concat_df.to_csv('normed_' + sys.argv[1][1:], index = False)
#normalize the data



# for i in range(len(title)):
# 	std_column = df1[title[i]].std()
# 	mean_column = df1[title[i]].mean()
# 	for j in range(len(df1)):
# 		df1[title[i]][j] = (df1[title[i]][j] - mean_column) / std_column

# data = df1.values
# results = PCA(data)

# x = []
# y = []
# z = []
# for item in results.Y:
# 	x.append(item[0])
# 	y.append(item[1])
# 	z.append(item[2])

# plt.close('all') # close all latent plotting windows
# fig1 = plt.figure() # Make a plotting figure
# ax = Axes3D(fig1) # use the plotting figure to create a Axis3D object.
# pltData = [x,y,z] 
# ax.scatter(pltData[0], pltData[1], pltData[2], 'bo') # make a scatter plot of blue dots from the data

# # make simple, bare axis lines through space:
# xAxisLine = ((min(pltData[0]), max(pltData[0])), (0, 0), (0,0)) # 2 points make the x-axis line at the data extrema along x-axis 
# ax.plot(xAxisLine[0], xAxisLine[1], xAxisLine[2], 'r') # ma3ke a red line for the x-axis.
# yAxisLine = ((0, 0), (min(pltData[1]), max(pltData[1])), (0,0)) # 2 points make the y-axis line at the data extrema along y-axis
# ax.plot(yAxisLine[0], yAxisLine[1], yAxisLine[2], 'r') # make a red line for the y-axis.
# zAxisLine = ((0, 0), (0,0), (min(pltData[2]), max(pltData[2]))) # 2 points make the z-axis line at the data extrema along z-axis
# ax.plot(zAxisLine[0], zAxisLine[1], zAxisLine[2], 'r') # make a red line for the z-axis.

# # label the axes 
# ax.set_xlabel("x-axis label") 
# ax.set_ylabel("y-axis label")
# ax.set_zlabel("z-axis label")
# ax.set_title("The title of the plot")
# plt.show() # show the plot


#np.savetxt('out.csv', out, delimiter=',',fmt="%s")