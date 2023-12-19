#include <gsl/gsl>

int main(void)
{

    int* p = new int(10);

    gsl::owner<int*> pOwner = p;

    delete pOwner;
    delete p;

    return 0;
}

