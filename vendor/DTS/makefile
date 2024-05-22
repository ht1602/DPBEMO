CC=g++
CXXFLAGS=-g -std=c++11 -Wall

all: Arm.o Run.o main.o test.o
	$(CC) $(CXXFLAGS) Arm.o Run.o main.o -o DTS
	# $(CC) $(CXXFLAGS) Arm.o Run.o test.o -o Test


clean:
	rm -f DTS
	# rm -f Test
	rm -f *.o
