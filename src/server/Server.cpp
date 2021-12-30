#include "Server.hpp"

using namespace damon;

int main(int argc, char** args)
{
    using namespace std;
    // create a set to store all TcpConnectionPtr
    map<string, shared_ptr<User>> userPtrMap;
    
    EventLoop* loop = EventLoop::DefaultLoop();

    SocketAddr addr("0.0.0.0", 10010, SocketAddr::Ipv4);

    std::atomic<uint64_t> dataSize(0);
    uv::TcpServer server(loop);

    // create client when connection created
    server.setNewConnectCallback([&userPtrMap](weak_ptr<TcpConnection> tcpConn) {
        auto sharedTcpConnPtr = tcpConn.lock();
        shared_ptr<User> pUser = make_shared<User>(sharedTcpConnPtr);
        string addName = sharedTcpConnPtr->Name();
        userPtrMap[addName] = pUser;

        // set the color for the client
        // get a random num between 240 ~ 254
        srand(time(NULL));
        uint32_t color = (rand() % (254-240) + 240);
        pUser->m_color = color;
    });

    // erase client when connection closed
    server.setConnectCloseCallback([&userPtrMap](weak_ptr<TcpConnection> tcpConn) {
        auto sharedTcpConnPtr = tcpConn.lock();
        string addName = sharedTcpConnPtr->Name();
        userPtrMap.erase(addName);
    });

    // set callback when message come
    server.setMessageCallback([&dataSize, &userPtrMap](uv::TcpConnectionPtr ptr,const char* data, ssize_t size)
    {
        // sum datasize
        // dataSize += size;
        // ptr->write(data, size, nullptr);
        cout << "show message " << data << endl;

        // get client
        string addName = ptr->Name();
        cout << "show ptr name is " << ptr->Name() << endl;

        // register first
        if (userPtrMap.find(addName) != userPtrMap.end()) {
            cout << "find name " << endl;
            shared_ptr<User> pUser = userPtrMap.at(addName);
            // set name, callback to client
            string strData(data);
            if (strData.rfind("#name:", 0) == 0) {
                cout << "set a name " << endl;
                string username = strData.substr(6, size);
                pUser->setName(username);
                string answer2client = "join success, you name is " + username;
                int dataLength = answer2client.length()+1;
                // convert the message to char*, and then send it to client
                char *char_arr = &answer2client[0];
                ptr->write(char_arr, dataLength, nullptr);
            } else {
                // broadcast the message
                for (auto it=userPtrMap.begin(); it!=userPtrMap.end(); it++) {
                    // write message when the ptr isn't the sender
                    if (it->first != addName) {
                        shared_ptr<User> pUser = userPtrMap.at(addName);
                        string senderName = pUser->getName();
                        uint32_t senderColor = pUser->m_color;
                        string sendMessage = std::to_string(senderColor) + "#" + senderName + " : " + strData;
                        int dataLength = sendMessage.length()+1;
                        char *char_arr = &sendMessage[0];
                        it->second->write(char_arr, dataLength, nullptr);
                    }
                }
            }
        }
    });

    //心跳超时
    server.bindAndListen(addr);
    cout << "bind success, the port is addr"<< addr.toStr() << endl;

    uv::Timer timer(loop, 10000, 10000, [&dataSize](uv::Timer* ptr)
    {
        // std::cout << "send data:" << (dataSize >> 10) << " kbyte/s" << std::endl;
        dataSize = 0;
    });
    timer.start();
    loop->run();
}