.PHONY: clean
CC = gcc
DIR = ./build
SRC = stat.c ex3.c
OBJS = $(addprefix $(DIR)/, $(SRC:.c=.o))

main: $(OBJS)
	$(CC) $(OBJS) -o $(DIR)/main.o
$(DIR)/ex3.o: ex3.c | dir
	$(CC) -c ex3.c -o $@
$(DIR)/stat.o: stat.c | dir
	$(CC) -c stat.c -o $@
dir:
	mkdir -p $(DIR)
clean:
	rm -rf $(DIR)