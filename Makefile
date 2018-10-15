build: allocator.o
	gcc allocator.o -o allocator
allocator.o:
	gcc -c allocator.c
clean:
	rm -f *.o allocator
