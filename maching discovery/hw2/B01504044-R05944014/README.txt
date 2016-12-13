The program have to run in ubuntu: 

1. The program is write in c++ language, so you should type the following instructions if you never install the package to compile c++ program. 

$ sudo apt-get update
$ sudo apt-get install build-essential manpages-dev


2. And then use the following instruction to compile the Source.cpp

first we have to copy Source.cpp and the makefile to the folder which cantains data(e.g. test1/ and test2/)
when the copy step is over, just keep following the instructions below.

$ make

3. Finally, type the instruction below to run the program

$ make run 

the pred.txt will automatically generate by the program, and it will locate in the folder where the Source.cpp is.


