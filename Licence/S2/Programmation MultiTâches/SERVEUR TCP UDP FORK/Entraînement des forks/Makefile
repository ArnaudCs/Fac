########################################
#~ définitions
########################################

# Executables à générer
BIN=bin/fork

# liste des fichiers sources 
SRCS0=fork.c

default: $(BIN)

########################################
#~ regles pour l'executable
########################################

obj/%.o: %.c
	gcc -Wall -Iinclude -c $< -o $@

bin/fork: $(SRCS0:%.c=obj/%.o)
	gcc -o $@ $+

clean:
	rm -f $(BIN) obj/*.o *~
