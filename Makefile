CC = gcc

TARGET_SOURCE = main.c
TARGET = main
SOURCES = $(wildcard src/*.c)
OBJECTS = $(SOURCES:.c=.o)
INCLUDE_DIR = ./include/
CFLAGS  = -I$(INCLUDE_DIR) -Ofast

all: $(TARGET)

%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(TARGET): $(TARGET_SOURCE) $(OBJECTS)
	$(CC) $(CFLAGS) $(TARGET_SOURCE) $(OBJECTS) -o $@

clean:
	rm -f src/*.o
	rm -f $(TARGET)
	rm -f TSP