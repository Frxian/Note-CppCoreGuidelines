#include <optional>
#include <iostream>

std::optional<int> foo()
{
    return std::optional<int>{1};
}
auto foo1()
{
    return std::nullopt;
}
int main()
{
    std::cout << foo1().value();

    return 0;
}
