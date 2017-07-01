import pickle

from scipy import ndimage, misc
import scipy.io as sio
import numpy as np
import sys
import random


files = sys.argv
files.pop(0)

data_easy = dict()


index = 0

max_pixel = 255.0
for file in files:
	print ('process ' + file)
	d =  sio.loadmat(file)

	dataset = d['X']
	labels = d['y']

	for i in range(3, 0, -1):
		dataset = np.swapaxes(dataset, i - 1, i)

	dataset = (np.sum(dataset, 3) / 3.0 - max_pixel / 2) / max_pixel

	if (index == 0):
		index = index + 1
		all_dataset = dataset
		all_labels = labels

	else:
		all_dataset = np.concatenate((all_dataset, dataset), axis=0)
		all_labels = np.concatenate((all_labels, labels), axis=0)
	del d


validation_size = 10000
test_size = 10000

train_size = len(all_labels) - test_size - validation_size

# random_index = range(len(labels))
# random.shuffle(random_index)

# dataset = dataset[random_index]
# labels = labels[random_index]

data_easy['train_data'] = all_dataset[:train_size]
data_easy['train_labels'] = all_labels[:train_size].reshape(train_size)
data_easy['validation_data'] = all_dataset[train_size:train_size + validation_size] 
data_easy['validation_labels'] = all_labels[train_size:train_size + validation_size].reshape(validation_size)
data_easy['test_data'] = all_dataset[train_size + validation_size:]
data_easy['test_labels'] = all_labels[train_size + validation_size:].reshape(test_size)

with open('data_easy.pickle', 'w') as o:
	pickle.dump(data_easy, o, -1)




