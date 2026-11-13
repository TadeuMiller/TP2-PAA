CC = gcc
CFLAGS = -std=c99 -O2 -Wall -Wextra -Wpedantic
CPPFLAGS = -Iinclude
TARGET = programa
SOURCES = src/main.c src/caverna.c src/dp.c

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SOURCES) include/caverna.h include/dp.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $(SOURCES) -o $(TARGET)

clean:
	$(RM) $(TARGET) $(TARGET).exe
