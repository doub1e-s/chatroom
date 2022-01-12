FROM centos:7 AS build

# Install depends tools.
# https://segmentfault.com/a/1190000041012397
# fix https://stackoverflow.com/questions/44205687/glibcxx-3-4-21-not-found-on-centos-7
# fix https://segmentfault.com/a/1190000041012397
RUN yum install -y gcc cmake make gcc-c++ automake libtool
        #git clone https://github.com/libuv/libuv.git /usr/local/libuv
COPY . /chat-room


# Build and install chat-room.
WORKDIR /chat-room
RUN cd 3rdparty/libuv && sh autogen.sh && ./configure && make && make install \
       && cd ../uv-cpp && mkdir build && cd build && cmake .. && make \
       && cp libuv_cpp.a ../../lib \
       && cd /chat-room && make
#
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