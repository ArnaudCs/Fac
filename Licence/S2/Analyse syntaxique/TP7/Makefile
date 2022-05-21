BIN=bin/tradauto
SRC=tradauto.l 
DIR_GUARD=@mkdir -p $(@D)
CC=gcc
CFLAGS=-Wall -Iinclude

default: $(BIN)

%.c: %.l
	flex $^

bin/tradauto: $(SRC:%.l=%.c)
	$(DIR_GUARD)
	$(CC) $(CFLAGS) -o $@ lex.yy.c arbin.o
