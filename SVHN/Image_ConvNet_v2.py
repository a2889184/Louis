from __future__ import print_function
import numpy as np
import tensorflow as tf
from six.moves import cPickle as pickle
from six.moves import range
import math
import itertools
import matplotlib.pyplot as plt
import sys
from PIL import Image, ImageFont, ImageDraw
import random

'''parameter declaration'''
image_size = 32
num_labels = 11 # 1 for non-number
num_channels = 1 # grayscale
digit_length = 5 # how many digit of number string

conv_layer_num = 3
batch_size = 16
patch_size_1 = 5
patch_size_2 = 3
patch_size_3 = 1
depth_1 = 48
depth_2 = 80
depth_3 = 100
depth_4 = 128
depth_5 = 150
depth_6 = 180
num_hidden_1 = 128
num_hidden_2 = 128
num_hidden_3 = 32

std1 = 0.1
std2 = 0.1

cont = 0.0
cont2 = 0.0


'''function part'''
def fill_label(label, pos):
  label_string = str(label)
  for i in range(digit_length):
    if(i < len(label_string)):
      index = i
      digit = int(label_string[index])
      pos_list.append(digit + i * group_num)
    # else:
    #   # pos_list.append(10 + i * group_num )
    #   pos_list.append(0 + i * group_num )

  return pos_list

def reformat(dataset, labels):
  dataset = dataset.reshape((-1, image_size, image_size, num_channels)).astype(np.float32)

  # reshape labels
  labels_digit = np.zeros(shape = (len(labels), digit_length), dtype=np.float32)
  index_num = 0
  for label in labels:
    # label is a number, e.g 1234
    label_string = str(label)
    for i in xrange(digit_length):
      if(i < len(label_string)):
        labels_digit[index_num][i] = float(label_string[i])
      else:
        labels_digit[index_num][i] = float(num_labels - 1)

    index_num = index_num + 1

  return dataset, labels_digit

myAlpha, myLambda = (1.9769021954241999, 1.073851239616047)
def selu(x):
  alpha = myAlpha
  scale = myLambda
  return scale*tf.where(x>=0.0, x, alpha*tf.nn.elu(x))

def inception_parameters_init(patch_size_list, depth_size_list, input_depth, std):
  weighted_list = list()
  bias_list = list()
  with tf.name_scope('inception'):
    for i in xrange(len(patch_size_list)):
      # weighted_list.append(tf.Variable(tf.truncated_normal(
      #   [patch_size_list[i], patch_size_list[i], input_depth, depth_size_list[i]], stddev=std), name='weight_' + str(i + 1)))
      weighted_list.append(tf.get_variable(name='weight_' + str(i + 1), shape = [patch_size_list[i], patch_size_list[i], \
          input_depth, depth_size_list[i]], initializer=tf.contrib.layers.variance_scaling_initializer()))
      #bias_list.append(tf.Variable(tf.zeros([depth_size_list[i]])))
      bias_list.append(tf.Variable(tf.constant(cont, shape=[depth_size_list[i]]), name='bias_' + str(i + 1)))

  return weighted_list, bias_list

def conv_parameters_init(patch_size_list, depth_size_list, input_depth, std):
  weighted_list = list()
  bias_list = list()
  with tf.name_scope('conv'):
    for i in range(len(patch_size_list)):
      if(i == 0):
        # weighted_list.append(tf.Variable(tf.truncated_normal(
        #   [patch_size_list[i], patch_size_list[i], input_depth, depth_size_list[i]], stddev=std), name='weight_' + str(i + 1)))
        weighted_list.append(tf.get_variable(name='cweight_' + str(i + 1), shape = [patch_size_list[i], patch_size_list[i], \
          input_depth, depth_size_list[i]], initializer=tf.contrib.layers.variance_scaling_initializer()))
        #bias_list.append(tf.Variable(tf.zeros([depth_size_list[i]])))
        bias_list.append(tf.Variable(tf.constant(cont, shape=[depth_size_list[i]]), name='weight_' + str(i + 1)))
      else:
        # weighted_list.append(tf.Variable(tf.truncated_normal(
        #   [patch_size_list[i], patch_size_list[i], depth_size_list[i - 1], depth_size_list[i]], stddev=std), name='weight_' + str(i + 1)))
        weighted_list.append(tf.get_variable(name='cweight_' + str(i + 1), shape = [patch_size_list[i], patch_size_list[i], \
          depth_size_list[i - 1], depth_size_list[i]], initializer=tf.contrib.layers.variance_scaling_initializer()))
        #bias_list.append(tf.Variable(tf.zeros([depth_size_list[i]])))
        bias_list.append(tf.Variable(tf.constant(cont, shape=[depth_size_list[i]]), name='weight_' + str(i + 1)))

  return weighted_list, bias_list

