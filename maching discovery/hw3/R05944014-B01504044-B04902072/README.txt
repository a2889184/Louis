Task 3:
	Please put source.txt, train.txt, test.txt in test3/
    type the following command on terminal

    $ cd src
    $ g++ biasMF3.cpp -o biasMF3
    $ ./biasMF3

    should generate ../test3/pred.txt

Task 2:
	Please put source.txt, train.txt, test.txt in test2/
	type the following command on terminal
	
	$ cd src
	$ g++ transform.cpp -o transform
	$ g++ pred.cpp -o pred
	$ make

	First, we need to transform test.txt to the format that libFm accept, type:
	
	$ ./transform
	
	will generate a file "../test2/test.trans", which replace '?' with 0 in "test.txt"
	==> test.txt <==
	9878 1433 ?
	2068 1245 ?
	==> test.trans <==
	9878 1433 0
	2068 1245 0

	Then, type
	
	$ ./scripts/triple_format_to_libfm.pl -in ../test2/train.txt,../test2/test.trans -target 2 -separator " "
	
	This should generate "../test2/train.libfm", "../test2/test.trans.libfm"

	Second, type
	
	$ ./bin/libFM -task r -train ../test2/train.txt.libfm -test ../test2/test.trans.libfm -dim ’1,1,5’ -iter 700 -init_stdev 0.01 -out ../test2/fmpred.txt

	should generate "../test2/fmpred.txt" (This step shall take about 5 minutes)
	After libfm finished, we need to transform the prediction file to the format that TAs required. Type
	
	$ ./pred
	
	should generate file "../test2/pred.txt"

Task 1:
	First we need to install "Armadillo" (go to http://arma.sourceforge.net/download.html, and download "armadillo-7.600.1.tar.xz")

	Before install Armadillo, we should install cmake, libopenblas-dev, liblapack-dev, libarpack2-dev first (use sudo apt-get intall [Package Name above]). 

	Extract the file armadillo-7.600.1.tar.xz, and then go to the directory you extract.

	Type
		cmake .
		make 
		sudo make install

	Congratulation! We first finish the installation of Armadillo.


	Go to R05944014-B01504044-B04902072/src/, move test1.cpp main.cpp 'makefile' combine_test change_test_format to R05944014-B01504044-B04902072/test1 (where the data is)

	1. MF + SVD: 
		make run ARGV="train.txt source.txt"

		this step will compile the code and run the program.

	2. latent matching
		g++ main.cpp -o main.out -O3 -larmadillo

		after this step, we have data name final_data.txt merged from R1 and R2.

	3. change the test.txt to be the format without ?
		./change_test_format test.txt test_format_txt

	4. change data and test to libfm format
		./../src/scripts/triple_format_to_libfm.pl -in final_data.txt,test_format.txt -target 2 -separator " "

	5. use libfm to pred 
		./../src/bin/libFM -task r -train final_data.txt.libfm -test test_format.txt.libfm -dim ’1,1,8’ -out output.txt

	6. combine the user item information with their predicted value
		./combine_test test.txt output.txt pred.txt
    