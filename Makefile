TARGET := main

CC := clang++
CFLAGS := -std=c++11
LIB := -lpng

SRC := $(shell ls *.cpp)
OBJ := $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
		$(CC) $(CFLAGS) $(LIB) -o $(TARGET) $(SRC)

.cpp.o:
		$(CC) $(CFLAGS) -c $<

clean:
		rm $(OBJ)
		rm $(TARGET)
