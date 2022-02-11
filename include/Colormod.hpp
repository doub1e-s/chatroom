#include <ostream>
namespace Color
{
    const int COLOR_TYPES = 6;

    enum Code
    {
        RED = 31,
        GREEN = 32,
        YELLO = 33,
        BLUE = 34,
        MAGENTA = 35,
        CYAN = 36,
        DEFAULT = 39,
    };

    using namespace std;
    class Modifier
    {
    public:
        Code code;

    public:
        Modifier()
        {
            this->code = Code::DEFAULT;
        }
        Modifier(Code pCode) : code(pCode) {}

        string toString()
        {
            return "\033[" + to_string(code)+"m";
        }

        friend std::ostream &
        operator<<(std::ostream &os, const Modifier &mod)
        {
            return os << "\033[" << mod.code << "m";
        }
    };
}