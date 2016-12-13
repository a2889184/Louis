The program have to run in ubuntu: 

1. The program is write in c++ language, so you should type the following instructions if you never install the package to compile c++ program. 

$ sudo apt-get update
$ sudo apt-get install build-essential manpages-dev


2. And then use the following instruction to compile the hw1_2.cpp(be sure you have to be in the folder b01504044/src)

$ g++ hw1_2.cpp -std=c++11 -O3 -o hw1_2.out


3. Finally, type the instruction below to run the program

$ ./hw1_2.out [path of encode.bin] < [path of test.num] > [path of pred.num]

path of each files have to relative to folder b01504044/src, except you use absolute path.  

ex: ./hw1_2.out ../test1/encode.bin < ../test1/test.num > ../test1/pred.num

There will be some information of the program on the terminal.(which round of EM algorithm and the time take by each step)
Normally, about 70~90 rounds the program will end.
