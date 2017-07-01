import pickle
import numpy as np
import tensorflow as tf
import math
import random
import itertools
import matplotlib.pyplot as plt

pickle_file = 'data.pickle'
image_size = 32
num_labels = 10 # 0~10 is first digit(10 means not a number), 11~21 is second digit(21 means not a number)...... and so on
num_channels = 1 # grayscale
digit_length = 4
group_num = 10

myAlpha, myLambda = (1.9769021954241999, 1.073851239616047)
def selu(x):
    alpha = myAlpha
    scale = myLambda
    return scale*tf.where(x>=0.0, x, alpha*tf.nn.elu(x))

def switch_num_to_pos(label):
	pos_list = list()
	label_string = str(label)
	for i in range(digit_length):
		if(i < len(label_string)):
			index = i
			digit = int(label_string[index])
			pos_list.append(digit + i * group_num)
		# else:
		# 	# pos_list.append(10 + i * group_num )
		# 	pos_list.append(0 + i * group_num )

	return pos_list

def switch_pos_to_num(pos_list):
	number_string = ''
	while (len(pos_list) != 0):
		num = pos_list.pop() % (group_num)
		if(num != 10):
			number_string = number_string + str(num)

	if(number_string == ''):
		return -1
	return int(number_string)

def switch_array_to_num(arr):
	return switch_pos_to_num(np.where(arr == 1)[0].tolist())

def reformat(dataset, labels):
	dataset = dataset.reshape((-1, image_size, image_size, num_channels)).astype(np.float32)

	# reshape labels
	labels_conv = np.zeros(shape = (len(labels), digit_length, group_num), dtype=np.float32)
	labels_length = np.zeros(shape = (len(labels), digit_length), dtype=np.float32)
	index_num = 0
	for label in labels:
		# label is a number, e.g 1234
		label_list = switch_num_to_pos(label)
		labels_length[index_num, len(label_list) - 1] = 1.0

		# modify the code to 7 pipline
		while(len(label_list) != 0):
			num = label_list.pop(0)
			layer = num // group_num
			digit = num % group_num

			labels_conv[index_num, layer, digit] = 1.0

		index_num = index_num + 1

	return dataset, labels_conv, labels_length

def get_1_0_array(arr):
	pure_array = np.zeros(shape = arr.shape, dtype=np.float32)
	for i in range(digit_length):
		index = np.argmax(arr[group_num * i:group_num * (i + 1)])
		pure_array[i * group_num + index] = 1

	return pure_array


def accuracy(predictions, labels):
	correct = 0.0
	for i in range(labels.shape[0]):
		p = switch_array_to_num(get_1_0_array(predictions[i]))
		l = switch_array_to_num(get_1_0_array(labels[i]))
		if(p == l):
			print (p)
			correct = correct + 1.0

	show_size = 5
	# tt = random.randint(0, labels.shape[0] - show_size)
	tt = 10
	for i in range(show_size):
		print 'pred: {}, real: {}'.format(switch_array_to_num(get_1_0_array(predictions[tt + i])), switch_array_to_num(
			get_1_0_array(labels[tt + i])))

	return 100.0 * (correct / float(labels.shape[0]))


print ('execute ' + pickle_file)

with open(pickle_file, 'rb') as f:
	save = pickle.load(f)
	train_dataset = save['train_data']
	train_labels = save['train_labels']
	valid_dataset = save['validation_data']
	valid_labels = save['validation_labels']
	test_dataset = save['test_data']
	test_labels = save['test_labels']
	del save  # hint to help gc free up memory
	print('Training set', train_dataset.shape, train_labels.shape)
	print('Validation set', valid_dataset.shape, valid_labels.shape)
	print('Test set', test_dataset.shape, test_labels.shape)


train_dataset, train_labels_conv, train_labels_length = reformat(train_dataset, train_labels)
valid_dataset, valid_labels_conv, valid_labels_length = reformat(valid_dataset, valid_labels)
test_dataset, test_labels_conv, test_labels_length = reformat(test_dataset, test_labels)


