.PHONY: clean
CC = arm-none-linux-gnueabi-gcc
DIR = ./build
FTFP_DIR = /srv/tftp
SRC = ex4.c
OBJS = $(addprefix $(DIR)/, $(SRC:.c=.o))

all: $(OBJS)
$(OBJS): $(SRC) | dir
	$(CC) -c $(SRC) -o $@
	sudo cp $(OBJS) $(FTFP_DIR)
dir:
	mkdir -p $(DIR)
clean:
	rm -rf $(DIR)