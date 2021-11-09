main: asn4.c asn4.h
    gcc asn4.c -o asn4 -lpthread
queue.o: queue.c queue.h
    gcc queue.c -o queue.o
clean:
    rm asn4 Output.txt
