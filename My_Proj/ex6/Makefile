.PHONY: clean all
CC = arm-none-linux-gnueabi-gcc
CFLAGS = -Wall -I.
DIR = ./build
TFTP_DIR = /srv/tftp
TARGETS = uart_trs uart_rcv

SRC_TRS = uart_trs.c uart_init.c
SRC_RCV = uart_rcv.c uart_init.c

OBJ_TRS = $(addprefix $(DIR)/, $(SRC_TRS:.c=.o))
OBJ_RCV = $(addprefix $(DIR)/, $(SRC_RCV:.c=.o))

all: $(TARGETS)

uart_trs: $(OBJ_TRS)
	$(CC) -o $@ $^ $(CFLAGS)
	sudo cp $@ $(TFTP_DIR)/

uart_rcv: $(OBJ_RCV)
	$(CC) -o $@ $^ $(CFLAGS)
	sudo cp $@ $(TFTP_DIR)/

$(DIR)/%.o: %.c | $(DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

$(DIR):
	mkdir -p $@

clean:
	rm -rf $(DIR) $(TARGETS)
	sudo rm -f $(TFTP_DIR)/uart_trs $(TFTP_DIR)/uart_rcv