### 聊天室
这是一个基于libuv和uv-cpp实现的网络聊天室。使用uv-cpp作为传输的socket的封装。

#### 编译步骤
##### 编译libuv
1. 在`3rdparty/libuv`目录下，创建build文件夹，进入build文件夹
说明：这一步是为了创建libuv编译后的文件夹
2. 执行`cmake ..`，然后执行`make`。
说明：这一步是为了生成Makefile文件，并执行，从而生成uv-cpp所需要的库文件
3. 将生成的`libuv.so`文件放入`3rdparty/lib`目录下
说明：本项目默认第三方的库文件会放在`3rdparty/lib`目录下

##### 编译uv-cpp
1. 在`3rdparty/uv-cpp`目录下，创建build文件夹，进入build文件夹
2. 执行`cmake ..`，然后执行`make`
3. 将生成的`libuv.so`文件放入`3rdparty/lib`目录下

##### 编译本项目
项目根目录执行`make`
项目的`Makefile`文件会自动创建build文件夹，里面存放着所有的`.cpp`文件生成`.o`文件，同时会在build文件夹中创建client和server对应的文件，最终的可执行文件可以在client和server的目录中找到