CC=gcc
INCLUDE_FLAGS=-Iinc
DEBUG_FLAGS=
CFLAGS=-g
OBJ_DIR=obj
PROGRAM=calc.a

OBJ = $(OBJ_DIR)/calc.o \
	  $(OBJ_DIR)/csv_check.o \
	  $(OBJ_DIR)/polishnotation.o \
	  $(OBJ_DIR)/ptime.o \
	  $(OBJ_DIR)/validate.o

.PHONY = clean all
	
all : $(OBJ_DIR) $(OBJ) $(PROGRAM)

$(OBJ_DIR) :
	mkdir $(OBJ_DIR)

$(OBJ_DIR)/%.o : calc/%.c
	$(CC) $(CFLAGS) $(INCLUDE_FLAGS) -o $@ -c $^ 

$(OBJ_DIR)/%.o : csv_check/%.c
	$(CC) $(CFLAGS) $(INCLUDE_FLAGS) -o $@ -c $^

$(PROGRAM): $(OBJ)
	$(CC) $(OBJ) -o $(PROGRAM)

clean:
	rm -rf $(OBJ_DIR)
