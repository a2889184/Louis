The program have to run in ubuntu: 

1. The program is write in c++ language, so you should type the following instructions if you never install the package to compile c++ program. 

$ sudo apt-get update
$ sudo apt-get install build-essential manpages-dev


2. And then use the following instruction to compile the hw1_1.cpp(be sure you have to be in the folder b01504044/src)

$ g++ -o hw1_1.out hw1_1.cpp 


3. Finally, type the instruction below to run the program

$ ./hw1_1.out bigram.txt encode.txt < test.txt

bigram.txt, encode.txt are in the b01504044/src folder, if you want to use your own file, just change them with your 
own paths of the files, but the paths must relative to b01504044/src folder.

test.txt is the test file, you can also change with your own test file, but be cautious just like I mentioned above.

output: b01504044/pred.txt 
