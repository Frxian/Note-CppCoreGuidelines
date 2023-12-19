#include <cstddef> 
#include <iostream>
#include <string_view>

#include <span>
#define OUT(x) std::cout << x << " ";

int main(void)
{
    std::cout << sizeof(std::ptrdiff_t);

    int a[] = {1, 2, 3};
    std::span<int> span{a};

    for(auto const& i : span)
    {
        OUT(i);        
    }

    std::string_view sv{"frxian hello"};

    char* s = "matthew hello";
    sv = s;

    for(auto s : sv)
    {
        OUT(s);
    }
    return 0;
}
