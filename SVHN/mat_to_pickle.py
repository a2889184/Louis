import scipy.io as sio
import sys
import pickle

f = list()

l = sys.argv
l.pop(0)

print (l)

for ll in l:
	f.append(sio.loadmat(ll))

d = list()

for ff in f:
	d.append(ff['digitStruct'])

del f[:]

# d[0] should be train_data
label_dict = dict()

label_dict['train_labels'] = list()
label_dict['test_labels'] = list()
label_dict['extra_labels'] = list()


for partial_data in d[0][0]:
	
	# partiacl_data is one piece of data of a .mat file
	# partial_data[0] is 'name' of the file, partial_data[1] is the number information of this file
	for i in partial_data[1]['label']:
		# i concludes a list of all labels in this piece of data
		number = 0
		for j in i:
			# j[0][0] is one of labels
			number = 10 * number + (j[0][0] % 10)

		label_dict['train_labels'].append(number)


for partial_data in d[1][0]:
	# partiacl_data is one piece of data of a .mat file
	# partial_data[0] is 'name' of the file, partial_data[1] is the number information of this file
	for i in partial_data[1]['label']:
		# i concludes a list of all labels in this piece of data
		number = 0
		for j in i:
			# j[0][0] is one of labels
			number = 10 * number + (j[0][0] % 10)

		label_dict['test_labels'].append(number)


for partial_data in d[2][0]:
	# partiacl_data is one piece of data of a .mat file
	# partial_data[0] is 'name' of the file, partial_data[1] is the number information of this file
	for i in partial_data[1]['label']:
		# i concludes a list of all labels in this piece of data
		number = 0
		for j in i:
			# j[0][0] is one of labels
			number = 10 * number + (j[0][0] % 10)

		label_dict['extra_labels'].append(number)


with open('labels.pickle', 'w') as o:
	pickle.dump(label_dict, o, -1)




