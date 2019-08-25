CC = gcc

OneVirtualMachine: main.o debug_tools.o
	$(CC) -g -Wall  main.o debug_tools.o -o OneVirtualMachine
	$(RM) *.o

main.o: main.c
	$(CC) -c main.c

debug_tools.o: debug_tools.c debug_tools.h
	$(CC) -c debug_tools.c

clean:
	$(RM) OneVirtualMachine
