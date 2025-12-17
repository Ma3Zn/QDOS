USEFUL_FLAGS = -DDEBUG_BLTN

INC_DIR = ./include/
SRC_DIR = ./source/
OBJ_DIR = ./object/

OBJS :=  gate_built_in.o
OBJS := $(addprefix $(OBJ_DIR), $(OBJS))

CC_DEBUG_FLAGS = -DDEBUG $(USEFUL_FLAGS)
CCFLAGS = -Werror -Wall -Wextra -c 
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

$(OBJ_DIR)gate_built_in.o : $(SRC_DIR)gate_built_in.c $(INC_DIR)gate_built_in.h
	$(CC) $(CCFLAGS) $(SRC_DIR)gate_built_in.c
	mv gate_built_in.o $@