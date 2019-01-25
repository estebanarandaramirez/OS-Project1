all: mc0 mc1 mc2

mc0: mc0.c
	gcc -g -Wall mc0.c -o mc0

mc1: mc1.c
	gcc -g -Wall mc1.c -o mc1

mc2: mc2.c
	gcc -g -Wall mc2.c -o mc2

clean:
	rm -f mc0 mc1 mc2
