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
    m_defModifier = new Color::Modifier();
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
        string message = "#name:" + m_username;
        int dataLength = message.length() + 1;
        char *data = &message[0];
        m_tcpClient->write(data, dataLength, nullptr);
    } else {
        std::cout << "Error : connect to server fail, please check the netstatus and the server statement" << std::endl;
    }
}

// client need to do nothing and all settle down by the server side
void ChatroomClient::onMessageCallback(const char* data, ssize_t size)
{
    string strData(data);
    cout << "\r" <<  strData << "\n" << m_defModifier->toString() << "YOU: ";
    cout.flush();
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

void ChatroomClient::writeMessage(string& message)
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

    string message;
    while(1) {
        cout << m_defModifier->toString() << "YOU: ";
        cout.flush();
        getInput(message);
        writeMessage(message);
    }
}

}
