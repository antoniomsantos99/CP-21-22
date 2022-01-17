TARGET = prog
LIBS = -lm -lpapi -L/share/apps/papi/6.0.0/lib
CC = gcc
CFLAGS = -Wall -fopenmp -g -I/share/apps/papi/6.0.0/include

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	-rm -f *.o
	-rm -f $(TARGET)
