main: asn4.c queue.o asn4.h
	gcc queue.o asn4.c -o asn4 
	./asn4
clean: 
	rm asn4 *.o