def fc_parameters_init(node_size_list, input_size, std):
  weighted_list = list()
  bias_list = list()
  with tf.name_scope('fc'):
    for i in range(len(node_size_list)):
      if(i == 0):
        # weighted_list.append(tf.Variable(tf.truncated_normal(
        #   [input_size, node_size_list[i]], stddev=std), name='weight_' + str(i + 1)))
        weighted_list.append(tf.get_variable(name='fcweight_' + str(i + 1), shape = [input_size, node_size_list[i]], \
          initializer=tf.contrib.layers.variance_scaling_initializer()))
        bias_list.append(tf.Variable(tf.constant(cont2, shape=[node_size_list[i]]), name='weight_' + str(i + 1)))
      else:
        # weighted_list.append(tf.Variable(tf.truncated_normal(
        #   [node_size_list[i - 1], node_size_list[i]], stddev=std), name='weight_' + str(i + 1)))
        weighted_list.append(tf.get_variable(name='fcweight_' + str(i + 1), shape = [node_size_list[i - 1], node_size_list[i]], \
          initializer=tf.contrib.layers.variance_scaling_initializer()))
        bias_list.append(tf.Variable(tf.constant(cont2, shape=[node_size_list[i]]), name='weight_' + str(i + 1)))

  return weighted_list, bias_list


def output_parameters_init(node_size_list, input_size, std):
  weighted_list = list()
  bias_list = list()
  with tf.name_scope('output_classfier'):
    for i in range(len(node_size_list)):
      # weighted_list.append(tf.Variable(tf.truncated_normal(
      #   [input_size, node_size_list[i]], stddev=std), name='weight_' + str(i + 1)))
      weighted_list.append(tf.get_variable(name='oweight_' + str(i + 1), shape = [input_size, node_size_list[i]], \
        initializer=tf.contrib.layers.variance_scaling_initializer()))
      bias_list.append(tf.Variable(tf.constant(cont2, shape=[node_size_list[i]]), name='weight_' + str(i + 1)))

  return weighted_list, bias_list

''' activated function'''
ACTIVATED_FUNC = tf.nn.relu

def inception_compute(data_input, weighted_list, bias_list, conv_padding, pool_padding, pool_size,\
  padding, padding_list, pool_type, dropout_keep_prob, norm=True, training=True):
  if (norm == True):
    data_input = tf.layers.batch_normalization(data_input, axis=0, training=training)
  for i in range(len(weighted_list)):
    conv = tf.nn.conv2d(data_input, weighted_list[i], conv_padding, padding=padding)
    if(norm == True):
      if(padding_list[i] == 1):
          if (pool_type[i] == 'avg'):
            POOL_FUNC = tf.nn.avg_pool
          else:
            POOL_FUNC = tf.nn.max_pool
          temp = tf.layers.batch_normalization(conv + bias_list[i], axis=0, training=training)
          pool = POOL_FUNC(ACTIVATED_FUNC(temp) , pool_size, pool_padding, padding=padding)
          pool = tf.nn.dropout(pool, keep_prob = dropout_keep_prob)
      else:
        temp = tf.layers.batch_normalization(conv + bias_list[i], axis=0, training=training)
        pool = ACTIVATED_FUNC(temp)
    else:
      if(padding_list[i] == 1):
        if (pool_type[i] == 'avg'):
          POOL_FUNC = tf.nn.avg_pool
        else:
          POOL_FUNC = tf.nn.max_pool
        pool = POOL_FUNC(ACTIVATED_FUNC(conv + bias_list[i]) , pool_size, pool_padding, padding=padding)
        pool = tf.nn.dropout(pool, keep_prob = dropout_keep_prob)
      else:
        pool = ACTIVATED_FUNC(conv + bias_list[i])

    if(i == 0): 
      hidden = pool
    else:
      hidden = tf.concat([hidden, pool], 3)
  return hidden

