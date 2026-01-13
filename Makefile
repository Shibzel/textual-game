# The compiler to use
CC = gcc

# Compiler flags (warnings, debugging info)
CFLAGS = -Wall -Wextra -g

# The name of the final executable
TARGET = build/jeu

SRC = main.c parser.c file_loader.c

OBJ = $(SRC:.c=.o)

# Default target: builds the application
all: $(TARGET)


$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -f $(OBJ) $(TARGET)