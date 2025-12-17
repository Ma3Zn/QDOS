USEFUL_FLAGS = 

INC_DIR = ./include/
SRC_DIR = ./source/
OBJ_DIR = ./object/

OBJS :=  custom_logic.o
OBJS := $(addprefix $(OBJ_DIR), $(OBJS))

CC_DEBUG_FLAGS = -DDEBUG
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

$(OBJ_DIR)custom_logic.o : $(SRC_DIR)custom_logic.c $(INC_DIR)custom_logic.h
	$(CC) $(CCFLAGS) $(SRC_DIR)custom_logic.c
	mv custom_logic.o $@