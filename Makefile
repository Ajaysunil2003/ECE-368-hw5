CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LIBS = -lm
TARGET = a5
SRC = overlap.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LIBS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET) points.txt

