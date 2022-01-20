#include "colormod.hpp" // namespace Color
#include <iostream>
#include <uv11.hpp>
using namespace std;

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

int main() {
    Color::Modifier red(Color::FG_RED);
    Color::Modifier def(Color::FG_DEFAULT);
    cout << "This ->" << red << "word" << def << "<- is red." << endl;
    Color::Modifier green(Color::FG_GREEN);
    cout << "This ->" << green << "word" << def << "<- is green." << endl;
    Color::Modifier yello(Color::FG_YELLO);
    cout << "This ->" << yello<< "word" << def << "<- is yello." << endl;
    Color::Modifier blue(Color::FG_BLUE);
    cout << "This ->" << blue << "word" << def << "<- is blue." << endl;
    Color::Modifier megenta(Color::FG_MAGENTA);
    cout << "This ->" << megenta << "word" << def << "<- is megenta." << endl;
    Color::Modifier cyan(Color::FG_CYAN);
    cout << "This ->" << cyan << "word" << def << "<- is cyan." << endl;

    // while (true)
    // {
    //     std::thread t1(std::bind(&loop_exit));
    //     t1.join();
    //     std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    // }

}