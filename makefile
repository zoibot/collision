CC = g++
CFLAGS = -g -O2
OBJECTS = main.o graphics.o object.o object_manager.o text.o vector.o
LIBS = -lsfml-window -lsfml-system -lsfml-graphics -lGL

default: col

col : $(OBJECTS)
	$(CC) $(CFLAGS) $(LIBS) $(OBJECTS) -o col

%.o : %.c
	$(CC) $(CFLAGS) -c $<

%.c : %.h
