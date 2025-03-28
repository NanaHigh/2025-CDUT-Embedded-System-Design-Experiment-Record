.PHONY: clean all
CC = arm-none-linux-gnueabi-gcc
CFLAGS = -Wall -I.
DIR = ./build
TFTP_DIR = /srv/tftp
TARGETS = ex8

SRC_TRS = ex8.c uart_init.c

OBJ_TRS = $(addprefix $(DIR)/, $(SRC_TRS:.c=.o))

all: $(TARGETS)

ex8: $(OBJ_TRS)
	$(CC) -o $@ $^ $(CFLAGS)
	sudo cp $@ $(TFTP_DIR)/

$(DIR)/%.o: %.c | $(DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

$(DIR):
	mkdir -p $@

clean:
	rm -rf $(DIR) $(TARGETS)
	sudo rm -f $(TFTP_DIR)/ex8