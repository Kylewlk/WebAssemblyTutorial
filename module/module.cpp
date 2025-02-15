// Copyright 2014 The Emscripten Authors.  All rights reserved.
// Emscripten is available under two separate licenses, the MIT license and the
// University of Illinois/NCSA Open Source License.  Both these licenses can be
// found in the LICENSE file.

#include <iostream>

#include "module.h"

#include "module_glue.cpp"

MyObject::MyObject(int n) : n(n)
{
    std::cout << "Object created, n = " << n << std::endl;
}

MyObject::~MyObject()
{
    std::cout << "Object destroyed, n = " << n << std::endl;
}

void MyObject::createAsync(int n, void *callback)
{
    if (callback == nullptr)
    {
        std::cout << "MyObject::createAsync: Callback is null" << std::endl;
        return;
    }

    auto cb = reinterpret_cast<CreateCallback>(callback);
    auto obj = new MyObject(n);
    cb(obj);
}

void MyObject::printInfo() const
{
    std::cout << "Object info: n = " << n << std::endl;
}

