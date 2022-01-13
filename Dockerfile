FROM centos:8 AS build

# Install depends tools.
# https://segmentfault.com/a/1190000041012397
# fix centos:7 https://stackoverflow.com/questions/44205687/glibcxx-3-4-21-not-found-on-centos-7
# fix centos:7 https://segmentfault.com/a/1190000041012397
# or just use centos:8 directly
#RUN yum install -y gcc cmake make gcc-c++ automake libtool
RUN yum install -y gcc cmake make gcc-c++
        #git clone https://github.com/libuv/libuv.git /usr/local/libuv
COPY . /chat-room


# Build and install chat-room.
WORKDIR /chat-room

# avoid compiled local
RUN make clean && make

############################################################
# dist
############################################################
FROM centos:8 AS dist

# Expose port for media connection
EXPOSE 10010

# dependency: libuv
COPY --from=build /chat-room/build /usr/local/chat-room/build

# Default workdir and command.
WORKDIR /usr/local/chat-room

# add dynamic depot directroy
RUN echo $(pwd)/build/lib >> /etc/ld.so.conf && ldconfig