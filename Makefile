CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11
LDLIBS = -lm

TARGET = build/asciidonut
SRC = src/main.c

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDLIBS)

clean:
	rm -f $(TARGET)

