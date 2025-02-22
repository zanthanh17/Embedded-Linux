

CUR_DIR := .
BIN_DIR := $(CUR_DIR)/bin
INC_DIR := $(CUR_DIR)/inc
OBJ_DIR := $(CUR_DIR)/obj
SRC_DIR := $(CUR_DIR)/src
LIB_DIR := $(CUR_DIR)/lib
LIB_STATIC_DIR := $(LIB_DIR)/static
LIB_SHARED_DIR := $(LIB_DIR)/shared

C_FLAGS := -I $(INC_DIR)

create_objs:
	gcc -c -fPIC $(SRC_DIR)/fmath.c -o $(OBJ_DIR)/fmath.o -I $(INC_DIR)
	gcc -c $(SRC_DIR)/main.c -o $(OBJ_DIR)/main.o -I $(INC_DIR)
create_shared_libs:
	gcc -shared $(OBJ_DIR)/hellfmathoThanh.o -o $(LIB_SHARED_DIR)/libfmath.so

create_static_libs:
	ar rcs $(LIB_STATIC_DIR)/libfmath.a $(OBJ_DIR)/fmath.o

all: create_objs create_shared_libs create_static_libs
	gcc  $(OBJ_DIR)/main.o -L$(LIB_SHARED_DIR) -lfmath -o $(BIN_DIR)/use-shared-library
	gcc   $(OBJ_DIR)/main.o  -L$(LIB_STATIC_DIR) -lfmath -o $(BIN_DIR)/statically-linked

clean:
	rm -rf $(OBJ_DIR)/*.o
	rm -rf $(LIB_SHARED_DIR)/*.so
	rm -rf $(LIB_STATIC_DIR)/*.a
	rm -rf $(BIN_DIR)/use-shared-library

