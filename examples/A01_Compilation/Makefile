CC := gcc

STD := -std=c11

WARNINGS := -Wall -Wextra -Werror

CFLAGS := $(STD) $(WARNINGS)

TARGET := program
OBJECTS := main.o math.o

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

main.o: main.c math.h
	$(CC) $(CFLAGS) -c main.c -o main.o

math.o: math.c math.h
	$(CC) $(CFLAGS) -c math.c -o math.o

clean:
	rm -f $(OBJECTS) $(TARGET)