SRC = src
BIN = bin
TARGET = main
LIBS = -lSDL2 -lSDL_mixer -lGL -ldl -lm -lgsl

SOURCE = $(wildcard $(SRC)/*.c)
OBJECT = $(patsubst %, $(BIN)/%, $(notdir $(SOURCE:.c=.o))) 

$(BIN)/%.o : $(SRC)/%.c 
	mkdir -p $(@D)
	gcc -g -Wall -c $< -o $@ 

$(BIN)/$(TARGET) : $(OBJECT)
	gcc -o $@ $^ include/glad/src/glad.c $(LIBS)

run :
		bin/./$(TARGET)

clean : 
		rm -r bin 
