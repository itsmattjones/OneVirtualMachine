CC = gcc
CFLAGS = -g -Wall
LDLIBS = -lm -lcurses
OUTPUT = OneVirtualMachine

OneVirtualMachine: main.o virtual_machine.o user_interface.o logger.o
	$(CC) $(CFLAGS) main.o virtual_machine.o user_interface.o logger.o $(LDLIBS) -o $(OUTPUT)
	$(RM) *.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c $(LDLIBS) -o main.o

virtual_machine.o: virtual_machine.c
	$(CC) $(CFLAGS) -c virtual_machine.c $(LDLIBS) -o virtual_machine.o

user_interface.o: user_interface.c user_interface.h
	$(CC) $(CFLAGS) -c user_interface.c $(LDLIBS) -o user_interface.o

logger.o: logger.c logger.h
	$(CC) $(CFLAGS) -c logger.c $(LDLIBS) -o logger.o

clean:
	$(RM) $(OUTPUT)
	$(RM) *.o
