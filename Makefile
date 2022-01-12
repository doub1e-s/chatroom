SRC_DIR = src
SERVER_DIR = server
CLIENT_DIR = client
TEST_DIR = test
# 确保生成obj文件的时候指定目录是存在的
DIR_GUARD=mkdir -p $(@D)
OBJ_DIR = build
# g++的对应参数
INC_DIR	= -Iinclude -I3rdparty/uv-cpp/uv/include -I3rdparty/uv-cpp/libuv1.30.0/include
LIB_DIR = -L3rdparty/lib
LIB_NAME= -luv_cpp -luv -pthread


# SOURCES := $(wildcard $(SRC_DIR)/$(SERVER_DIR)/*.cpp $(SRC_DIR)/$(CLIENT_DIR)/.*cpp $(SRC_DIR))
# SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
#https://stackoverflow.com/questions/4036191/sources-from-subdirectories-in-makefile
# 将src内的所有.cpp文件，转换成对应的.o文件，并加上前缀，从而将obj文件输入至指定的目录(这里为build)
SERVER_SOURCES := $(wildcard $(SRC_DIR)/$(SERVER_DIR)/*.cpp)
SERVER_OBJ := $(patsubst $(SRC_DIR)/%,%,$(SERVER_SOURCES))
SERVER_OBJ := $(patsubst %.cpp,%.o,$(SERVER_OBJ))
SERVER_OBJ := $(addprefix $(OBJ_DIR)/,$(SERVER_OBJ))

CLIENT_SOURCES := $(wildcard $(SRC_DIR)/$(CLIENT_DIR)/*.cpp)
CLIENT_OBJ := $(patsubst $(SRC_DIR)/%,%,$(CLIENT_SOURCES))
CLIENT_OBJ := $(patsubst %.cpp,%.o,$(CLIENT_OBJ))
CLIENT_OBJ := $(addprefix $(OBJ_DIR)/,$(CLIENT_OBJ))

TEST_SOURCES := $(wildcard $(SRC_DIR)/$(TEST_DIR)/*.cpp)
TEST_OBJ := $(patsubst $(SRC_DIR)/%,%,$(TEST_SOURCES))
TEST_OBJ := $(patsubst %.cpp,%.o,$(TEST_OBJ))
TEST_OBJ := $(addprefix $(OBJ_DIR)/,$(TEST_OBJ))


# VPATH=src:include

# 设置编译相关内容
CC      = g++
CPPFLAGS= -Wall -std=c++11 $(INC_DIR) -c -g -o

# build client and server. TODO: separate this makefile into two makefile
all : client server

client : $(OBJ_DIR)/$(CLIENT_DIR)/client
$(OBJ_DIR)/$(CLIENT_DIR)/client: $(CLIENT_OBJ)
	@$(DIR_GUARD)
	@$(CC) -o $@ $(CLIENT_OBJ) $(LIB_DIR) $(LIB_NAME) 

server : $(OBJ_DIR)/$(SERVER_DIR)/server
$(OBJ_DIR)/$(SERVER_DIR)/server: $(SERVER_OBJ)
	@$(DIR_GUARD)
	@$(CC) -o $@ $(SERVER_OBJ) $(LIB_DIR) $(LIB_NAME) 

# .o 文件转换成.cpp文件
$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	@$(DIR_GUARD)
	@$(CC) $(CPPFLAGS) $@ $<

test : $(OBJ_DIR)/$(TEST_DIR)/test
$(OBJ_DIR)/$(TEST_DIR)/test: $(TEST_OBJ)
	@$(DIR_GUARD)
	@$(CC) -o $@ $(TEST_OBJ) $(LIB_DIR) $(LIB_NAME) 


.PHONY:clean

clean:
	@rm -rf $(OBJ_DIR)