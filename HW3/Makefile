CC = g++
CFLAGS = -g

SOURCE = ppHuff.cpp
TARGET = mp3_20171635
OBJ = $(SOURCE:.cpp=.o)

$(TARGET) : $(OBJ) 
		$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

debug : $(TARGET)
		gdb $(TARGET)

clean :
		rm $(OBJ)
