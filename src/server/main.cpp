#include "ChatroomServer.hpp"
#include <chrono>
#include <thread>

INITIALIZE_EASYLOGGINGPP

using namespace std;

int main() {
    Damon::ChatroomServer server;
    server.init();
    server.getThread().join();
}