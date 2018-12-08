CC = gcc
CFLAGS = -g

SOURCE = huffman.c
TARGET = mp3_20171635
OBJ = $(SOURCE:.c=.o)

$(TARGET) : $(OBJ) 
		$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

debug : $(TARGET)
		gdb $(TARGET)

clean :
		rm $(OBJ)
