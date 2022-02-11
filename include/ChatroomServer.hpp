#pragma once
#include <iostream>
#include <atomic>
#include <uv11.hpp>
#include <map>
#include <memory>
#include <time.h>
#include <unistd.h>
#include "User.hpp"
#include "Colormod.hpp"
#include "easylogging++.h"

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
    EventLoop* m_loop;
    TcpServer* m_server;
    map<string, shared_ptr<User>> m_address2User;
    Color::Modifier *m_colors;
    std::thread m_thread;

    void onNewConnectCallback(weak_ptr<TcpConnection> tcpConn);
    void onConnectCloseCallback(weak_ptr<TcpConnection> tcpConn);
    void onMessageCallback(uv::TcpConnectionPtr ptr, const char *data, ssize_t size);
};

}

