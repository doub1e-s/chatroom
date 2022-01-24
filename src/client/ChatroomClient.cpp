#include "ChatroomClient.hpp"
using namespace std;

namespace Damon {

ChatroomClient::ChatroomClient()
    : m_init(false), m_loop(0), m_tcpClient(0)
{
}

ChatroomClient::~ChatroomClient()
{
    cout << "client terminate \n";
    stop();
}

uint32_t ChatroomClient::init()
{
    m_loop = uv::EventLoop::DefaultLoop();
    m_tcpClient = std::make_shared<uv::TcpClient>(m_loop);
    using namespace std::placeholders;
    m_tcpClient->setConnectStatusCallback(std::bind(&ChatroomClient::onConnectStatusCallback, this, _1));
    m_tcpClient->setMessageCallback(std::bind(&ChatroomClient::onMessageCallback, this, _1, _2));
    uv::SocketAddr serverAddr("127.0.0.1", 10010, uv::SocketAddr::Ipv4);
    m_tcpClient->connect(serverAddr);
    start();
    return 0;
}

void ChatroomClient::start()
{
    cout << "start \n";
    m_sendThread = thread(&ChatroomClient::sendLoop, this);
    m_reciveThread = thread(&ChatroomClient::run, this);
}

void ChatroomClient::stop()
{
    m_loop->stop();
    if (m_init)
    {
        m_init = false;
        cout << "stop loop \n";
        uv::EventLoop *loop = m_loop;
        m_loop->runInThisLoop([loop]()
                              { loop->stop(); });

        cout << "stop thread \n";
        if (m_reciveThread.joinable())
        {
            m_reciveThread.join();
        }
    }
    delete m_loop;
}

std::thread& ChatroomClient::getSendThread()
{
    return m_sendThread;
}

void ChatroomClient::onConnectStatusCallback(uv::TcpClient::ConnectStatus status)
{
    if (status == uv::TcpClient::ConnectStatus::OnConnectSuccess) {
        int dataLength = m_username.length() + 1;
        char *data = &m_username[0];
        m_tcpClient->write(data, dataLength, nullptr);
        cout << "create client successs" << endl;
    }
    else {
        std::cout << "Error : connect to server fail" << std::endl;
    }
}

void ChatroomClient::onMessageCallback(const char* data, ssize_t size)
{
    string strData(data);
    if (strData.find("#") == string::npos) {
        if (strData.rfind("join", 0) == 0) {
            string hello = "hello, nice to meet you ";
            int dataLength = hello.length() + 1;
            char *char_arr = &hello[0];
            m_tcpClient->write(char_arr, dataLength, nullptr);
        }
        return;
    }
    string message = strData.substr(strData.find("#") + 1);
    string color = strData.substr(0, strData.find("#"));
    int colorNum = stoi(color);
    // windows case, please remeber to include <Windows.h> header file
    // 使用colorNum 设置用户发送的消息的颜色
    // SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorNum);
    // std::cout << message << std::endl;
    // // 将颜色重置回默认的颜色
    // SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

    std::cout << message << std::endl;
}

void ChatroomClient::getInput(string& input)
{
    while (true)
    {
        getline(cin, input);
        if (input.length() == 0 || input == "") {
            cout << "Your input is illegal, please insert again \n"; 
            cout << input << endl;
        } else {
            break;
        }
    }
}

void ChatroomClient::writeMessage(string message)
{
    int dataLength = message.length() + 1;
    char *char_arr = &message[0];
    m_tcpClient->write(char_arr, dataLength, nullptr);
}

void ChatroomClient::run()
{
    while (!m_loggedIn) {
        std::this_thread::sleep_for(chrono::milliseconds(400));
    }
    cout << "do run \n";
    m_loop->run();

    cout << "loop end \n";
    delete m_loop;
}

void ChatroomClient::sendLoop()
{
    // TODO: check if the username has been used
    cout << "please insert your name \n";
    getInput(m_username);
    // for recive thread
    cout << "login success, your name is " << m_username << "\n";
    m_loggedIn = true;
    string registerMessage = "#name:" + m_username;
    writeMessage(registerMessage);

    string message;
    while(1) {
        getInput(message);
        writeMessage(message);
    }
}

}
