.PHONY: clean all
CC = arm-none-linux-gnueabi-gcc
HOST_CC = gcc
CFLAGS = -Wall
DIR = ./build
TFTP_DIR = /srv/tftp
SERVER_TARGET = ex11_server
CLIENT_TARGET = ex11_client

# 明确分开服务器端和客户端的源文件
SRC_SERVER = ex11_server.c
SRC_CLIENT = ex11_client.c

# 分开生成不同架构的对象文件
OBJ_SERVER = $(addprefix $(DIR)/, $(SRC_SERVER:.c=.o))
OBJ_CLIENT = $(addprefix $(DIR)/, $(SRC_CLIENT:.c=.o))

all: $(SERVER_TARGET) $(CLIENT_TARGET)

# 服务器端构建规则（ARM架构）
$(SERVER_TARGET): $(OBJ_SERVER)
	$(CC) -o $@ $^ $(CFLAGS)
	sudo cp $@ $(TFTP_DIR)/

# 客户端构建规则（x86架构）
$(CLIENT_TARGET): $(SRC_CLIENT)
	$(HOST_CC) $(CFLAGS) $^ -o $@

# 服务器端对象文件编译规则
$(DIR)/%.o: %.c | $(DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

$(DIR):
	mkdir -p $@

clean:
	rm -rf $(DIR) $(SERVER_TARGET) $(CLIENT_TARGET)
	sudo rm -f $(TFTP_DIR)/$(SERVER_TARGET)