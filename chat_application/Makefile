CC = gcc
CFLAGS = -Iinclude -Wall
LDFLAGS = -lpthread   
TARGET = app         

SRC = src/cli.c \
      src/connection.c \
      src/network.c \
      src/utils.c \
      src/main.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
      
clean:
	rm -f $(TARGET)

.PHONY: clean