#include <gsl/gsl>
#include <iostream>
#include <cassert>

#define Likely(cond) __builtin_expect(!!(cond), 1)
#define Contract_check(type, cond)       \      
        (Likely(cond) ? static_cast<void>(0) : std::terminate())
#define MyExpects(cond) Contract_check("precondition", cond)

template <typename T>
class not_null {
public:
    static_assert(1 > 0);
};

int main(void)
{
    int i = 10;

    if (__builtin_expect(i > 0, 1))
    {
        std::cout << "in expected branch...\n";
    }
    else
    {
        std::cout << "...\n";
    }

    MyExpects(i > 0);


    return 0;
}
