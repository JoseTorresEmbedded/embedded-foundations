CC := gcc

STD := -std=c11

WARNINGS := -Wall -Wextra -Werror

DEPFLAGS := -MMD -MP

CFLAGS := $(STD) $(WARNINGS) $(DEPFLAGS)

TARGET := program
OBJECTS := main.o math.o
DEPS := $(OBJECTS:.o=.d)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET) $(DEPS)

-include $(DEPS)
