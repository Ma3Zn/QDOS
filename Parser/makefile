USEFUL_FLAGS = -Wcounterexamples -DDEBUG_PARSER -DDEBUG_LEXER

YFLAGS = -d 
LFLAGS = -t

INC_DIR = ./include/
SRC_DIR = ./source/
OBJ_DIR = ./object/

OBJS :=  parser.o lexer.o parser_support.o lexer_support.o
OBJS := $(addprefix $(OBJ_DIR), $(OBJS))

CC_DEBUG_FLAGS = -DDEBUG
CCFLAGS = -Werror -Wall -Wextra -Wno-unused-function -c 
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

$(OBJ_DIR)lexer_support.o : $(SRC_DIR)lexer_support.c $(INC_DIR)lexer_support.h
	$(CC) $(CCFLAGS) $(SRC_DIR)lexer_support.c
	mv lexer_support.o $@

$(OBJ_DIR)parser_support.o : $(SRC_DIR)parser_support.c $(INC_DIR)parser_support.h
	$(CC) $(CCFLAGS) $(SRC_DIR)parser_support.c
	mv parser_support.o $@

$(OBJ_DIR)lexer.o : $(INC_DIR)lexer.h $(SRC_DIR)lexer.l
	flex $(LFLAGS) $(SRC_DIR)lexer.l > $(SRC_DIR)lexer.c
	$(CC) $(CCFLAGS) $(SRC_DIR)lexer.c
	mv lexer.o $@

$(OBJ_DIR)parser.o : $(SRC_DIR)parser.y
	yacc $(YFLAGS) $(SRC_DIR)parser.y
	mv y.tab.c $(SRC_DIR)parser.c
	mv y.tab.h $(INC_DIR)parser.h
	$(CC) $(CCFLAGS) $(SRC_DIR)parser.c
	mv parser.o $@