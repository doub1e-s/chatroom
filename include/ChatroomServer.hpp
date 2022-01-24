#pragma once
#include <iostream>
#include <atomic>
#include <uv11.hpp>
#include <map>
#include <memory>
#include <time.h>
#include "User.hpp"

using namespace uv;

namespace Damon{


class ChatroomServer
{
public:
    ChatroomServer();
    virtual ~ChatroomServer();

public:
    uint32_t init();
    void start();
    void run();
    void stop();
    int broadcastMessage(const string& sender, const string& message);
    std::thread& getThread();

private:
    std::atomic<bool> m_init;
    TcpServer* m_server;
    EventLoop* m_loop;
    map<string, shared_ptr<User>> m_address2User;
    std::thread m_thread;

    void onNewConnectCallback(weak_ptr<TcpConnection> tcpConn);
    void onConnectCloseCallback(weak_ptr<TcpConnection> tcpConn);
    void onMessageCallback(uv::TcpConnectionPtr ptr, const char *data, ssize_t size);
};

}

