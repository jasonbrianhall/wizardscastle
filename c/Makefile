# Makefile for Wizard's Castle

# Compiler
CC = gcc

# Source files
SRCS = main.c wizards-castle.c

# Output executable
TARGET = wizardscastle

# Compiler flags
CFLAGS = -Wall -Wextra
DEBUGFLAGS = -g -fsanitize=address

# Default target
all: $(TARGET)

# Rule to build the executable without debug symbols
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

# Debug target to build with debug symbols
debug: CFLAGS += $(DEBUGFLAGS)
debug: $(TARGET)

# Clean target to remove generated files
clean:
	rm -f $(TARGET)
