Error Back Propagation (c++)
BP算法c++实现版本
====================
手写体识别的BP算法实现，输出为0~9的数组，最大的为预测输出
change following values to fit your training data

sample characteristics
#define Feature 256

hidden node
#define Hide_node 100

Feature + 1
#define Feature_plus_1 257

hidden node + 1
#define Hide_node_plus_1 101

The biggest error can tolerate
#define max_err 0.000005

The largest number of training
#define max_train 3000000

number of your training data
#define Data_size 7291


output:
w_hide.txt and w_output.txt

w_hide: weights from input layer to hidden layer
w_output: weights from hidden layer to output layer

reference:
train.txt is a demo of input
and please change your file path before runing
Line 37: fq = fopen("/Users/Damcy/Documents/cplusplus/bp/bp/bp/train.txt","r");

——————————————————————————————