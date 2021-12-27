#include "Writer.hpp"

Writer::Writer(/* args */)
{
    m_name = "writer";
    m_age = 22;
    m_favor = "swimming";
}

Writer::~Writer()
{
}

void Writer::setName(string name)
{
    m_name = name;
}

void Writer::setAge(int age)
{
    m_age = age;
}

void Writer::setFavor(string favor)
{
    m_favor = favor;
}

string Writer::getName()
{
    return m_name;
}

int Writer::getAge()
{
    return m_age;
}

string Writer::getFavor()
{
    return m_favor;
}