def conv_compute(data_input, weighted_list, bias_list, conv_padding, pool_padding, pool_size,\
  padding, padding_list, pool_type, dropout_keep_prob, norm=True, training=True):
  
  for i in range(len(weighted_list)):
    if(i == 0):
      conv = tf.nn.conv2d(data_input, weighted_list[i], conv_padding, padding=padding)
    else:
      conv = tf.nn.conv2d(hidden, weighted_list[i], conv_padding, padding=padding)

    if(norm == True):
      if(padding_list[i] == 1):
          if (pool_type[i] == 'avg'):
            POOL_FUNC = tf.nn.avg_pool
          else:
            POOL_FUNC = tf.nn.max_pool
          temp = tf.layers.batch_normalization(conv + bias_list[i], axis=0, training=training)
          pool = POOL_FUNC(ACTIVATED_FUNC(temp) , pool_size, pool_padding, padding=padding)
          hidden = tf.nn.dropout(pool, keep_prob = dropout_keep_prob)
      else:
        temp = tf.layers.batch_normalization(conv + bias_list[i], axis=0, training=training)
        hidden = ACTIVATED_FUNC(temp)
    else:
      if(padding_list[i] == 1):
        if (pool_type[i] == 'avg'):
          POOL_FUNC = tf.nn.avg_pool
        else:
          POOL_FUNC = tf.nn.max_pool
        pool = POOL_FUNC(ACTIVATED_FUNC(conv + bias_list[i]) , pool_size, pool_padding, padding=padding)
        hidden = tf.nn.dropout(pool, keep_prob = dropout_keep_prob)
      else:
        hidden = ACTIVATED_FUNC(conv + bias_list[i])

  return hidden

def fc_compute(data_input, weighted_list, bias_list, dropout_keep_prob, norm=True, training=True):

  for i in range(len(weighted_list)):
    if(i == 0):
      if(norm == True):
        temp = tf.layers.batch_normalization(tf.matmul(data_input, weighted_list[i]) + bias_list[i], axis=0, training=training)
        hidden = tf.nn.dropout(ACTIVATED_FUNC(temp), keep_prob=dropout_keep_prob)
      else:
        hidden = tf.nn.dropout(ACTIVATED_FUNC(tf.matmul(data_input, weighted_list[i]) + bias_list[i]), keep_prob=dropout_keep_prob)
    else:
      if(norm == True):
        temp = tf.layers.batch_normalization(tf.matmul(hidden, weighted_list[i]) + bias_list[i], axis=0, training=training)
        hidden = tf.nn.dropout(ACTIVATED_FUNC(temp), keep_prob=dropout_keep_prob)
      else:
        hidden = tf.nn.dropout(ACTIVATED_FUNC(tf.matmul(hidden, weighted_list[i]) + bias_list[i]), keep_prob=dropout_keep_prob)

  return hidden
def output_compute(data_input, weighted_list, bias_list):
  output_list = list()

  for i in range(len(weighted_list)):
    output_list.append(tf.matmul(data_input, weighted_list[i]) + bias_list[i])
  return output_list



