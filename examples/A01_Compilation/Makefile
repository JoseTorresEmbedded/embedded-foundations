CC := gcc

STD := -std=c11

WARNINGS := -Wall -Wextra -Werror

CFLAGS := $(STD) $(WARNINGS)

TARGET := program
OBJECTS := main.o math.o

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $^ -o $@

main.o math.o: math.h

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
