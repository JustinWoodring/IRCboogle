# the compiler: gcc for C program, define as g++ for C++
CC = gcc
SRC = src/
# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall

# the build target executable:
TARGET = ircb

all: $(TARGET)

$(TARGET):
	$(CC) $(SRC)ircb.c $(SRC)search.c $(SRC)connection.c -o $(TARGET)

clean:
	$(RM) $(TARGET)
