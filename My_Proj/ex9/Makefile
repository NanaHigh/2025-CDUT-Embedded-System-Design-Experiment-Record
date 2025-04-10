.PHONY: clean all
CC = arm-none-linux-gnueabi-gcc
CFLAGS = -Wall -I.
DIR = ./build
TFTP_DIR = /srv/tftp
TARGETS = ex9_1 ex9_2 ex9_3

# 每个目标对应的源文件
SRC_ex9_1 = ex9_1.c uart_init.c
SRC_ex9_2 = ex9_2.c uart_init.c
SRC_ex9_3 = ex9_3.c uart_init.c

# 生成每个目标对应的对象文件列表
OBJ_ex9_1 = $(addprefix $(DIR)/, $(SRC_ex9_1:.c=.o))
OBJ_ex9_2 = $(addprefix $(DIR)/, $(SRC_ex9_2:.c=.o))
OBJ_ex9_3 = $(addprefix $(DIR)/, $(SRC_ex9_3:.c=.o))

all: $(TARGETS)

ex9_1: $(OBJ_ex9_1)
	$(CC) -o $@ $^ $(CFLAGS)
	sudo cp $@ $(TFTP_DIR)/

ex9_2: $(OBJ_ex9_2)
	$(CC) -o $@ $^ $(CFLAGS)
	sudo cp $@ $(TFTP_DIR)/

ex9_3: $(OBJ_ex9_3)
	$(CC) -o $@ $^ $(CFLAGS)
	sudo cp $@ $(TFTP_DIR)/

$(DIR)/%.o: %.c | $(DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

$(DIR):
	mkdir -p $@

clean:
	rm -rf $(DIR) $(TARGETS)
	sudo rm -f $(addprefix $(TFTP_DIR)/, $(TARGETS))