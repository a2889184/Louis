GX=g++
FILE=test1.cpp
OUT=test1.out
AA=-std=c++11 -O3 -w -larmadillo
FILE:${FILE}
	@${GX} ${FILE} ${AA} -o ${OUT}
run:FILE
	@./${OUT} $(ARGV)
clean:
	@rm -rf ${OUT}