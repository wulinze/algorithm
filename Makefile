CC= g++
CFLAG = -lpthread -o

default: algorithm, delist, heap, tree
algorithm: 
	CC CFLAG -L.algorithm/ 