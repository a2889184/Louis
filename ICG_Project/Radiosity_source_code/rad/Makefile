#
# Radiostity 
#

CC=g++ -O -g -Wall


#
# for single machine 
#

OBJS = rad.o shade.o raycast.o vector.o time.o

rad: $(OBJS)
	$(CC) -o rad $(OBJS) -lm 

rad.h : vector.h
rad.o shade.o raycast.o : rad.h

backup: *.c *.h Makefile
	cp *.c backup
	cp *.h backup
	cp Makefile History backup

clean: 
	rm *.o