train_labels_num = np.argmax(train_labels_length, axis=1) + 1

print (train_labels_num[:5])

print ('reshape done !')


batch_size = 16
patch_size_1 = 5
patch_size_2 = 3
patch_size_3 = 1
depth_1 = 16
depth_2 = 15
depth_3 = 32
depth_4 = 64
depth_5 = 64
num_hidden_1 = 1024
num_hidden_2 = 512
num_hidden_3 = 32

graph = tf.Graph()

std1 = 0.1
std2 = 0.1

cont = 0.0
cont2 = 0.0

def inception_parameters_init(patch_size_list, depth_size_list, input_depth, std):
	weighted_list = list()
	bias_list = list()

	for i in range(len(patch_size_list)):
		weighted_list.append(tf.Variable(tf.truncated_normal(
			[patch_size_list[i], patch_size_list[i], input_depth, depth_size_list[i]], stddev=std)))
		#bias_list.append(tf.Variable(tf.zeros([depth_size_list[i]])))
		bias_list.append(tf.Variable(tf.constant(cont, shape=[depth_size_list[i]])))

	return weighted_list, bias_list

def conv_parameters_init(patch_size_list, depth_size_list, input_depth, std):
	weighted_list = list()
	bias_list = list()
	for i in range(len(patch_size_list)):
		if(i == 0):
			weighted_list.append(tf.Variable(tf.truncated_normal(
				[patch_size_list[i], patch_size_list[i], input_depth, depth_size_list[i]], stddev=std)))
			#bias_list.append(tf.Variable(tf.zeros([depth_size_list[i]])))
			bias_list.append(tf.Variable(tf.constant(cont, shape=[depth_size_list[i]])))
		else:
			weighted_list.append(tf.Variable(tf.truncated_normal(
				[patch_size_list[i], patch_size_list[i], depth_size_list[i - 1], depth_size_list[i]], stddev=std)))
			#bias_list.append(tf.Variable(tf.zeros([depth_size_list[i]])))
			bias_list.append(tf.Variable(tf.constant(cont, shape=[depth_size_list[i]])))

	return weighted_list, bias_list

def fc_parameters_init(node_size_list, input_size, std):
	weighted_list = list()
	bias_list = list()
	for i in range(len(node_size_list)):
		if(i == 0):
			weighted_list.append(tf.Variable(tf.truncated_normal(
				[input_size, node_size_list[i]], stddev=std)))
			bias_list.append(tf.Variable(tf.constant(cont2, shape=[node_size_list[i]])))
		else:
			weighted_list.append(tf.Variable(tf.truncated_normal(
				[node_size_list[i - 1], node_size_list[i]], stddev=std)))
			bias_list.append(tf.Variable(tf.constant(cont2, shape=[node_size_list[i]])))

	return weighted_list, bias_list

def output_parameters_init(node_size_list, input_size, std):
	weighted_list = list()
	bias_list = list()
	for i in range(len(node_size_list)):
		weighted_list.append(tf.Variable(tf.truncated_normal(
			[input_size, node_size_list[i]], stddev=std)))
		bias_list.append(tf.Variable(tf.constant(cont2, shape=[node_size_list[i]])))

	return weighted_list, bias_list

