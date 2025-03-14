.PHONY: clean
CC = arm-none-linux-gnueabi-gcc
DIR = ./build
FTFP_DIR = /srv/tftp
SRC = ex4.c
OBJS = $(addprefix $(DIR)/, $(SRC:.c=.o))

all: $(OBJS)
$(OBJS): ex4.c | dir
	$(CC) -c ex4.c -o $@
	sudo cp $(OBJS) $(FTFP_DIR)
dir:
	mkdir -p $(DIR)
clean:
	rm -rf $(DIR)