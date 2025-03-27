.PHONY: clean all
CC = gcc
HOST_CC = gcc
CFLAGS = -Wall -lpthread
SERVER_TARGET = ex12_server
CLIENT_TARGET = ex12_client

# 明确分开服务器端和客户端的源文件
SRC_SERVER = ex12_server.c
SRC_CLIENT = ex12_client.c

# 分开生成不同架构的对象文件
OBJ_SERVER = $(SRC_SERVER:.c=.o)
OBJ_CLIENT = $(SRC_CLIENT:.c=.o)

all: $(SERVER_TARGET) $(CLIENT_TARGET)

# 服务器端构建规则
$(SERVER_TARGET): $(OBJ_SERVER) | $(DIR)
	$(CC) -o $@ $^ $(CFLAGS)

# 客户端构建规则
$(CLIENT_TARGET): $(SRC_CLIENT) | $(DIR)
	$(HOST_CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf $(DIR) $(CLIENT_TARGET) $(SERVER_TARGET)