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

info = list()
for partial_data in d[0][0]:
	
	# partiacl_data is one piece of data of a .mat file
	# partial_data[0] is 'name' of the file, partial_data[1] is the number information of this file
	temp = 0
	number = 0
	for i in partial_data[1][0]:
		# i concludes a list of all labels in this piece of data
		
		if (temp == 0):
			info.append(int(i['left'][0][0]))
			info.append(int(i['top'][0][0]))

		if (temp == partial_data[1][0].shape[0] - 1):
			info.append(int(i['left'][0][0]) + int(i['width'][0][0]))
			info.append(int(i['top'][0][0]) + int(i['height'][0][0]))
		temp = temp + 1
			# j[0][0] is one of labels
		number = 10 * number + (i['label'][0][0] % 10)
		
	info.append(number)
	label_dict['train_labels'].append(info[:])
	del info[:]

for partial_data in d[1][0]:
	# partiacl_data is one piece of data of a .mat file
	# partial_data[0] is 'name' of the file, partial_data[1] is the number information of this file
	temp = 0
	number = 0
	for i in partial_data[1][0]:
		# i concludes a list of all labels in this piece of data
		
		if (temp == 0):
			info.append(int(i['left'][0][0]))
			info.append(int(i['top'][0][0]))
		if (temp == partial_data[1][0].shape[0] - 1):
			info.append(int(i['left'][0][0]) + int(i['width'][0][0]))
			info.append(int(i['top'][0][0]) + int(i['height'][0][0]))
			if (int(i['top'][0][0] < info[1])):
				info[3] = info[1] + int(i['height'][0][0])
				info[1] = int(i['top'][0][0])



			# j[0][0] is one of labels
		temp = temp + 1
		number = 10 * number + (i['label'][0][0] % 10)
		
	info.append(number)
	label_dict['test_labels'].append(info[:])
	del info[:]

# for partial_data in d[2][0]:
# 	# partiacl_data is one piece of data of a .mat file
# 	# partial_data[0] is 'name' of the file, partial_data[1] is the number information of this file
# 	for i in partial_data[1]['label']:
# 		# i concludes a list of all labels in this piece of data
# 		number = 0
# 		for j in i:
# 			# j[0][0] is one of labels
# 			number = 10 * number + (j[0][0] % 10)

# 		label_dict['extra_labels'].append(number)
for partial_data in d[2][0]:
	# partiacl_data is one piece of data of a .mat file
	# partial_data[0] is 'name' of the file, partial_data[1] is the number information of this file
	temp = 0
	number = 0
	for i in partial_data[1][0]:
		# i concludes a list of all labels in this piece of data
		
		if (temp == 0):
			info.append(int(i['left'][0][0]))
			info.append(int(i['top'][0][0]))
		if (temp == partial_data[1][0].shape[0] - 1):
			info.append(int(i['left'][0][0]) + int(i['width'][0][0]))
			info.append(int(i['top'][0][0]) + int(i['height'][0][0]))
			if (int(i['top'][0][0] < info[1])):
				info[3] = info[1] + int(i['height'][0][0])
				info[1] = int(i['top'][0][0])



			# j[0][0] is one of labels
		temp = temp + 1
		number = 10 * number + (i['label'][0][0] % 10)
		
	info.append(number)
	label_dict['extra_labels'].append(info[:])
	del info[:]


with open('labels_full.pickle', 'w') as o:
	pickle.dump(label_dict, o, -1)




