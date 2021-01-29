CC=gcc
CFLAGS=-Wall -Werror
DEBUGFLAGS=-g -DDEBUG
BIN=race
SRC=src
HDR=includes
OBJ=obj
SRCS=$(wildcard $(SRC)/*.c)
HDRS=$(wildcard $(HDR)/*.h)
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))

all: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(BIN) -I $(HDR)

debug:
	$(CC) $(CFLAGS) $(DEBUGFLAGS) $(SRCS) -o $(BIN) -I $(HDR)

$(OBJ)/%.o: $(SRC)/%.c $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@ -I $(HDR)

$(OBJ):
	mkdir -p $(OBJ)

clean:
	rm -f $(BINS)
	rm -f $(OBJ)/*
	rmdir $(OBJ)
	rm -f $(BIN)
