USEFUL_FLAGS = 

INC_DIR = ./include/
SRC_DIR = ./source/
OBJ_DIR = ./object/

OBJS :=  parser_interfaces.o simulation_interfaces.o
OBJS := $(addprefix $(OBJ_DIR), $(OBJS))

CC_DEBUG_FLAGS = -DDEBUG
CCFLAGS = -Werror -Wall -Wextra -Wno-implicit-fallthrough -c 
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

$(OBJ_DIR)parser_interfaces.o : $(SRC_DIR)parser_interfaces.c $(INC_DIR)parser_interfaces.h
	$(CC) $(CCFLAGS) $(SRC_DIR)parser_interfaces.c
	mv parser_interfaces.o $@

$(OBJ_DIR)simulation_interfaces.o : $(SRC_DIR)simulation_interfaces.c $(INC_DIR)simulation_interfaces.h
	$(CC) $(CCFLAGS) -Wno-implicit-function-declaration $(SRC_DIR)simulation_interfaces.c
	mv simulation_interfaces.o $@
