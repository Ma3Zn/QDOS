USEFUL_FLAGS = 

INC_DIR = ./include/
SRC_DIR = ./source/
OBJ_DIR = ./object/

OBJS :=  evolution.o simulation.o measure.o
OBJS := $(addprefix $(OBJ_DIR), $(OBJS))

CC_DEBUG_FLAGS = -DDEBUG
CCFLAGS = -Werror -Wall -Wextra -Wno-unused-parameter -c 
CC = gcc

.PHONY: build debug clean

build : build_compile

debug : clean debug_compile

build_compile : CCFLAGS += -O2
build_compile : $(OBJS)

debug_compile : CCFLAGS += -g3 $(CC_DEBUG_FLAGS)
debug_compile : $(OBJS)

clean :
	rm $(OBJ_DIR)*.o

$(OBJ_DIR)evolution.o : $(SRC_DIR)evolution.c $(INC_DIR)evolution.h
	$(CC) $(CCFLAGS) $(SRC_DIR)evolution.c
	mv evolution.o $@

$(OBJ_DIR)simulation.o : $(SRC_DIR)simulation.c $(INC_DIR)simulation.h
	$(CC) $(CCFLAGS) $(SRC_DIR)simulation.c
	mv simulation.o $@

$(OBJ_DIR)measure.o : $(SRC_DIR)measure.c $(INC_DIR)measure.h
	$(CC) $(CCFLAGS) $(SRC_DIR)measure.c
	mv measure.o $@