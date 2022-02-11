#pragma once

#include <iostream>
#include "TcpConnection.hpp"

using namespace std;

namespace Damon{


class User
{
public:
    User() = delete;
    explicit User(uv::TcpConnectionPtr tcpConnPtr);
    virtual ~User();

    void setName(string name);
    string getName();
    int write(const char* buf, ssize_t size, uv::AfterWriteCallback callback);

public:
    uint32_t m_colorIndex;

private:
    uv::TcpConnectionPtr m_tcpConnPtr;
    string m_name;
    int age;
};

}