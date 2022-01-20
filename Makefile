# 确保生成obj文件的时候指定目录是存在的
DIR_GUARD=mkdir -p $(@D)
# 构建时需要用到的路径
SRC_DIR = src
SERVER_DIR = server
CLIENT_DIR = client
TEST_DIR = test
OBJ_DIR = build
# 获取当前正在执行的makefile的路径
CUR_PATH:=$(shell pwd)
DEP_OBJ_DIR = $(CUR_PATH)/3rdparty/libuv/build $(CUR_PATH)/3rdparty/uv-cpp/build

# g++的对应参数
INC_DIR	= -Iinclude -I3rdparty/uv-cpp/uv/include -I3rdparty/uv-cpp/libuv1.30.0/include
LIB_DIR = -L$(OBJ_DIR)/lib
LIB_NAME= -luv_cpp -luv -pthread


# SOURCES := $(wildcard $(SRC_DIR)/$(SERVER_DIR)/*.cpp $(SRC_DIR)/$(CLIENT_DIR)/.*cpp $(SRC_DIR))
# SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
#https://stackoverflow.com/questions/4036191/sources-from-subdirectories-in-makefile
# 将src各自文件夹内的所有.cpp文件，转换成对应的.o文件，并加上前缀，从而将obj文件输入至指定的目录(这里为build+对应的目录)
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
all : dep client server

# WARNING: 运行失败记得看下动态库的位置是否包含了
# 构建所需依赖，将所有的依赖都放入build文件夹中，使build文件夹就包含所有运行所需文件
dep :
	@mkdir -p 3rdparty/libuv/build && cd 3rdparty/libuv/build && cmake .. && make
	@mkdir -p $(CUR_PATH)/3rdparty/uv-cpp/build && cd $(CUR_PATH)/3rdparty/uv-cpp/build && cmake .. && make
	@cd $(CUR_PATH) && mkdir -p build/lib && \
		cp -r 3rdparty/uv-cpp/build/libuv_cpp* 3rdparty/libuv/build/libuv* build/lib

# cp -r $(mkfile_path)3rdparty/uv-cpp/build/uv
# 执行'make client' 会生成后面的'client','client'依赖于$(client_obj)
# $(client_obj)变量的定义中会把所有的.cpp文件都替换为.o文件，注意这里只是变量而已，并没有真正执行实际命令
# 由于目标文件依赖于$(client_obj)里面的指定.o文件，触发.o文件的依赖，促使对所依赖的.o文件的生成
# 最后使用下面的命令输出client目标文件
client : $(OBJ_DIR)/$(CLIENT_DIR)/client
$(OBJ_DIR)/$(CLIENT_DIR)/client: $(CLIENT_OBJ)
	@$(DIR_GUARD)
	@$(CC) -o $@ $(CLIENT_OBJ) $(LIB_DIR) $(LIB_NAME) 

server : $(OBJ_DIR)/$(SERVER_DIR)/server
$(OBJ_DIR)/$(SERVER_DIR)/server: $(SERVER_OBJ)
	@$(DIR_GUARD)
	@$(CC) -o $@ $(SERVER_OBJ) $(LIB_DIR) $(LIB_NAME) 

# 将被依赖的.o文件通过.cpp进行编译，百分号将保留文件的路径
$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	@$(DIR_GUARD)
	@$(CC) $(CPPFLAGS) $@ $<

test : $(OBJ_DIR)/$(TEST_DIR)/test
$(OBJ_DIR)/$(TEST_DIR)/test: $(TEST_OBJ)
	@$(DIR_GUARD)
	@$(CC) -o $@ $(TEST_OBJ) $(LIB_DIR) $(LIB_NAME) 


.PHONY:clean

clean:
	@rm -rf $(OBJ_DIR) $(DEP_OBJ_DIR)