CC = gcc
CFLAGS = -g

SOURCE = mss20171635.c
TARGET = mss20171635
OBJ = $(SOURCE:.c=.o)
INPUT = "input"

$(TARGET) : $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

debug : $(TARGET)
	gdb $(TARGET)

clean :
	rm $(OBJ)
