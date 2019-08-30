CC = gcc
CFLAGS = -g -Wall
LDLIBS = -lm -lcurses
OUTPUT = OneVirtualMachine

OneVirtualMachine: main.o user_interface.o logger.o
	$(CC) $(CFLAGS) main.o user_interface.o logger.o $(LDLIBS) -o $(OUTPUT)
	$(RM) *.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c $(LDLIBS) -o main.o

user_interface.o: user_interface.c user_interface.h
	$(CC) $(CFLAGS) -c user_interface.c $(LDLIBS) -o user_interface.o

logger.o: logger.c logger.h
	$(CC) $(CFLAGS) -c logger.c $(LDLIBS) -o logger.o

clean:
	$(RM) $(OUTPUT)
	$(RM) *.o
