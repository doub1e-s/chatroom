#include "Server.hpp"

using namespace damon;
using namespace std;

ChatroomServer::ChatroomServer()
    : m_init(false), m_loop(0), m_server(0)
{
    cout << "create server \n";
}

ChatroomServer::~ChatroomServer()
{
    cout << "destruct chatroom server\n";
    terminate();
}

// use configure file to decide which port to listening
uint32_t ChatroomServer::init()
{
    cout << "init chatroom server \n";
    if (m_init == false)
    {
        m_loop = EventLoop::DefaultLoop();
        SocketAddr addr("0.0.0.0", 10010, SocketAddr::Ipv4);
        // set each callback

        m_server = new uv::TcpServer(m_loop);
        // setup callback functions
        cout << "bind callback function \n";
        using namespace std::placeholders;
        m_server->setNewConnectCallback(std::bind(&ChatroomServer::onNewConnectCallback, this, _1));
        m_server->setConnectCloseCallback(std::bind(&ChatroomServer::onConnectCloseCallback, this, _1));
        m_server->setMessageCallback(std::bind(&ChatroomServer::onMessageCallback, this, _1, _2, _3));

        m_server->bindAndListen(addr);
        std::cout << "bind success, the port is addr" << addr.toStr() << endl;
        start();
    }

    return 0;
}

void ChatroomServer::start()
{
    cout << "start \n";
    m_thread = std::thread(&ChatroomServer::run, this);
}

void ChatroomServer::run()
{
    cout << "do run \n";
    m_loop->run();

    std::cout << "loop end \n";

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
}

void ChatroomServer::onNewConnectCallback(weak_ptr<TcpConnection> tcpConn)
{
    cout << " new connect coming \n";
    auto sharedTcpConnPtr = tcpConn.lock();
    shared_ptr<User> pUser = make_shared<User>(sharedTcpConnPtr);
    string addName = sharedTcpConnPtr->Name();
    m_userPtrMap[addName] = pUser;

    // set the color for the client
    // pick up a color when the client comes firstly, currently exist six kind of color
    srand(time(NULL));
    uint32_t color = (rand() % 5);
    pUser->m_color = color;
}

void ChatroomServer::onConnectCloseCallback(weak_ptr<TcpConnection> tcpConn)
{
    cout << "connection close \n";
    auto sharedTcpConnPtr = tcpConn.lock();
    string addName = sharedTcpConnPtr->Name();
    m_userPtrMap.erase(addName);
}

void ChatroomServer::onMessageCallback(uv::TcpConnectionPtr ptr, const char *data, ssize_t size)
{
    std::cout << "show message " << data << endl;

    // get client
    string addName = ptr->Name();
    std::cout << "show ptr name is " << ptr->Name() << endl;

    // register first
    if (m_userPtrMap.find(addName) != m_userPtrMap.end())
    {
        std::cout << "find name " << endl;
        shared_ptr<User> pUser = m_userPtrMap.at(addName);
        // set name, callback to client
        string strData(data);
        if (strData.rfind("#name:", 0) == 0)
        {
            std::cout << "set a name " << endl;
            string username = strData.substr(6, size);
            pUser->setName(username);
            string answer2client = "join success, you name is " + username;
            int dataLength = answer2client.length() + 1;
            // convert the message to char*, and then send it to client
            char *char_arr = &answer2client[0];
            ptr->write(char_arr, dataLength, nullptr);
        }
        else
        {
            // broadcast the message
            for (auto it = m_userPtrMap.begin(); it != m_userPtrMap.end(); it++)
            {
                // write message when the ptr isn't the sender
                if (it->first != addName)
                {
                    shared_ptr<User> pUser = m_userPtrMap.at(addName);
                    string senderName = pUser->getName();
                    uint32_t senderColor = pUser->m_color;
                    string sendMessage = std::to_string(senderColor) + "#" + senderName + " : " + strData;
                    int dataLength = sendMessage.length() + 1;
                    char *char_arr = &sendMessage[0];
                    it->second->write(char_arr, dataLength, nullptr);
                }
            }
        }
    }
}
