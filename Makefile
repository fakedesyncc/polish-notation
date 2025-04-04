CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

SRC_DIR = src
BUILD_DIR = build

SRCS = $(SRC_DIR)/graph.c
OBJS = $(SRCS:.c=.o)
TARGET = $(BUILD_DIR)/graph

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

rebuild: clean all

check: $(TARGET)
	valgrind --tool=memcheck --leak-check=yes ./$(TARGET) "sin(x)"
	cppcheck --enable=all --suppress=missingIncludeSystem $(SRCS)

sanitize: CFLAGS += -fsanitize=address -fsanitize=leak -fsanitize=undefined
sanitize: clean $(TARGET)

format:
	clang-format -i -style="{BasedOnStyle: Google, IndentWidth: 4, ColumnLimit: 110}" $(SRCS)

.PHONY: all clean rebuild check sanitize format
