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
    void setAge(int age);
    void setFavor(string favor);
    string getName();
    int getAge();
    string getFavor();
};