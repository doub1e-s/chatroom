#pragma once
#include <string>

using namespace std;
class Writer
{
private:
    string m_name;
    int m_age;
    string m_favor;
public:
    Writer(/* args */);
    virtual ~Writer();

    void setName(string name);
    void setAge(uint32_t age);
    void setFavor(string favor);
    string getName();
    uint32_t getAge();
    string getFavor();
};