def inception_compute(data_input, weighted_list, bias_list, conv_padding, pool_padding, pool_size, pool_list, dropout_keep_prob):
	
	for i in range(len(weighted_list)):
		if(i == 0):
			conv = tf.nn.conv2d(data_input, weighted_list[i], [1, conv_padding, conv_padding, 1], padding='SAME')
			if(pool_list[i] == 1):
				pool = tf.nn.max_pool(selu(conv + bias_list[i]) , [1, pool_size, pool_size, 1], [1, pool_padding, pool_padding, 1], padding='SAME')
				hidden = tf.nn.dropout(pool, keep_prob = dropout_keep_prob)
			else:
				hidden = tf.nn.dropout(selu(conv + bias_list[i]), keep_prob = dropout_keep_prob)
		else:
			conv = tf.nn.conv2d(data_input, weighted_list[i], [1, conv_padding, conv_padding, 1], padding='SAME')
			if(pool_list[i] == 1):
				pool = tf.nn.max_pool(selu(conv + bias_list[i]) , [1, pool_size, pool_size, 1], [1, pool_padding, pool_padding, 1], padding='SAME')
				hidden = tf.concat([hidden, tf.nn.dropout(pool, keep_prob = dropout_keep_prob)], 3)
			else:
				hidden = tf.concat([hidden, tf.nn.dropout(selu(conv + bias_list[i]), keep_prob = dropout_keep_prob)], 3)
	return hidden

def conv_compute(data_input, weighted_list, bias_list, conv_padding, pool_padding, pool_size, pool_list, dropout_keep_prob):
	
	for i in range(len(weighted_list)):
		if(i == 0):
			conv = tf.nn.conv2d(data_input, weighted_list[i], [1, conv_padding, conv_padding, 1], padding='SAME')
			if(pool_list[i] == 1):
				pool = tf.nn.max_pool(selu(conv + bias_list[i]) , [1, pool_size, pool_size, 1], [1, pool_padding, pool_padding, 1], padding='SAME')
				hidden = tf.nn.dropout(pool, keep_prob = dropout_keep_prob)
			else:
				hidden = tf.nn.dropout(selu(conv + bias_list[i]), keep_prob = dropout_keep_prob)
		else:
			conv = tf.nn.conv2d(hidden, weighted_list[i], [1, conv_padding, conv_padding, 1], padding='SAME')
			if(pool_list[i] == 1):
				pool = tf.nn.max_pool(selu(conv + bias_list[i]) , [1, pool_size, pool_size, 1], [1, pool_padding, pool_padding, 1], padding='SAME')
				hidden = tf.nn.dropout(pool, keep_prob = dropout_keep_prob)
			else:
				hidden = tf.nn.dropout(selu(conv + bias_list[i]), keep_prob = dropout_keep_prob)
	return hidden

def fc_compute(data_input, weighted_list, bias_list, dropout_keep_prob):

	for i in range(len(weighted_list)):
		if(i == 0):
			hidden = tf.nn.dropout(selu(tf.matmul(data_input, weighted_list[i]) + bias_list[i]), keep_prob=dropout_keep_prob)
		else:
			hidden = tf.nn.dropout(selu(tf.matmul(hidden, weighted_list[i]) + bias_list[i]), keep_prob=dropout_keep_prob)

	return hidden
def output_compute(data_input, weighted_list, bias_list):
	output_list = list()

	for i in range(len(weighted_list)):
		output_list.append(tf.matmul(data_input, weighted_list[i]) + bias_list[i])
	return output_list


def compute_loss_and_minimize(tf_train_labels, tf_train_labels_length, logits_list):

	loss_list = list()
	loss = tf.multiply(
		tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits(labels=tf_train_labels_length, logits=logits_list[0])), 1.67)
	loss_list.append(loss)
	#opt = optimizer.minimize(loss=loss, var_list=list(itertools.chain(default_vars, [output_weight[0], output_bias[0]])))

	for i in range(tf_train_labels.get_shape()[0]):
		#for j in range(tf.argmax(tf_train_labels_length[i], axis=1) + 1):
		for j in range(digit_length):
			partial_loss = tf.reduce_sum(tf.nn.softmax_cross_entropy_with_logits(
				labels=tf_train_labels[i, j], logits=logits_list[j + 1][i]))
			if(i == 0):
				loss_list.append(partial_loss)
			else:
				loss_list[j + 1] = loss_list[j + 1] + partial_loss
			#opt = optimizer.minimize(loss=partial_loss, var_list=list(itertools.chain(default_vars, [output_weight[j + 1], output_bias[j + 1]])))

	for i in xrange(1, len(loss_list)):
		loss_list[i] = tf.divide(loss_list[i], float(batch_size))
		loss = tf.add(loss, loss_list[i])

	return loss, loss_list

