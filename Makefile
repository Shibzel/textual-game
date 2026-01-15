# The compiler to use
CC = gcc

# Compiler flags (warnings, debugging info, include path)
CFLAGS = -Wall -Wextra -g -Isrc

# The name of the final executable
TARGET = build/jeu

# Source files (now in ./src)
SRC = src/main.c src/parser.c src/save_manager.c src/inputs.c src/files.c

# Object files (built in ./src)
OBJ = $(SRC:.c=.o)

# Default target: builds the application
all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p build
	$(CC) $(OBJ) -o $(TARGET)

# Compile source files into object files
%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ) $(TARGET)