#pragma once
#include <iostream>
#include "Writer.hpp"

class Book {

public:
    Book();
    void setName(string name);
    string getName();
    void setPrice(double price);
    double getPrice();
    void setWriter(Writer *pWriter);
    Writer* getWriter();

private:
    string m_name;
    Writer* m_pWriter;
    double m_price;
};