PROJECT_NAME = MusicGen
CC = gcc
CFLAGS = -Wall -g
LDFLAGS =
OBJECTS = \
	$(OBJDIR)/parser.o\
	$(OBJDIR)/read.o\
	$(OBJDIR)/midi.o 
OBJDIR = Obj
VPATH = Src

.PHONY: all

all: $(PROJECT_NAME) 

$(PROJECT_NAME): $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: %.c %.h
	$(CC) $< -c -o $@ $(CFLAGS) 

.PHONY: clean

clean:
	@rm $(OBJDIR)/*.o 
	@rm $(PROJECT_NAME)
