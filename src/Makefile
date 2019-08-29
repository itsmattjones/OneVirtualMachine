CC = gcc
CFLAGS = -g -Wall
LDLIBS = -lm -lcurses
OUTPUT = OneVirtualMachine

OneVirtualMachine: main.o debug_tools.o user_interface.o
	$(CC) $(CFLAGS) main.o debug_tools.o user_interface.o $(LDLIBS) -o $(OUTPUT)
	$(RM) *.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c $(LDLIBS) -o main.o

debug_tools.o: debug_tools.c debug_tools.h
	$(CC) $(CFLAGS) -c debug_tools.c $(LDLIBS) -o debug_tools.o

user_interface.o: user_interface.c user_interface.h
	$(CC) $(CFLAGS) -c user_interface.c $(LDLIBS) -o user_interface.o

clean:
	$(RM) $(OUTPUT)
