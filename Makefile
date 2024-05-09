CC = gcc

CFLAGS = -Wall -Werror -std=c99
OBJS = bmp.o transformations.o main.o
TARGET = bmp

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

bmp.o: bmp.c bmp.h
	$(CC) $(CFLAGS) -c bmp.c

transformations.o: transformations.c transformations.h
	$(CC) $(CFLAGS) -c transformations.c

main.o: main.c bmp.h transformations.h
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f $(OBJS) $(TARGET)
