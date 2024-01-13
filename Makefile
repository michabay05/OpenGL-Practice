# Modified from michabay05/FortuneChess
#  - https://github.com/michabay05/FortuneChess/blob/main/Makefile

SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = bin
LIBDIR = lib
BIN_NAME = main

COMP = clang
COMPFLAGS = -Wall -Wextra -std=c17 -I$(INCDIR) -ggdb
LIBFLAGS=-lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lm

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))
BINARY = $(BINDIR)/$(BIN_NAME)

.PHONY: all clean

all: compile run

compile: setup $(BINARY)

run:
	$(BINARY)

setup:
	@mkdir -p $(OBJDIR)
	@mkdir -p $(BINDIR)

$(BINARY): $(OBJECTS)
	$(COMP) $^ -o $@ -L$(LIBDIR) $(LIBFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(COMP) $(COMPFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR)/* $(BINDIR)/*