def softmax_list(logits_list):
	predictions_list = list()

	for logits in logits_list:
		predictions_list.append(tf.nn.softmax(logits))
	return predictions_list

def compute_accuracy(predictions_list, labels):
	correct = 0.0
	lll = 0
	for i in range(labels.shape[0]):
		max_prob = -100000.0
		max_num = 0
		pre_num = 0
		pre_prob = 0.0
		if(i == 0):
			print 'label: {}'.format(labels[i])
		for j in xrange(np.argmax(predictions_list[0][i]) + 1):
			max_num = max_num * 10 + np.argmax(predictions_list[j + 1][i])
		# for j in range(predictions_list[0][i].shape[0]):
		# 	if(j == 0):
		# 		max_pos = np.argmax(predictions_list[j + 1][i])
		# 		temp_num = max_pos
		# 		if (predictions_list[0][i, j] == 0):
		# 			temp_prob = predictions_list[j + 1][i, max_pos]
		# 			pre_prob = temp_prob
		# 		else:
		# 			temp_prob = predictions_list[0][i, j] * predictions_list[j + 1][i, max_pos]
		# 			if(temp_prob > max_prob):
		# 				max_prob = temp_prob
		# 				max_num = temp_num
		# 			pre_prob = temp_prob / predictions_list[0][i, j]
		# 		pre_num = temp_num
				
		# 		# if(i == 0):
		# 		# 	print '{}'.format(i)
		# 		# 	print ('digit num')
		# 		# 	print (predictions_list[0][i, :])
		# 		# 	print ('first digit')
		# 		# 	print (predictions_list[1][i, :])
		# 		# 	print ('second digit')
		# 		# 	print (predictions_list[2][i, :])
		# 		# 	print ('third digit')
		# 		# 	print (predictions_list[3][i, :])
		# 		# 	print ('forth digit')
		# 		# 	print (predictions_list[4][i, :])
		# 		# 	print ('fifth digit')
		# 		# 	print (predictions_list[5][i, :])
		# 		# 	print ('sixth digit')
		# 		# 	print (predictions_list[6][i, :])
		# 		# 	print 'max pos : {}'.format(max_pos)
		# 		# 	print 'temp_prob : {}'.format(temp_prob)
		# 		# 	print 'pre_num : {}'.format(pre_num)
	
		# 	else:
		# 		max_pos = np.argmax(predictions_list[j + 1][i])
		# 		temp_num = pre_num * 10 + max_pos
		# 		if (predictions_list[0][i, j] == 0):
		# 			temp_prob = pre_prob * predictions_list[j + 1][i][max_pos]
		# 			pre_prob = temp_prob
		# 		else:
		# 			temp_prob = (pre_prob * predictions_list[0][i][j]) * predictions_list[j + 1][i][max_pos]
		# 			if(temp_prob > max_prob):
		# 				max_prob = temp_prob
		# 				max_num = temp_num

		# 			pre_prob = temp_prob / predictions_list[0][i][j]

		# 		pre_num = temp_num
				
		# 		# if(i == 0):
		# 		# 	print 'max pos : {}'.format(max_pos)
		# 		# 	print 'temp_prob : {}'.format(temp_prob)
		# 		# 	print 'pre_num : {}'.format(pre_num)
		
		if(lll < 7):
			lll = lll + 1
			print 'pred:{} , real: {}'.format(max_num, labels[i])
			# print 'pred:{} , real: {}'.format(np.argmax(predictions_list[1][i]), int(str(labels[i])[0]))
		if(max_num == labels[i]):
		# if((np.argmax(predictions_list[1][i])) == int(str(labels[i])[0])):
			correct = correct + 1.0

	return 100.0 * correct / labels.shape[0]


