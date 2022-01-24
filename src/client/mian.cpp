#include "ChatroomClient.hpp"

using namespace std;

int main() {
    Damon::ChatroomClient client;
    client.init();
    client.getSendThread().join();
}

