CC := gcc

STD := -std=c11

WARNINGS := -Wall -Wextra -Werror

DEPFLAGS := -MMD -MP

CFLAGS := $(STD) $(WARNINGS) $(DEPFLAGS)

BUILD_DIR := build
ifeq ($(strip $(BUILD_DIR)),)
$(error BUILD_DIR está vacía; abortando)
endif

TARGET := $(BUILD_DIR)/program
OBJECTS := $(BUILD_DIR)/main.o $(BUILD_DIR)/math.o
DEPS := $(OBJECTS:.o=.d)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $^ -o $@

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

-include $(DEPS)
