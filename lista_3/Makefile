# Makefile
CFLAGS = -Wall -pedantic -O3

all:
	g++ $(CFLAGS) Dijkstra.cpp Heap.cpp -o dijkstra
	g++ $(CFLAGS) Diala.cpp -o dial
	g++ $(CFLAGS) RadixHeap.cpp -o radixheap

clean:
	rm dijkstra
	rm dial
	rm radixheap