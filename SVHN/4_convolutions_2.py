
# coding: utf-8

# Deep Learning
# =============
# 
# Assignment 4
# ------------
# 
# Previously in `2_fullyconnected.ipynb` and `3_regularization.ipynb`, we trained fully connected networks to classify [notMNIST](http://yaroslavvb.blogspot.com/2011/09/notmnist-dataset.html) characters.
# 
# The goal of this assignment is make the neural network convolutional.

# In[1]:

# These are all the modules we'll be using later. Make sure you can import them
# before proceeding further.
from __future__ import print_function
import numpy as np
import tensorflow as tf
from six.moves import cPickle as pickle
from six.moves import range


# In[2]:

pickle_file = 'data_easy.pickle'

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


# Reformat into a TensorFlow-friendly shape:
# - convolutions need the image data formatted as a cube (width by height by #channels)
# - labels as float 1-hot encodings.

# In[3]:

image_size = 32
num_labels = 10
num_channels = 1 # grayscale

import numpy as np

def reformat(dataset, labels):
  dataset = dataset.reshape(
    (-1, image_size, image_size, num_channels)).astype(np.float32)
  labels = (np.arange(num_labels) == labels[:,None]).astype(np.float32)
  return dataset, labels
train_dataset, train_labels = reformat(train_dataset, train_labels)
valid_dataset, valid_labels = reformat(valid_dataset, valid_labels)
test_dataset, test_labels = reformat(test_dataset, test_labels)
print('Training set', train_dataset.shape, train_labels.shape)
print('Validation set', valid_dataset.shape, valid_labels.shape)
print('Test set', test_dataset.shape, test_labels.shape)


# In[4]:

def accuracy(predictions, labels):
  return (100.0 * np.sum(np.argmax(predictions, 1) == np.argmax(labels, 1))
          / predictions.shape[0])

# Let's build a small network with two convolutional layers, followed by one fully connected layer. Convolutional networks are more expensive computationally, so we'll limit its depth and number of fully connected nodes.

# In[14]:

batch_size = 64
patch_size_1 = 5
patch_size_2 = 3
patch_size_3 = 1
depth = 7
depth_2 = 3
depth_3 = 1
depth_4 = 30
num_hidden_1 = 128
num_hidden_2 = 64
num_hidden_3 = 32
import math

graph = tf.Graph()

std = 1