def model(data, keep_prob, norm, training):
  # layer 1
  inception_out_1 = inception_compute(data, inception_weight_1, inception_bias_1, [1, 1, 1, 1], [1, 2, 2, 1], [1, 2, 2, 1],\
   'SAME', [1], ['max'], keep_prob, norm, training)

  # conv_out_2 = conv_compute(inception_out_1, conv_weight_2, conv_bias_2, [1, 1, 1, 1], [1, 2, 2, 1], [1, 2, 2, 1], \
  #   'SAME', [0, 1, 0, 1, 0, 1, 0, 1], ['none', 'max', 'none', 'max', 'none', 'avg', 'none', 'avg'], keep_prob, norm, training)
  conv_out_2 = conv_compute(inception_out_1, conv_weight_2, conv_bias_2, [1, 1, 1, 1], [1, 2, 2, 1], [1, 2, 2, 1], \
     'SAME', [1, 1], ['avg', 'avg'], keep_prob, norm, training)
  shape = conv_out_2.get_shape().as_list()
  reshape = tf.reshape(conv_out_2, [shape[0], shape[1] * shape[2] * shape[3]])

  # fc_out_3 = fc_compute(reshape, fc_weight_3, fc_bias_3, keep_prob, norm, training)
  return output_compute(reshape, output_weight_4, output_bias_4), reshape

def accuracy(predictions, labels):
  true_table_sum = tf.reduce_sum(tf.to_int32(tf.equal(predictions, tf.to_int64(labels))), axis=1)
  true_table_sum_2 = tf.reduce_sum(true_table_sum / tf.shape(predictions)[1])
  # return 100.0 * tf.to_float(true_table_sum / tf.shape(predictions)[0]) / tf.to_float(tf.shape(predictions)[1])
  return 100.0 * tf.to_float(true_table_sum_2) / tf.to_float(tf.shape(predictions)[0])

def compute_loss(logits_list, labels):
  loss = tf.constant(0.0, dtype=tf.float32)
  for i in xrange(len(logits_list)):
    loss = loss + tf.reduce_mean(tf.nn.sparse_softmax_cross_entropy_with_logits(logits=logits_list[i], labels=labels[:,i]))
  return loss

def compute_loss_list_and_loss(logits_list, labels):
  loss_list = list()
  loss = tf.constant(0.0, dtype=tf.float32)
  for i in xrange(len(logits_list)):
    loss_list.append(tf.reduce_mean(tf.nn.sparse_softmax_cross_entropy_with_logits(logits=logits_list[i], labels=labels[:,i])))
    loss = loss + loss_list[i]
  return loss_list, loss

def pred_num(prediction_list):
  pred = list()
  for i in xrange(digit_length):
    pred.append(tf.nn.softmax(prediction_list[i]))

  return tf.transpose(tf.argmax(tf.stack(pred), 2))

def numarray_to_num(num_array):
  num_of_none_number = np.sum((num_array == (num_labels - 1)), 1, keepdims=True)
  num_of_none_number = np.power(10, num_of_none_number)
  mask = np.ndarray(shape=(num_array.shape[0], num_array.shape[1]), dtype=np.int)

  for i in xrange(mask.shape[1]):
    mask[:, i] = math.pow(10, mask.shape[1] - 1 - i)
  mask = mask / num_of_none_number

  return np.sum(mask * num_array, 1, keepdims=True)

def print_image(arr, text):
  arr = arr * (255.0 / 2) + (255.0 / 2)
  im = Image.fromarray(arr.astype('uint8'))
  im = im.resize ((4 * image_size, 4 * image_size), Image.ANTIALIAS)
  draw = ImageDraw.Draw(im)
  draw.text((0, 0), str(text), (255))
  im.show()

'''execute part'''


l = sys.argv
l.pop(0)

restore = l.pop(0)


pickle_file = 'data_full.pickle'

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

for i in xrange(4):
  print (train_labels[i])

train_dataset, train_labels = reformat(train_dataset, train_labels)
valid_dataset, valid_labels = reformat(valid_dataset, valid_labels)
test_dataset, test_labels = reformat(test_dataset, test_labels)

for i in xrange(4):
  print (train_labels[i])

random_size = 5
sample = random.sample(xrange(0, 5000), random_size)
random_test_dataset = test_dataset[sample]


graph = tf.Graph()

