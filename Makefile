CC = gcc
CFLAGS = -g

SOURCE = mp2_20171635.c source.c
TARGET = mp2_20171635
OBJ = $(SOURCE:.c=.o)

$(TARGET) : $(OBJ) Algo2.h
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

debug : $(TARGET)
	gdb $(TARGET)

clean :
	rm $(OBJ)