with graph.as_default():
	# Input data.
	tf_train_dataset = tf.placeholder(tf.float32, shape=(batch_size, image_size, image_size, num_channels))
	tf_train_real_labels = tf.placeholder(tf.float32, shape=(batch_size))
	tf_train_labels = tf.placeholder(tf.float32, shape=(batch_size, digit_length, num_labels))
	tf_train_labels_length = tf.placeholder(tf.float32, shape=(batch_size, digit_length))
	# tf_length_num = tf.placeholder(tf.float32, shape=(batch_size))
	tf_valid_dataset = tf.constant(valid_dataset[:5000])
	tf_valid_real_labels = tf.constant(valid_labels[:5000])
	tf_test_dataset = tf.constant(test_dataset)
	

	keep_prob = tf.placeholder(tf.float32)

	learning_rate_array = tf.placeholder(tf.float32, shape=(digit_length + 1))
	# Variables.

	inception_weight_1, inception_bias_1 = inception_parameters_init(
		[patch_size_1], [depth_1], num_channels, std1)


	conv_weight_2, conv_bias_2 = conv_parameters_init([patch_size_1, patch_size_1], [depth_3, depth_4], depth_1, std1)

	input_size = int(math.ceil(float(image_size) / 8.0 )) * int(math.ceil(float(image_size) / 8.0)) * (depth_4)

	# fc_weight_3, fc_bias_3 = fc_parameters_init([num_hidden_1, num_hidden_2], input_size, std2)

	output_weight_4, output_bias_4 = output_parameters_init(
		[digit_length, num_labels, num_labels, num_labels, num_labels, num_labels, num_labels], input_size, std2)
  
	# Model.
	def model(data):
		# layer 1
		inception_out_1 = inception_compute(data, inception_weight_1, inception_bias_1, 1, 2, 2, [1], keep_prob)

		conv_out_2 = conv_compute(inception_out_1, conv_weight_2, conv_bias_2, 1, 2, 2, [1, 1], keep_prob)

		shape = conv_out_2.get_shape().as_list()
		# print '{}, {}, {}, {}'.format(shape[0], shape[1], shape[2], shape[3])
		reshape = tf.reshape(conv_out_2, [shape[0], shape[1] * shape[2] * shape[3]])

		# fc_out_3 = fc_compute(reshape, fc_weight_3, fc_bias_3, keep_prob)
		return output_compute(reshape, output_weight_4, output_bias_4), reshape

	# Training computation.
	logits_list, reshape = model(tf_train_dataset)

	# loss = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits(labels=tf_train_labels, logits=logits))

	# learning rate
	#learning_rate = tf.train.exponential_decay(0.1, global_step, 2000, 0.8)
	# learning_rate = 0.001
	# learning_rate_list = [0.0001, 0.0001, 0.0001, 0.0001, 0.0001]
	# Optimizer.
	# default_vars = list(itertools.chain(inception_weight_1, inception_bias_1, conv_weight_2, conv_bias_2, fc_weight_3, fc_bias_3))
	default_vars = list(itertools.chain(inception_weight_1, inception_bias_1, conv_weight_2, conv_bias_2))
	# default_vars = list()
	opt = list()
	for i in xrange(digit_length + 1):
		opt.append(tf.train.AdamOptimizer(learning_rate_array[i]))

	loss, loss_list= compute_loss_and_minimize(
		tf_train_labels, tf_train_labels_length, logits_list)
	# optimizer = tf.train.AdagradOptimizer(learning_rate).minimize(loss)
	optimizer = list()
	for i in xrange(digit_length + 1):
		optimizer.append(opt[i].minimize(loss=loss_list[i], var_list=list(itertools.chain(
			default_vars, [output_weight_4[i], output_bias_4[i]]))))
	
	# Predictions for the training, validation, and test data.
	train_predictions_list = softmax_list(model(tf_train_dataset)[0])
	valid_predictions_list = softmax_list(model(tf_valid_dataset)[0])
	#train_prediction_list = tf.nn.softmax(logits)
	#valid_prediction = tf.nn.softmax(model(tf_valid_dataset))
	#test_prediction = tf.nn.softmax(model(tf_test_dataset))


