#include <iostream>
#include "Book.hpp"
#include "uv11.hpp"
#include <iostream>
#include <atomic>

using namespace uv;
using namespace std;

int main() {
    Book diary = Book();
    diary.setName("diary");
    diary.setPrice(20.0);

    Writer damon = Writer();
    damon.setName("damon");
    damon.setAge(24);
    damon.setFavor("gaming and swiming");

    diary.setWriter(&damon);
    cout << "show diary name:" << diary.getName() << " price :" << diary.getPrice() << endl;
    cout << "add some test" << endl;

    cout << "---------------------------------" << endl;
    EventLoop* loop = EventLoop::DefaultLoop();

    SocketAddr addr("0.0.0.0", 10005, SocketAddr::Ipv4);

    std::atomic<uint64_t> dataSize(0);
    uv::TcpServer server(loop);
    server.setMessageCallback([&dataSize](uv::TcpConnectionPtr ptr,const char* data, ssize_t size)
    {
        dataSize += size;
        ptr->write(data, size, nullptr);
    });
    //心跳超时
    //server.setTimeout(15);
    server.bindAndListen(addr);

    uv::Timer timer(loop, 1000, 1000, [&dataSize](uv::Timer* ptr)
    {
        std::cout << "send data:" << (dataSize >> 10) << " kbyte/s" << std::endl;
        dataSize = 0;
    });
    timer.start();
    loop->run();

}