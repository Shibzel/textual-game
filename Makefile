# The compiler to use for Linux
CC = gcc
# The cross-compiler for Windows https://stackoverflow.com/questions/38786014/how-to-compile-executable-for-windows-with-gcc-with-linux-subsystem 
WINC = x86_64-w64-mingw32-gcc
WIN_LIBS = -lwinmm
# Compiler flags
CFLAGS = -Wall -Wextra -g 

# Output names
TARGET = build/jeu
WIN_TARGET = build/jeu.exe

SRC = main.c parser.c save_manager.c inputs.c files.c
OBJ = $(SRC:.c=.o)

#  a separate list of object files for Windows to avoid conflicts
WIN_OBJ = $(SRC:.c=.win.o)

# Default target: builds for Linux
all: $(TARGET)

# Linux build
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

# Windows build target
windows: $(WIN_TARGET)

$(WIN_TARGET): $(WIN_OBJ)
	$(WINC) $(WIN_OBJ) -o $(WIN_TARGET) $(WIN_LIBS)

# Compile source files into Linux object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile source files into Windows object files
%.win.o: %.c
	$(WINC) $(CFLAGS) -c $< -o $@  -lwinmm

clean:
	rm -f $(OBJ) $(WIN_OBJ) $(TARGET) $(WIN_TARGET)