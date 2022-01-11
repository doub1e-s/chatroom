FROM centos:7 AS build

# Install depends tools.
RUN yum install -y gcc make gcc-c++ automake libtool git && \
        #RPC failed; result=35, HTTP code = 0
        git config --global http.postBuffer 50M && git clone https://github.com/libuv/libuv.git /usr/local/libuv

# Build and install chat-room.
WORKDIR /usr/local/libuv
RUN sh autogen.sh && ./configure && make && make install

############################################################
# dist
############################################################
# FROM centos:7 AS dist

# # Expose port for media connection
# EXPOSE 10010

# # chat-room binary, and any it's dependency.
# COPY --from=build /chat-room /usr/local/chat-room

# # Default workdir and command.
# WORKDIR /usr/local/chat-room