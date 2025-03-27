.PHONY: clean all
CC = arm-none-linux-gnueabi-gcc
CFLAGS = -Wall
DIR = ./build
TFTP_DIR = /srv/tftp
SRC = ex5.c
OBJS = $(addprefix $(DIR)/, $(SRC:.c=.o))

all: $(OBJS)
$(OBJS): $(SRC) | dir
	$(CC) $(SRC) -o $@ $(CFLAG)
	sudo cp $(OBJS) $(TFTP_DIR)
dir:
	mkdir -p $(DIR)
clean:
	rm -rf $(DIR)
	sudo rm -rf $(TFTP_DIR)/$(SRC:.c=.o)