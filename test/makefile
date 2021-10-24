CPP = g++

all: main

main: main.o LinearAlgebra.o 
	$(CPP) -o main main.o LinearAlgebra.o

LinearAlgebra.o: LinearAlgebra.cpp 
	$(CPP) -c LinearAlgebra.cpp 

main.o: main.cpp	
	$(CPP) -c main.cpp

clean:
	rm main.o