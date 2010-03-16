CC = g++
CFLAGS = -g -O2
OBJECTS = main.o graphics.o object.o object_manager.o vector.o
LIBS = -lSDL -lGL

default: col

col : $(OBJECTS)
	$(CC) $(CFLAGS) $(LIBS) $(OBJECTS) -o col

%.o : %.c
	$(CC) $(CFLAGS) -c $<

%.c : %.h

