// Interface with no implementation
#pragma once

#include <memory>

class Interface {
    class Impl;
    std::unique_ptr<Impl> mPimpl;

public:
    // Only provide api declaration, not data and not api implementation.
    // Data and api implementation are stored in class Impl.
    Interface(int n);

    void draw();
};
