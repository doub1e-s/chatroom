#include "ChatroomServer.hpp"

using namespace Damon;
using namespace std;

ChatroomServer::ChatroomServer()
    : m_init(false), m_loop(0), m_server(0)
{
    cout << "create server \n";
}

ChatroomServer::~ChatroomServer()
{
    cout << "destruct chatroom server\n";
    stop();
}

// use configure file to decide which port to listening
uint32_t ChatroomServer::init()
{
    cout << "init chatroom server \n";
    if (m_init == false)
    {
        // init color with six basic color to identity diffrent users for client
        using namespace Color;
        this->m_colors = new Modifier[COLOR_TYPES];
        for (int i = 0, j = RED; i < COLOR_TYPES; i++, j++) {
            this->m_colors[i] = Modifier(static_cast<Code>(j));
        }

        m_loop = EventLoop::DefaultLoop();
        SocketAddr addr("0.0.0.0", 10010, SocketAddr::Ipv4);
        // set each callback

        m_server = new uv::TcpServer(m_loop);
        // setup callback functions
        cout << "bind callback function \n";
        using namespace std::placeholders;
        m_server->setNewConnectCallback(bind(&ChatroomServer::onNewConnectCallback, this, _1));
        m_server->setConnectCloseCallback(bind(&ChatroomServer::onConnectCloseCallback, this, _1));
        m_server->setMessageCallback(bind(&ChatroomServer::onMessageCallback, this, _1, _2, _3));

        m_server->bindAndListen(addr);
        cout << "bind success, the port is addr" << addr.toStr() << endl;
        start();
    }

    return 0;
}

void ChatroomServer::start()
{
    cout << "start \n";
    m_thread = thread(&ChatroomServer::run, this);
}

void ChatroomServer::run()
{
    cout << "do run \n";
    m_loop->run();

    cout << "loop end \n";

    delete m_loop;
}

void ChatroomServer::stop()
{
    m_loop->stop();
    if (m_init)
    {
        m_init = false;
        cout << "stop loop \n";
        EventLoop *loop = m_loop;
        m_loop->runInThisLoop([loop]()
                              { loop->stop(); });

        cout << "stop thread \n";
        if (m_thread.joinable())
        {
            m_thread.join();
        }
    }
    delete m_loop;
}

void ChatroomServer::onNewConnectCallback(weak_ptr<TcpConnection> tcpConn)
{
    cout << " new connect coming \n";
    auto sharedTcpConnPtr = tcpConn.lock();
    shared_ptr<User> pUser = make_shared<User>(sharedTcpConnPtr);
    string userName = sharedTcpConnPtr->Name();
    m_address2User[userName] = pUser;

    // set the color for the client
    // pick up a color when the client comes firstly, currently exist six kind of color
    srand(time(NULL));
    uint32_t colorIndex = (rand() % 6);
    pUser->m_colorIndex = colorIndex;
}

void ChatroomServer::onConnectCloseCallback(weak_ptr<TcpConnection> tcpConn)
{
    cout << "connection close \n";

    uv::TcpConnectionPtr tcpPtr = tcpConn.lock();
    string addressName = tcpPtr->Name();
    shared_ptr<User> pUser = m_address2User.at(addressName);
    string username = pUser->getName();
    string color = m_colors[pUser->m_colorIndex].toString();
    string helloMessage = " has left the chatroom";
    string message = color + username + helloMessage;

    // broadcast the message
    broadcastMessage(addressName, message);
    auto sharedTcpConnPtr = tcpConn.lock();
    m_address2User.erase(sharedTcpConnPtr->Name());

}

void ChatroomServer::onMessageCallback(uv::TcpConnectionPtr ptr, const char *data, ssize_t size)
{
    // get client， addressName will like 127.0.0.1:46840
    string addressName = ptr->Name();

    // make sure the message isn't empty
    if (size != 0 && data[0] != 0 && m_address2User.find(addressName) != m_address2User.end())
    {
        // cout << "find name " << endl;
        shared_ptr<User> pUser = m_address2User.at(addressName);
        // set name, callback to client
        string strData(data);
        if (strData.rfind("#name:", 0) == 0)
        {
            string username = strData.substr(6, size);
            pUser->setName(username);
            // broadcast that new chatter has coming
            string color = m_colors[pUser->m_colorIndex].toString();
            string helloMessage = " has join in the chatroom";
            string message = color + username + helloMessage;

            // broadcast the message
            broadcastMessage(addressName, message);
        }
        else
        {
            shared_ptr<User> pUser = m_address2User.at(addressName);
            string senderName = pUser->getName();
            string color = m_colors[pUser->m_colorIndex].toString();
            string message = color + senderName + " : " + strData;
            cout << "show message" << message << endl;

            // broadcast the message
            broadcastMessage(addressName, message);
        }
    }
}

int ChatroomServer::broadcastMessage(const string& sender, const string& message)
{
    for (auto it = m_address2User.begin(); it != m_address2User.end(); it++)
    {
        // write message when the ptr isn't the sender
        if (it->first != sender)
        {
            int dataLength = message.length() + 1;
            const char *char_arr = &message[0];
            it->second->write(char_arr, dataLength, nullptr);
        }
    }

    return 0;
}


std::thread& ChatroomServer::getThread()
{
    return m_thread;
}