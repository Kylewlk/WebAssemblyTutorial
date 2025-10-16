// Copyright 2014 The Emscripten Authors.  All rights reserved.
// Emscripten is available under two separate licenses, the MIT license and the
// University of Illinois/NCSA Open Source License.  Both these licenses can be
// found in the LICENSE file.

#include <iostream>
#include <emscripten.h>
#include <emscripten/html5.h>

#include "module.h"

#include "module_glue.cpp"

namespace MyNameSpace
{


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
    struct Params
    {
        int n{};
        CreateCallback cb{};
    };
    auto params = new Params{n, reinterpret_cast<CreateCallback>(callback)};
    
    emscripten_set_timeout([](void* useData){
        auto params = reinterpret_cast<Params *>(useData);
        auto cb = reinterpret_cast<CreateCallback>(params->cb);
        
        if (params->n <= 0)
        {
            cb(nullptr);
        }
        else
        {
            auto obj = new MyObject(params->n);
            cb(obj);
        }
        delete params;
    }, 1000, params);
}

void MyObject::printInfo() const
{
    std::cout << "Object info: n = " << n << std::endl;
    std::cerr << "Error info: n = " << n << std::endl;
}

}