with graph.as_default():

  # Input data.
  tf_train_dataset = tf.placeholder(tf.float32, shape=(batch_size, image_size, image_size, num_channels))
  tf_train_labels = tf.placeholder(tf.float32, shape=(batch_size, num_labels))
  tf_valid_dataset = tf.constant(valid_dataset)
  tf_test_dataset = tf.constant(test_dataset)

  keep_prob = tf.placeholder(tf.float32)
  # Variables.
  layer1_weights = tf.Variable(tf.truncated_normal([patch_size_1, patch_size_1, num_channels, depth], stddev=std))
  layer1_biases = tf.Variable(tf.zeros([depth]))
  layer1_2_weights = tf.Variable(tf.truncated_normal([patch_size_2, patch_size_2, num_channels, depth_2], stddev=std))
  layer1_2_biases = tf.Variable(tf.zeros([depth_2]))
  layer1_3_weights = tf.Variable(tf.truncated_normal([patch_size_3, patch_size_3, num_channels, depth_3], stddev=std))
  layer1_3_biases = tf.Variable(tf.zeros([depth_3]))

  # layer2_1_weights = tf.Variable(tf.truncated_normal(
  #     [patch_size_1, patch_size_1, depth + depth_2 + depth_3, depth + 2], stddev=std))
  # layer2_1_biases = tf.Variable(tf.constant(1.0, shape=[depth + 2]))
  # layer2_2_weights = tf.Variable(tf.truncated_normal(
  #     [patch_size_2, patch_size_2, depth + depth_2 + depth_3, depth_2 + 2], stddev=std))
  # layer2_2_biases = tf.Variable(tf.constant(1.0, shape=[depth_2 + 2]))
  # layer2_3_weights = tf.Variable(tf.truncated_normal(
  #     [patch_size_3, patch_size_3, depth + depth_2 + depth_3, depth_3 + 2], stddev=std))
  # layer2_3_biases = tf.Variable(tf.constant(1.0, shape=[depth_3 + 2]))

  layer22_weights = tf.Variable(tf.truncated_normal([patch_size_1, patch_size_1, depth + depth_2 + depth_3, depth_4], stddev=std))
  layer22_biases = tf.Variable(tf.constant(1.0, shape=[depth_4]))
  layer3_weights = tf.Variable(tf.truncated_normal(
      [int(math.ceil(float(image_size) / 4.0 )) * int(math.ceil(float(image_size) / 4.0)) * (depth_4), num_hidden_1], stddev=std))
  layer3_biases = tf.Variable(tf.constant(1.0, shape=[num_hidden_1]))
  layer4_weights = tf.Variable(tf.truncated_normal(
      [num_hidden_1, num_hidden_2], stddev=std))
  layer4_biases = tf.Variable(tf.constant(1.0, shape=[num_hidden_2]))
  #layer5_weights = tf.Variable(tf.truncated_normal(
      #[num_hidden_2, num_hidden_3], stddev=0.1))
  #layer5_biases = tf.Variable(tf.constant(1.0, shape=[num_hidden_3]))
  layer6_weights = tf.Variable(tf.truncated_normal([num_hidden_2, num_labels], stddev=std))
  layer6_biases = tf.Variable(tf.constant(1.0, shape=[num_labels]))
  
  
  
  # Model.
  def model(data):
    # layer 1
    conv = tf.nn.conv2d(data, layer1_weights, [1, 1, 1, 1], padding='SAME')
    pool = tf.nn.max_pool(conv + layer1_biases , [1, 2, 2, 1], [1, 2, 2, 1], padding='SAME')
    hidden = tf.nn.tanh(pool)
    conv = tf.nn.conv2d(data, layer1_2_weights, [1, 1, 1, 1], padding='SAME')
    pool = tf.nn.max_pool(conv + layer1_2_biases , [1, 2, 2, 1], [1, 2, 2, 1], padding='SAME')
    hidden = tf.concat(3, [hidden, tf.nn.tanh(pool)])
    conv = tf.nn.conv2d(data, layer1_3_weights, [1, 1, 1, 1], padding='SAME')
    pool = tf.nn.max_pool(conv + layer1_3_biases , [1, 2, 2, 1], [1, 2, 2, 1], padding='SAME')
    hidden = tf.concat(3, [hidden, tf.nn.tanh(pool)])
    
    # layer 2
    # conv = tf.nn.conv2d(hidden, layer2_1_weights, [1, 1, 1, 1], padding='SAME')
    # pool = tf.nn.max_pool(conv + layer2_1_biases, [1, 2, 2, 1], [1, 2, 2, 1], padding='SAME')
    # hidden_2 = tf.nn.tanh(pool)
    # conv = tf.nn.conv2d(hidden, layer2_2_weights, [1, 1, 1, 1], padding='SAME')
    # pool = tf.nn.max_pool(conv + layer2_2_biases, [1, 2, 2, 1], [1, 2, 2, 1], padding='SAME')
    # hidden_2 = tf.concat(3, [hidden_2, tf.nn.tanh(pool)])
    # conv = tf.nn.conv2d(hidden, layer2_3_weights, [1, 1, 1, 1], padding='SAME')
    # pool = tf.nn.max_pool(conv + layer2_3_biases, [1, 2, 2, 1], [1, 2, 2, 1], padding='SAME')
    # hidden_2 = tf.concat(3, [hidden_2, tf.nn.tanh(pool)])
    
    #pool = tf.nn.max_pool(conv, [1, 2, 2, 1], [1, 2, 2, 1], padding='SAME')
    #conv = tf.nn.conv2d(hidden, layer2_weights, [1, 1, 1, 1], padding='SAME')
    #pool = tf.nn.avg_pool(conv + layer2_biases, [1, 2, 2, 1], [1, 2, 2, 1], padding='SAME')
    #hidden = tf.nn.relu(pool)
    conv = tf.nn.conv2d(hidden, layer22_weights, [1, 1, 1, 1], padding='SAME')
    pool = tf.nn.avg_pool(conv + layer22_biases, [1, 2, 2, 1], [1, 2, 2, 1], padding='SAME')
    hidden = tf.nn.relu(pool)

    shape = hidden.get_shape().as_list()
    reshape = tf.reshape(hidden, [shape[0], shape[1] * shape[2] * shape[3]])
    hidden = tf.nn.dropout(tf.nn.tanh(tf.matmul(reshape, layer3_weights) + layer3_biases), keep_prob=keep_prob)
    hidden = tf.nn.dropout(tf.nn.tanh(tf.matmul(hidden, layer4_weights) + layer4_biases), keep_prob=keep_prob)
    #hidden = tf.nn.dropout(tf.nn.tanh(tf.matmul(hidden, layer5_weights) + layer5_biases), keep_prob=keep_prob)
    return tf.matmul(hidden, layer6_weights) + layer6_biases
  
  # Training computation.
  logits = model(tf_train_dataset)
  


  loss = tf.reduce_mean(
    tf.nn.softmax_cross_entropy_with_logits(labels=tf_train_labels, logits=logits))
    
  # learning rate
  #learning_rate = tf.train.exponential_decay(0.1, global_step, 2000, 0.8)
  learning_rate = 0.001
  # Optimizer.
  optimizer = tf.train.AdagradOptimizer(learning_rate).minimize(loss)
  
  # Predictions for the training, validation, and test data.
  train_prediction = tf.nn.softmax(logits)
  valid_prediction = tf.nn.softmax(model(tf_valid_dataset))
  test_prediction = tf.nn.softmax(model(tf_test_dataset))


# In[14]:

num_steps = 30001
import random
with tf.Session(graph=graph) as session:
  tf.global_variables_initializer().run()
  print('Initialized')
  for step in range(num_steps):
    offset = (step * batch_size) % (train_labels.shape[0] - batch_size)
    #offset = random.randint(0, train_labels.shape[0] - batch_size)
    batch_data = train_dataset[offset:(offset + batch_size), :, :, :]
    batch_labels = train_labels[offset:(offset + batch_size), :]
    feed_dict = {tf_train_dataset : batch_data, tf_train_labels : batch_labels, keep_prob: 0.7}
    _, l= session.run([optimizer, loss], feed_dict=feed_dict)
    if (step % 500 == 0):
      print('Iter: %d' % step)
      print('Minibatch loss : %f' % l)
      print('Minibatch accuracy: %.1f%%' % accuracy(train_prediction.eval(
        feed_dict = {tf_train_dataset : batch_data, keep_prob: 1.0}), batch_labels))
      print('Validation accuracy: %.1f%%' % accuracy(
        valid_prediction.eval(feed_dict = {keep_prob: 1.0}), valid_labels))
  # print('Test accuracy: %.1f%%' % accuracy(test_prediction.eval(feed_dict = {keep_prob: 1.0}), test_labels))


# ---
# Problem 2
# ---------
# 
# Try to get the best performance you can using a convolutional net. Look for example at the classic [LeNet5](http://yann.lecun.com/exdb/lenet/) architecture, adding Dropout, and/or adding learning rate decay.
# 
# ---
