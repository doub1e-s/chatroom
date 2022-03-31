#include <iostream>
#include <cstring>
#include <thread>
#include <chrono>
#include <uv11.hpp>
#include <string>
#include <functional>
#include "Colormod.hpp"

namespace Damon{


class ChatroomClient {
public:
    ChatroomClient();
    virtual ~ChatroomClient();

public:
    // 初始化的时候需要设置用户的用户名
    uint32_t init();
    void start();
    void stop();
    std::thread& getSendThread();

private:
    void onConnectStatusCallback(uv::TcpClient::ConnectStatus status);
    void onMessageCallback(const char* data, ssize_t size);
    void getInput(std::string& input);
    inline void writeMessage(std::string& message);
    void run();
    void sendLoop();

private:
    std::atomic<bool> m_init;
    std::atomic<bool> m_loggedIn;
    uv::EventLoop *m_loop;
    uv::TcpClientPtr m_tcpClient;
    std::string m_username;
    std::string m_senderName;
    std::thread m_reciveThread;
    std::thread m_sendThread;
    Color::Modifier *m_defModifier;
};

}
