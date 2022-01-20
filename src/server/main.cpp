#include <iostream>
#include "Server.hpp"
#include <chrono>
#include <thread>

using namespace uv;
using namespace std;
using namespace damon;

int main() {
    ChatroomServer server;
    server.init();
    while (1)
    {
        this_thread::sleep_for(chrono::milliseconds(1000));
    }
}