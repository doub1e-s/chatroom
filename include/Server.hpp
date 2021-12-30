#pragma once
#include <iostream>
#include <atomic>
#include <uv11.hpp>
#include <map>
#include <memory>
#include <time.h>
#include "User.hpp"

using namespace uv;

namespace damon{


class ChatroomServer : public TcpServer
{
public:
    ChatroomServer(EventLoop* loop);
    virtual ~ChatroomServer();

};

}

