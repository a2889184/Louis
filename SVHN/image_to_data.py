import pickle
import os
import matplotlib.pyplot as plt
from PIL import Image, ImageEnhance
from scipy import ndimage, misc
import numpy as np
import sys
import random

folders = sys.argv
folders.pop(0)

with open('labels.pickle', 'r') as p:
	pickle_data = pickle.load(p)

	train_labels = pickle_data['train_labels']
	test_labels = pickle_data['test_labels']
	extra_labels = pickle_data['extra_labels']
	del pickle_data




pixel_max = 255.0
image_size = 28
num_image = 0

c_factor = 2.0
b_factor = 2.0
s_factor = 2.0 

dataset = np.ndarray(shape=(len(train_labels) + len(extra_labels) + len(test_labels), image_size, image_size), dtype=np.float32)
labels = np.ndarray(shape=(len(train_labels) + len(extra_labels) + len(test_labels)), dtype=np.int)

for folder in folders:
	image_files = os.listdir(folder)
	print ('start processing: ', folder, ' folder')
	# remove not .png files
	image_files.remove('see_bboxes.m'), image_files.remove('digitStruct.mat')

	for image in image_files:
		# get image index (note that image name is start from 1.png, but index is start from 0, so substract 1)

		image_index = int(image[:-4]) - 1
		image_file = os.path.join(folder, image)
		try:
			pil_im = Image.open(image_file).convert('L')
			# resize the image to 32 * 32
			# width, height = pil_im.size
			# left = width/3
			# top = height/4
			# right = width * 2/3
			# bottom = height *3/4
			# pil_im = pil_im.crop((left, top,right, bottom)) 
			pil_im = pil_im.resize((image_size, image_size), Image.ANTIALIAS)
			if pil_im.size != (image_size, image_size):
				raise Exception('Unexpected image shape: %s' % str(pil_im.size))
			# enhance the image by factor 2
			c_en = ImageEnhance.Contrast(pil_im)
			pil_im = c_en.enhance(c_factor)
			b_en = ImageEnhance.Brightness(pil_im)
			pil_im = b_en.enhance(b_factor)
			s_en = ImageEnhance.Sharpness(pil_im)
			pil_im = s_en.enhance(s_factor)

			# tranform the image to array and normalize
			image_array = (np.asarray(pil_im).astype(float) - (pixel_max / 2))

			dataset[num_image, :, :] = image_array
			if (folder == 'train' or folder == 'train/' or folder == './train' or folder == './train/'):
				labels[num_image] = train_labels[image_index]
			elif (folder == 'test' or folder == 'test/' or folder == './test' or folder == './test/'):
				labels[num_image] = test_labels[image_index]

			elif (folder == 'extra' or folder == 'extra/' or folder == './extra' or folder == './extra/'):
				labels[num_image] = extra_labels[image_index]
			num_image = num_image + 1
		except IOError as e:
			print('Could not read:', image_file, ':', e, '- it\'s ok, skipping.')


		# pil_im = Image.fromarray(image_array)

	print (labels[num_image - 11])
	temp_data = dataset[num_image - 11]
	temp_data = temp_data + (pixel_max / 2)
	temp_im = Image.fromarray(temp_data)
	temp_im.show()


validation_size = 20000
test_size = 20000

train_size = len(labels) - test_size - validation_size

random_index = range(len(labels))
random.shuffle(random_index)

dataset = dataset[random_index]
labels = labels[random_index]

data = dict()

data['train_data'] = dataset[:train_size]
data['train_labels'] = labels[:train_size]
data['validation_data'] = dataset[train_size:train_size + validation_size] 
data['validation_labels'] = labels[train_size:train_size + validation_size] 
data['test_data'] = dataset[train_size + validation_size:]
data['test_labels'] = labels[train_size + validation_size:]

with open('data.pickle', 'w') as o:
	pickle.dump(data, o, -1)



	#image_data = (ndimage.imread(image_file).astype(float))
	# image_data = misc.imresize(image_data, [32, 32])

	# img = plt.imshow(image_data)



