main:
    gcc asn4.c queue.o -o asn4
queue.o: queue.c queue.h
    gcc queue.c -o queue.o
clean:
    rm asn4 Output.txt
