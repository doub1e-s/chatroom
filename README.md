### 聊天室
这是一个基于libuv和uv-cpp实现的网络聊天室。使用uv-cpp作为传输的socket的封装。

#### 编译步骤
##### 项目根目录直接执行
项目根目录执行`make`
项目的`Makefile`文件会自动创建build文件夹，里面存放着所有的`.cpp`文件生成`.o`文件，同时会在build文件夹中创建client和server对应的文件，最终的可执行文件可以在client和server的目录中找到