# Makefile for Wizard's Castle

# Compiler
CC = gcc

# Source files
SRCS = main.c wizards-castle.c

# Output executable
TARGET = wizardscastle

# Compiler flags
CFLAGS = -Wall -Wextra

# Default target
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

# Clean target to remove generated files
clean:
	rm -f $(TARGET)
