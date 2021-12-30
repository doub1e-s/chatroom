#include <iostream>

using namespace std;

class Test {
public:
    Test()
    :m_age(10)
    {
    }
    void printA()
    {
        cout << "print A" << endl;
    }

    virtual void printB()
    {
        cout << "print B " <<endl;
    }

    virtual ~Test(){}

private:
    int m_age;

};
