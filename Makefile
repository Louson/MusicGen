PROJECT_NAME = midi_parser
CC = gcc
CFLAGS = -Wall -g
LDFLAGS =
OBJECTS = main.o parser.o read.o

all: $(PROJECT_NAME) 

$(PROJECT_NAME): $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS)

%.o: %.c %.h
	$(CC) $< -c -o $@ $(CFLAGS) 

.PHONY: clean

clean:
	@rm *.o 
	@rm $(PROJECT_NAME)
