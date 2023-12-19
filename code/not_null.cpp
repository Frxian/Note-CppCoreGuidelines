#include <gsl/gsl>

void foo(gsl::not_null<int*> pointer)
{

}

int main()
{
    int* p = nullptr;
    // int* p = new int(2);
    foo(p);

    return 0;
}
