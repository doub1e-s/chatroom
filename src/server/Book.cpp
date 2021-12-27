#include "Book.hpp"

using namespace std;

Book::Book()
{
}

void Book::setName(string name)
{
    this->m_name = name;
}

string Book::getName()
{
    return m_name;
}

void Book::setPrice(double price)
{
    this->m_price= price;
}

double Book::getPrice()
{
    return this->m_price;
}

void Book::setWriter(Writer *pWriter)
{
    this->m_pWriter = pWriter;
}

Writer* Book::getWriter()
{
    return this->m_pWriter;
}