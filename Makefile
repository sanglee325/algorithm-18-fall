CC = gcc
CFLAGS = -g

SOURCE = main.c source.c
TARGET = mp2_20171635
OBJ = $(SOURCE:.c=.o)

$(TARGET) : $(OBJ) sort.h
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

debug : $(TARGET)
	gdb $(TARGET)

clean :
	rm $(OBJ)