# In[14]:

num_steps = 50001
train_y = list()
valid_y = list()
X = list()

with tf.Session(graph=graph) as session:
	tf.global_variables_initializer().run()
	print('Initialized')
	for step in range(num_steps):
		offset = (step * batch_size) % (train_labels_conv.shape[0] - batch_size)
		#offset = random.randint(0, train_labels.shape[0] - batch_size)
		batch_data = train_dataset[offset:(offset + batch_size), :, :, :]
		batch_labels = train_labels_conv[offset:(offset + batch_size), :]
		batch_real_labels = train_labels[offset:(offset + batch_size)]
		batch_labels_length = train_labels_length[offset:(offset + batch_size), :]
		# batch_length_num = train_labels_num[offset:(offset + batch_size)]
		l_feed_dict = {tf_train_dataset : batch_data, tf_train_labels : batch_labels, tf_train_real_labels : batch_real_labels, 
			tf_train_labels_length : batch_labels_length, keep_prob: 0.7}
		
		l_l = session.run(loss_list, feed_dict=l_feed_dict)

		decay_rate = 0.9
		global_step = step
		decay_steps = 2000.0
		# l_ll = 0.0001 * np.asarray(l_l)
		ll_sum = np.sum(np.asarray(l_l) ** 2)
		# LR =  0.02 * math.pow(decay_rate, float(step) / decay_steps)
		LR = 0.001
		l_ll = LR * ((np.asarray(l_l) ** 2 ) / ll_sum)

		feed_dict = {tf_train_dataset : batch_data, tf_train_labels : batch_labels, tf_train_real_labels : batch_real_labels, 
			tf_train_labels_length : batch_labels_length, keep_prob: 0.7, learning_rate_array:l_ll}
		_= session.run([optimizer], feed_dict=feed_dict)
		if (step % 500 == 0):
			print('Iter: %d' % step)
			# print('Minibatch loss : %f' % l)
			print ('loss: {}'.format(l_l))
			# print ('learning rate: {}'.format(l_ll))
			vfeed_dict = {tf_train_dataset : batch_data, tf_train_real_labels : batch_real_labels, keep_prob: 1.0}
			print ('{} and {}'.format(batch_real_labels[0], batch_real_labels[1]))
			print ('{} and {}'.format(batch_real_labels[5], batch_real_labels[3]))
			aa = session.run(reshape, feed_dict={tf_train_dataset: batch_data, keep_prob:1.0})

			print ('{}, {}'.format(np.sum((aa[0] - aa[1])**2), np.sum((aa[5] - aa[3])**2)))
			x = session.run(logits_list, feed_dict=vfeed_dict)
			for i in range(digit_length + 1):
				print i
				print (x[i][:4])

			train_acc = compute_accuracy(session.run(train_predictions_list, feed_dict=vfeed_dict), batch_real_labels)
			print('Minibatch accuracy: %.1f%%' % train_acc)
			valid_acc = compute_accuracy(session.run(valid_predictions_list,feed_dict = {keep_prob: 1.0}), valid_labels[:5000])
			train_y.append(train_acc)
			valid_y.append(valid_acc)
			X.append(step)
			Y1,  = plt.plot(X, train_y, 'r--')
			Y2,  = plt.plot(X, valid_y, 'b^')
			plt.legend([Y1, Y2], ['train_acc', 'valid_acc'])
			plt.axis([0, step + 1, 0, 100])
			plt.ylabel('accuracy(%)')
			plt.xlabel('loop')
			print('Validation accuracy: %.1f%%' % valid_acc)

plt.show()
	#print('Test accuracy: %.1f%%' % accuracy(test_prediction.eval(feed_dict = {keep_prob: 1.0}), test_labels_conv))

