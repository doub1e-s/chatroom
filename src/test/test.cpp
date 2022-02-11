#include "Colormod.hpp" // namespace Color
#include <iostream>
#include <uv11.hpp>

char message[1024];

void func(uv::EventLoop* loop)
{
    std::cout << "in func thread id:" << std::this_thread::get_id() << std::endl;
    loop->runInThisLoop([loop]() 
    {
        std::cout << "stop in loop thread id:" << std::this_thread::get_id() << std::endl;
        loop->stop();
    });
}

void loop_exit()
{
    uv::EventLoop* loop = new uv::EventLoop();
    std::thread t1(std::bind(&func, loop));
    loop->run();
    loop->runInThisLoop([]() 
    {
        std::cout << "loop is stop" << std::endl;
    });
    delete loop;
    t1.join();
    std::cout << "delete loop" << loop << std::endl << std::endl;;
}

using namespace std;
void echo()
{
    int i = 0;
    while (i != 5)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        cout << "\rafter seconds";
        cout.flush();
        cout << endl;
        cout << "message" << message << endl;
        i++;
    }
}

using namespace std;
int main() {
    Color::Modifier red(Color::RED);
    Color::Modifier def(Color::DEFAULT);
    // cout << "This ->" << red << "word" << def << "<- is red." << endl;
    // Color::Modifier green(Color::GREEN);
    // cout << "This ->" << green << "word" << def << "<- is green." << endl;
    // Color::Modifier yello(Color::YELLO);
    // cout << "This ->" << yello<< "word" << def << "<- is yello." << endl;
    // Color::Modifier blue(Color::BLUE);
    // cout << "This ->" << blue << "word" << def << "<- is blue." << endl;
    // Color::Modifier megenta(Color::MAGENTA);
    // cout << "This ->" << megenta << "word" << def << "<- is megenta." << endl;
    // Color::Modifier cyan(Color::CYAN);
    // cout << "This ->" << cyan << "word" << def << "<- is cyan." << endl;

    string ss = def.toStr() + "hello";
    cout << ss << endl;

    // cout << "This is \r" << "a line" << endl;

    // test join
    // while (true)
    // {
    //     std::thread t1(std::bind(&loop_exit));
    //     t1.join();
    //     std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    // }

    // test cout while getline();

    // getline(cin, message1);


    // thread t2(echo);
    // char ch;
    // int pos = 0;
    // while (ch != '\n') {
    //     ch = cin.get();
    //     message[pos] = ch;
    //     pos++;
    // }
    // cout << "finish" << endl;
    // cout << message << endl;
    // t2.join();

    return 0;

}

/*

// use easylogging as log lib if it's nessesary
// build easylogging as static library firstly
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

// install the lib and add it when compiling
int main(int argc, char* argv[]) {
   LOG(INFO) << "My first info log using default logger";
   return 0;
}

*/