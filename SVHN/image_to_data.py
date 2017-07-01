
folders = sys.argv
folders.pop(0)
  
with open('labels_full.pickle', 'r') as p:
	pickle_data = pickle.load(p)

	train_labels = pickle_data['train_labels']
	test_labels = pickle_data['test_labels']
	extra_labels = pickle_data['extra_labels']
	del pickle_data
  
print (len(train_labels))
print (len(test_labels))
print (len(extra_labels))
 
pixel_max = 255.0
image_size = 32
real_image_size = 32
num_image = 0
  
c_factor = 2.0
b_factor = 2.0
s_factor = 2.0 
ratio = 0.0

expand_ratio = 5
  
# dataset = np.ndarray(shape=(len(extra_labels) + len(test_labels), real_image_size, real_image_size), dtype=np.float32)
# lab = np.ndarray(shape=(len(extra_labels) + len(test_labels)), dtype=np.int)
  
# train_data = np.ndarray(shape=(expand_ratio * len(train_labels), real_image_size, real_image_size), dtype=np.float32)
# train_lab = np.ndarray(shape=(expand_ratio * len(train_labels)), dtype=np.int)

dataset = np.ndarray(shape=(len(train_labels) + len(extra_labels) + len(test_labels), real_image_size, real_image_size), dtype=np.float32)
lab = np.ndarray(shape=(len(train_labels) + len(extra_labels) + len(test_labels)), dtype=np.int)

