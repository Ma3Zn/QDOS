USEFUL_FLAGS = 

INC_DIR = ./include/
SRC_DIR = ./source/
OBJ_DIR = ./object/

OBJS :=  eigen.o gate.o global.o input.o output.o physical_system.o pulse.o rotation.o spin_operators.o tensor.o
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

$(OBJ_DIR)eigen.o : $(SRC_DIR)eigen.c $(INC_DIR)eigen.h
	$(CC) $(CCFLAGS) $(SRC_DIR)eigen.c
	mv eigen.o $@

$(OBJ_DIR)gate.o : $(SRC_DIR)gate.c $(INC_DIR)gate.h
	$(CC) $(CCFLAGS) $(SRC_DIR)gate.c
	mv gate.o $@

$(OBJ_DIR)global.o : $(SRC_DIR)global.c $(INC_DIR)global.h
	$(CC) $(CCFLAGS) $(SRC_DIR)global.c
	mv global.o $@

$(OBJ_DIR)input.o : $(SRC_DIR)input.c $(INC_DIR)input.h
	$(CC) $(CCFLAGS) $(SRC_DIR)input.c
	mv input.o $@

$(OBJ_DIR)output.o : $(SRC_DIR)output.c $(INC_DIR)output.h
	$(CC) $(CCFLAGS) $(SRC_DIR)output.c
	mv output.o $@

$(OBJ_DIR)physical_system.o : $(SRC_DIR)physical_system.c $(INC_DIR)physical_system.h
	$(CC) $(CCFLAGS) $(SRC_DIR)physical_system.c
	mv physical_system.o $@

$(OBJ_DIR)pulse.o : $(SRC_DIR)pulse.c $(INC_DIR)pulse.h
	$(CC) $(CCFLAGS) $(SRC_DIR)pulse.c
	mv pulse.o $@

$(OBJ_DIR)rotation.o : $(SRC_DIR)rotation.c $(INC_DIR)rotation.h
	$(CC) $(CCFLAGS) $(SRC_DIR)rotation.c
	mv rotation.o $@

$(OBJ_DIR)spin_operators.o : $(SRC_DIR)spin_operators.c $(INC_DIR)spin_operators.h
	$(CC) $(CCFLAGS) $(SRC_DIR)spin_operators.c
	mv spin_operators.o $@

$(OBJ_DIR)tensor.o : $(SRC_DIR)tensor.c $(INC_DIR)tensor.h
	$(CC) $(CCFLAGS) $(SRC_DIR)tensor.c
	mv tensor.o $@