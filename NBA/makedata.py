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

df1 = pd.read_csv(sys.argv[1], header = 0)

df1 = df1.drop(['Team', 'Rk', 'G', 'MP'], axis = 1)

df2 = pd.read_csv(sys.argv[2], header = 0)

df2 = df2.drop(['Team', 'Rk', 'G', 'MP'], axis = 1)

title = ['FG', 'FGA', 'FG%', '3P', '3PA', '3P%', '2P', '2PA', '2P%', 'FT', 'FTA', 'FT%', 'ORB', 'DRB', 'TRB',
 'AST', 'STL', 'BLK', 'TOV', 'PF', 'PTS', 'PS/G']

for i in range(len(title)):
	df1[title[i]] = df1[title[i]].astype(float)


#normalize the data



for i in range(len(title)):
	std_column = df1[title[i]].std()
	mean_column = df1[title[i]].mean()
	for j in range(len(df1)):
		df1[title[i]][j] = (df1[title[i]][j] - mean_column) / std_column

data = df1.values
results = PCA(data)

x = []
y = []
z = []
for item in results.Y:
	x.append(item[0])
	y.append(item[1])
	z.append(item[2])

plt.close('all') # close all latent plotting windows
fig1 = plt.figure() # Make a plotting figure
ax = Axes3D(fig1) # use the plotting figure to create a Axis3D object.
pltData = [x,y,z] 
ax.scatter(pltData[0], pltData[1], pltData[2], 'bo') # make a scatter plot of blue dots from the data

# make simple, bare axis lines through space:
xAxisLine = ((min(pltData[0]), max(pltData[0])), (0, 0), (0,0)) # 2 points make the x-axis line at the data extrema along x-axis 
ax.plot(xAxisLine[0], xAxisLine[1], xAxisLine[2], 'r') # make a red line for the x-axis.
yAxisLine = ((0, 0), (min(pltData[1]), max(pltData[1])), (0,0)) # 2 points make the y-axis line at the data extrema along y-axis
ax.plot(yAxisLine[0], yAxisLine[1], yAxisLine[2], 'r') # make a red line for the y-axis.
zAxisLine = ((0, 0), (0,0), (min(pltData[2]), max(pltData[2]))) # 2 points make the z-axis line at the data extrema along z-axis
ax.plot(zAxisLine[0], zAxisLine[1], zAxisLine[2], 'r') # make a red line for the z-axis.

# label the axes 
ax.set_xlabel("x-axis label") 
ax.set_ylabel("y-axis label")
ax.set_zlabel("z-axis label")
ax.set_title("The title of the plot")
plt.show() # show the plot


#np.savetxt('out.csv', out, delimiter=',',fmt="%s")