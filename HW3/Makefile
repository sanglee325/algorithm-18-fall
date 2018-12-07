CC = g++
CFLAGS = -g

SOURCE = huffman.cpp source.cpp
TARGET = mp3_20171635
OBJ = $(SOURCE:.cpp=.o)

$(TARGET) : $(OBJ) Huffman.h
		$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

debug : $(TARGET)
		gdb $(TARGET)

clean :
		rm $(OBJ)
