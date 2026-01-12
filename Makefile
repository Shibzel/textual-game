# The compiler to use
CC = gcc

# Compiler flags (warnings, debugging info)
CFLAGS = -Wall -Wextra -g

# The name of the final executable
TARGET = build/jeu

SRC = main.c file_loader.c

OBJ = $(SRC:.c=.o)

# Default target: builds the application
all: $(TARGET)

# Note: The indentation below MUST be a TAB character
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

# Compile source files into object files
# Note: The indentation below MUST be a TAB character
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -f $(OBJ) $(TARGET)