with graph.as_default():

  # Input data.
  with tf.name_scope('Initialized_Data'):
    tf_train_dataset = tf.placeholder(
      tf.float32, shape=(batch_size, image_size, image_size, num_channels), name='tf_train_dataset')
    tf_train_labels = tf.placeholder(tf.int32, shape=(batch_size, digit_length), name='tf_train_labels')
    tf_valid_dataset = tf.constant(valid_dataset, name='tf_valid_dataset')
    tf_test_dataset = tf.constant(test_dataset[:5000], name='tf_test_dataset')
    tf_valid_labels = tf.constant(valid_labels, name='tf_valid_labels')
    tf_test_labels = tf.constant(test_labels[:5000], name='tf_test_labels')
    tf_random_test_dataset = tf.constant(random_test_dataset, name='tf_random_test_dataset')
    keep_prob = tf.placeholder(tf.float32, name='keep_prob')
    global_step = tf.Variable(0, trainable=False, name='global_step')
    learning_rate = tf.train.exponential_decay(0.05, global_step, 10000, 0.95, name='learning_rate')
    learning_rate_array = tf.placeholder(tf.float32, shape=(digit_length), name='learning_rate_array')
  
  with tf.name_scope('DNN'):
    inception_weight_1, inception_bias_1 = inception_parameters_init(
      [patch_size_1], [depth_1], num_channels, std1)

    # conv_weight_2, conv_bias_2 = conv_parameters_init([patch_size_1, patch_size_1, patch_size_1, patch_size_1, patch_size_2, patch_size_2,\
    #   patch_size_2, patch_size_2], [depth_2, depth_3, depth_4, depth_5, depth_5, depth_5, depth_5, depth_6], depth_1, std1)
    conv_weight_2, conv_bias_2 = conv_parameters_init([patch_size_1, patch_size_1], [depth_2, depth_3], depth_1, std1)
    input_size = int(math.ceil(float(image_size) / math.pow(2.0, conv_layer_num) )) *\
     int(math.ceil(float(image_size) / math.pow(2.0, conv_layer_num))) * (depth_3)

    # fc_weight_3, fc_bias_3 = fc_parameters_init([num_hidden_1, num_hidden_2], input_size, std2)

    output_weight_4, output_bias_4 = output_parameters_init(
      [num_labels, num_labels, num_labels, num_labels, num_labels], input_size, std2)


  # Training computation.
  
  logits_list = model(tf_train_dataset, keep_prob, True, True)[0]

  with tf.name_scope('loss'):
    # loss = compute_loss(logits_list, tf_train_labels)
    loss_list, loss = compute_loss_list_and_loss(logits_list, tf_train_labels)
  
  # Optimizer.
  # learning_rate = 0.05
  # default_vars = list(itertools.chain(inception_weight_1, inception_bias_1, conv_weight_2, conv_bias_2))
  extra_update_ops = tf.get_collection(tf.GraphKeys.UPDATE_OPS)
  with tf.name_scope('optimizer'):
    with tf.control_dependencies(extra_update_ops):
      optimizer = tf.train.AdagradOptimizer(learning_rate).minimize(loss, global_step=global_step)
    # opt = list()
    # for i in xrange(digit_length):
    #   opt.append(tf.train.AdagradOptimizer(learning_rate_array[i]))
    # optimizer = list()
    # for i in xrange(digit_length):
    #   optimizer.append(opt[i].minimize(loss=loss_list[i], var_list=list(itertools.chain(
    #     default_vars, [output_weight_4[i], output_bias_4[i]]))))
  
  # Predictions for the train, validation, and test data.
  with tf.name_scope('prediction'):
    train_prediction = pred_num(model(tf_train_dataset, keep_prob, True, False)[0])
    valid_prediction = pred_num(model(tf_valid_dataset, keep_prob, True, False)[0])
    test_prediction = pred_num(model(tf_test_dataset, keep_prob, True, False)[0])
    random_test_prediction = pred_num(model(tf_random_test_dataset, keep_prob, True, False)[0])
  # compute accuracy

  with tf.name_scope('accuracy'):
    accuracy_train = accuracy(train_prediction, tf_train_labels)
    accuracy_valid = accuracy(valid_prediction, tf_valid_labels)
    accuracy_test = accuracy(test_prediction, tf_test_labels)


  # add information to tensorboard
  with tf.name_scope('summary'):
    tf.summary.histogram('logits', logits_list)
    tf.summary.scalar('loss', loss)
    tf.summary.scalar('learning_rate', learning_rate)
    tf.summary.scalar('train accuracy', accuracy_train)
    tf.summary.scalar('valid accuracy', accuracy_valid)

    merged = tf.summary.merge_all()

  with tf.name_scope('saver'):
    saver = tf.train.Saver()

