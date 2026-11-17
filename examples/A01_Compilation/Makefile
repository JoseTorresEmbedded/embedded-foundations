CC := gcc

STD := -std=c11

WARNINGS := -Wall -Wextra -Werror

DEPFLAGS := -MMD -MP

BUILD ?= debug

ifeq ($(BUILD),debug)
    OPTFLAGS := -O0 -g3
else ifeq ($(BUILD),release)
    OPTFLAGS := -O2 -DNDEBUG
else
    $(error BUILD='$(BUILD)' no es válido; usa debug o release)
endif

CFLAGS := $(STD) $(WARNINGS) $(DEPFLAGS) $(OPTFLAGS)

BUILD_ROOT := build
ifeq ($(strip $(BUILD_ROOT)),)
$(error BUILD_ROOT está vacía; abortando)
endif

BUILD_DIR := $(BUILD_ROOT)/$(BUILD)

SRCS := main.c math.c

TARGET := $(BUILD_DIR)/program
OBJECTS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRCS))
DEPS := $(OBJECTS:.o=.d)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $^ -o $@

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_ROOT)

-include $(DEPS)
