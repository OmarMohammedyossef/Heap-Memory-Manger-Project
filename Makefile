CC := gcc
CFLAGS:= -g -Wall
DYN:= -fPIC
SH:= -shared 
OPSH:= -ldl 
ST:= ar
OPS:= -rs

SRC:= $(shell ls *.c)
OBJ:= $(SRC:.c=.o)
INC:= $(shell ls *.h)
TARGET:= a


.PHONY: all clean build run 

all: clean build run

dynamic: clean DYNAMIC run

static: clean STATIC run


STATIC:
	@$(CC) -c  $(SRC) 
	@$(ST) $(OPS) libhmm.so *.o  
	@$(CC) -c  main.c  -I./mylib
	@$(CC) -o $(TARGET).exe main.o -L./mylib 
	@echo "[Makefile][build] : Compiled successfully."

DYNAMIC:
	@$(CC) -c $(DYN) $(SRC) 
	@$(CC) $(SH) -o lib.so *.o $(OPSH)   
	@$(CC) -o $(TARGET).exe main.c
	@LD_LIBRARY_PATH=`realpath lib.so`
	@echo "[Makefile][build] : Compiled successfully."

build: $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o $(TARGET).exe
	@echo "[Makefile][build] : Compiled successfully."

$(OBJ): %.o: %.c $(INC)
	@$(CC) $(CFLAGS) -c $< -o $@

run: 
	./$(TARGET).exe
clean:
	@rm -f *.o 
	@rm -f *.exe
	@echo "[Makefile][clean] : Cleaned successfully."