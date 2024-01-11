CC=gcc
CFLAGS=-Wall
DEBUGFLAGS=-g
OBJDIR=./obj
BINDIR=./bin
SRCDIR=./src

# List of object files separated by spaces
OBJS=$(OBJDIR)/main.o $(OBJDIR)/menu.o $(OBJDIR)/carte.o $(OBJDIR)/score.o $(OBJDIR)/autoplayer.o $(OBJDIR)/bandeau.o

# Default target
all: $(BINDIR)/game

# Debug target
debug: CFLAGS += $(DEBUGFLAGS)
debug: $(BINDIR)/game

# Linking the program
$(BINDIR)/game: $(OBJS)
	$(CC) $^ -o $@ -lncurses

# Compiling source files to object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Cleaning up the build
clean:
	rm -f $(OBJDIR)/*.o $(BINDIR)/game

.PHONY: all debug clean

