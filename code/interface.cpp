// Implementation for interface
#include "interface.hpp"

class Interface::Impl {
private:
    // Separate private data.
    int a;
public:
    Impl(int n) : a(n) {}
    void draw(const Interface& i) { a++; }
};

Interface::Interface(int n) : mPimpl{std::make_unique<Impl>(n)} {}

void Interface::draw() { mPimpl->draw(*this); }