# laod the data and crop the steet number bound box out
for folder in folders:
	image_files = os.listdir(folder)
	print ('start processing: ' + str(folder) + ' folder')
  	# remove not .png files
  	image_files.remove('see_bboxes.m'), image_files.remove('digitStruct.mat')
 	nummmm = 0
 	# if (folder == 'test' or folder == 'test/' or folder == './test' or folder == './test/'):
 	# 	num_image = 0
 	# if (folder == 'train' or folder == 'train/' or folder == './train' or folder == './train/'):
 	# 	num_image = 0

  	for image in image_files:
  		# get image index (note that image name is start from 1.png, but index is start from 0, so substract 1)
  
  		image_index = int(image[:-4]) - 1
  		image_file = os.path.join(folder, image)
 		if (nummmm % 1000 == 0):
 			print ('process {} %'.format(nummmm * 100 / len(image_files)))
  		try:
  			pil_im = Image.open(image_file).convert('L')
 
 			if (folder == 'train' or folder == 'train/' or folder == './train' or folder == './train/'):
 				bound = train_labels[image_index][:4]
 			elif (folder == 'test' or folder == 'test/' or folder == './test' or folder == './test/'):
 				bound = test_labels[image_index][:4]
 
 			elif (folder == 'extra' or folder == 'extra/' or folder == './extra' or folder == './extra/'):
 				bound = extra_labels[image_index][:4]
 
 			width = bound[2] - bound[0]
 			height = bound[3] - bound[1]
 			bound[0] = bound[0] - int(ratio * width)
 			bound[2] = bound[2] + int(ratio * width)
 			bound[1] = bound[1] - int(ratio * height)
 			bound[3] = bound[3] + int(ratio * height)
  			# resize the image to 32 * 32
  			# width, height = pil_im.size
  			# left = width/3
  			# top = height/4
  			# right = width * 2/3
  			# bottom = height *3/4
 			pil_im = pil_im.crop(bound) 

  			pil_im = pil_im.resize((image_size, image_size), Image.ANTIALIAS)
  			if pil_im.size != (image_size, image_size):
  				raise Exception('Unexpected image shape: %s' % str(pil_im.size))
 			if (folder == 'train' or folder == 'train/' or folder == './train' or folder == './train/'):
 				# for i in xrange(expand_ratio):
 				# 	x = random.randint(0, image_size - real_image_size)
 				# 	y = random.randint(0, image_size - real_image_size)
 				# 	pil_im_temp = pil_im.crop((x, y, x + real_image_size, y + real_image_size))
 				# 	image_array = (np.asarray(pil_im_temp).astype(float) - (pixel_max / 2)) / (pixel_max / 2)
 				# 	train_data[num_image] = image_array
 				# 	train_lab[num_image] = train_labels[image_index][4]
 				# 	num_image = num_image + 1
 				image_array = (np.asarray(pil_im).astype(float) - (pixel_max / 2)) / (pixel_max / 2)
 				dataset[num_image] = image_array
 				lab[num_image] = train_labels[image_index][4]
 				num_image = num_image + 1
 			elif (folder == 'test' or folder == 'test/' or folder == './test' or folder == './test/'):
 				# x = (image_size - real_image_size) / 2
 				# y = (image_size - real_image_size) / 2
 				# pil_im_temp = pil_im.crop((x, y, x + real_image_size, y + real_image_size))
 				image_array = (np.asarray(pil_im).astype(float) - (pixel_max / 2)) / (pixel_max / 2)
 				dataset[num_image] = image_array
 				lab[num_image] = test_labels[image_index][4]
 				num_image = num_image + 1
 			elif (folder == 'extra' or folder == 'extra/' or folder == './extra' or folder == './extra/'):
 				# x = (image_size - real_image_size) / 2
 				# y = (image_size - real_image_size) / 2
 				# pil_im_temp = pil_im.crop((x, y, x + real_image_size, y + real_image_size))
 				image_array = (np.asarray(pil_im).astype(float) - (pixel_max / 2)) / (pixel_max / 2)
 				dataset[num_image] = image_array
 				lab[num_image] = extra_labels[image_index][4]
 				num_image = num_image + 1
  			# enhance the image by factor 2
 			# c_en = ImageEnhance.Contrast(pil_im)
 			# pil_im = c_en.enhance(c_factor)
 			# b_en = ImageEnhance.Brightness(pil_im)
 			# pil_im = b_en.enhance(b_factor)
 			# s_en = ImageEnhance.Sharpness(pil_im)
 			# pil_im = s_en.enhance(s_factor)
  
  			# tranform the image to array and normalize
 			nummmm = nummmm + 1
  		except IOError as e:
  			print('Could not read:', image_file, ':', e, '- it\'s ok, skipping.')
  
  
  		# pil_im = Image.fromarray(image_array)
 	# if (folder == 'train' or folder == 'train/' or folder == './train' or folder == './train/'):
 	# 	for ll in xrange(3):
 	# 		print (train_lab[ll])
 	# 		temp_data = train_data[ll]
 	# 		temp_data = temp_data * (pixel_max / 2) + (pixel_max / 2)
 	# 		temp_im = Image.fromarray(temp_data.astype('uint8'))
	# 		temp_im.show()
 
 	# else:
 	# 	for ll in xrange(3):
 	# 		print (lab[ll])
 	# 		temp_data = dataset[ll]
 	# 		temp_data = temp_data * (pixel_max / 2) + (pixel_max / 2)
 	# 		temp_im = Image.fromarray(temp_data.astype('uint8'))
 	# 		temp_im.show()
 	for ll in xrange(num_image - 15, num_image - 10):
 		print (lab[ll])
 		temp_data = dataset[ll]
 		temp_data = temp_data * (pixel_max / 2) + (pixel_max / 2)
 		temp_im = Image.fromarray(temp_data.astype('uint8'))
 		temp_im.show()
  
validation_size = 5000
test_size = 5000
 
 # random_index = range(len(train_lab))
 # random.shuffle(random_index)

 # train_data = train_data[random_index]
 # train_lab = train_lab[random_index]
 
 train_size = len(lab) - validation_size - test_size

 data = dict()
  
 data['train_data'] = dataset[:train_size]
 data['train_labels'] = lab[:train_size]
 data['validation_data'] = dataset[train_size: train_size + validation_size] 
 data['validation_labels'] = lab[train_size: train_size + validation_size] 
 data['test_data'] = dataset[train_size + validation_size:]
 data['test_labels'] = lab[train_size + validation_size:]
 
 with open('data_full.pickle', 'w') as o:

  
 def print_image(arr):
 	arr = arr * (255.0 / 2) + (255.0 / 2)
 	im = Image.fromarray(arr.astype('uint8'))
 	im.show()
 
  