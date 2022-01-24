#include "User.hpp"
using namespace std;
using namespace uv;
namespace Damon {

User::User(TcpConnectionPtr tcpConnPtr)
{
    m_tcpConnPtr = tcpConnPtr;
}

User::~User()
{
}

void User::setName(string name)
{
    m_name = name;
}

string User::getName()
{
    return m_name;
}

int User::write(const char* buf, ssize_t size, uv::AfterWriteCallback callback)
{
    return m_tcpConnPtr->write(buf, size, callback);
}


}