num_steps = 60001

# train_y = list()
# valid_y = list()
# X = list()

folder_name = './bn/'
file_name = 'CNN_multi.ckpt'
with tf.Session(graph=graph) as session:

  if (restore == 1):
    ckpt = tf.train.get_checkpoint_state(folder_name)
    saver.restore(session, ckpt.model_checkpoint_path)
    print("Model restored.")  
    pred_result = session.run(random_test_prediction, feed_dict={keep_prob: 1.0})
    pred_result = numarray_to_num(pred_result)

    print (pred_result)

    temp = test_dataset.reshape((-1, image_size, image_size)).astype(np.float32)

    for i in xrange(len(sample)):
      print_image(temp[sample[i]], pred_result[i])
  else:
    test_writer = tf.summary.FileWriter(folder_name, session.graph)
    tf.global_variables_initializer().run()
    print('Initialized') 
    # reader = tf.train.NewCheckpointReader("CNN_1.ckpt")
    # reader.get_variable_to_shape_map()
    # saver.restore(session, "CNN_1.ckpt")
    # print("Model restored.")  

    for step in range(num_steps):
      offset = (step * batch_size) % (train_labels.shape[0] - batch_size)
      batch_data = train_dataset[offset:(offset + batch_size), :, :, :]
      batch_labels = train_labels[offset:(offset + batch_size),:]
      feed_dict = {tf_train_dataset : batch_data, tf_train_labels : batch_labels, keep_prob : 0.7}
      _, l= session.run([optimizer, loss], feed_dict=feed_dict)

      # decay_rate = 0.9
      # global_step = step
      # decay_steps = 10000.0
      # ll_sum = np.sum(np.asarray(ll) ** 2)
      # LR =  0.1 * math.pow(decay_rate, float(step) / decay_steps)
      # l_ll = LR * ((np.asarray(ll)) ** 2 / ll_sum)
      # _= session.run([optimizer], feed_dict={\
      #   tf_train_dataset : batch_data, tf_train_labels : batch_labels, keep_prob : 0.7, learning_rate_array:l_ll})
      if (step % 500 == 0): 
        vfeed_dict = {tf_train_dataset : batch_data, tf_train_labels : batch_labels, keep_prob : 1.0}
        ll= session.run(loss_list, feed_dict=feed_dict)
        print(ll)
        print('Minibatch loss at step %d: %f' % (step, l))
        [train_acc, valid_acc, summary]= session.run([accuracy_train, accuracy_valid, merged], feed_dict=vfeed_dict)
        print('Minibatch accuracy: %.1f%%' % train_acc)
        # valid_acc = session.run(accuracy_valid, feed_dict={keep_prob : 1.0})
        print('Validation accuracy: %.1f%%' % valid_acc)
        test_writer.add_summary(summary, step)

        ttt = session.run(train_prediction, feed_dict=vfeed_dict)
        for i in xrange(5):
          print ('prediction: ')
          print (ttt[i])
          print ('labels: ')
          print (batch_labels[i].astype(int))

        # train_y.append(train_acc)
        # valid_y.append(valid_acc)
        # X.append(step)
        # Y1,  = plt.plot(X, train_y, 'r--')
        # Y2,  = plt.plot(X, valid_y, 'b^')
        # plt.legend([Y1, Y2], ['train_acc', 'valid_acc'])
        # plt.axis([0, step + 1, 0, 100])
        # plt.ylabel('accuracy(%)')
        # plt.xlabel('loop')
    print('Test accuracy: %.1f%%' % session.run(accuracy_test, feed_dict={keep_prob : 1.0}))
    save_path = saver.save(session, folder_name + file_name)
    print("Model saved in file: %s" % save_path)
    # plt.show()

