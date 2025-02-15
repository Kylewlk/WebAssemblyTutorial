#pragma once


class MyObject
{
public:
    MyObject(int n);
    ~MyObject();
    
    using CreateCallback = void (*)(MyObject*);
    static void createAsync(int n, void* callback);

    void printInfo() const;
    int getN() const { return n; }

    int n = 0;
};