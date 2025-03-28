.PHONY: clean all
CC = arm-none-linux-gnueabi-gcc
CFLAGS = -Wall -I. -lpthread
DIR = ./build
TFTP_DIR = /srv/tftp
TARGETS = ex10

SRC_TRS = ex10.c uart_init.c

OBJ_TRS = $(addprefix $(DIR)/, $(SRC_TRS:.c=.o))

all: $(TARGETS)

ex10: $(OBJ_TRS)
	$(CC) -o $@ $^ $(CFLAGS)
	sudo cp $@ $(TFTP_DIR)/

$(DIR)/%.o: %.c | $(DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

$(DIR):
	mkdir -p $@

clean:
	rm -rf $(DIR) $(TARGETS)
	sudo rm -f $(TFTP_DIR)/ex10