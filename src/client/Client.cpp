#include <iostream>
#include <thread>
#include <chrono>
#include <uv11.hpp>

using namespace std;

namespace damon {


using namespace std;

void startClients(uv::EventLoop* loop,uv::SocketAddr& addr, uv::TcpClientPtr &client, string &name)
{
    client->setConnectStatusCallback( [&client, &name](uv::TcpClient::ConnectStatus status)
    {
        if (status == uv::TcpClient::ConnectStatus::OnConnectSuccess)
        {
            int dataLength = name.length()+1;
            char *data = &name[0];
            client->write(data, dataLength, nullptr);
            cout << "create client successs" << endl;
        }
        else
        {
            std::cout << "Error : connect to server fail" << std::endl;
        }
    });

    client->setMessageCallback( [&client](const char* data,ssize_t size)
    {
        string strData(data);
        if (strData.find("#") == string::npos) {
            if (strData.rfind("join", 0) == 0) {
                string hello = "hello, nice to meet you ";
                int dataLength = hello.length() + 1;
                char *char_arr = &hello[0];
                client->write(char_arr, dataLength, nullptr);
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

        cout << message << endl;
    });

    client->connect(addr);
}

void writeMessage(uv::TcpClientPtr &ptr, string message)
{
    int dataLength = message.length() + 1;
    char *char_arr = &message[0];
    ptr->write(char_arr, dataLength, nullptr);
}

void sendLoop(uv::TcpClientPtr &ptr)
{
    while(1) {
        string message;
        getline(cin, message);
        fflush(stdin);
        writeMessage(ptr, message);
    }
}

}

using namespace damon;
int main(int argc, char** args)
{
    // 需要在run之前，先多启动一个线程，然后让那个线程拿到 clients里面的连接，用连接进行发送数据
    uv::EventLoop* loop = uv::EventLoop::DefaultLoop();

    uv::SocketAddr serverAddr("127.0.0.1", 10010, uv::SocketAddr::Ipv4);
    // could create many client if necessary
    // std::vector<uv::TcpClientPtr> clients;

    // set username in the beginning
    uv::TcpClientPtr client = std::make_shared<uv::TcpClient>(loop);
    string username;
    cout << "welcome to chatroom, please insert your name " << endl;
    getline(cin, username);
    username=username.insert(0, "#name:");
    startClients(loop, serverAddr, client, username);
    cout << "start client" << endl;
    // create a new thread to read client message
    thread readMessageT(std::bind(sendLoop,client));
    readMessageT.detach();
    loop->run();
    return 0;
}




/* 
void delay()
{
    this_thread::sleep_for(chrono::nanoseconds(10));
    cout << "delay is finished" << endl;
}

int main() {
    cout << "start client" << endl;
    thread first(delay);
    this_thread::sleep_for(chrono::nanoseconds(20));
    cout << "main is finished " << endl;
} 
*/