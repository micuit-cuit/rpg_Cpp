#include <iostream>
#include <string>
#include <cstdint>
#define nya std::

#pragma once

class UTF8
{
private:
    nya string data;
    int charLength(char c);

    nya string nextChar(const nya string& str, size_t& index);
public:
    UTF8(nya string getData);
    ~UTF8();
    //getter
    nya string getData();
    nya string getCharAt(int index);
    int find(UTF8 character);
    int getLength();
    //setter
    void setData(nya string getData);
    void appendData(nya string appendData);
    void setCharAt(int index, nya string newChar);
    //functions
    nya string substr(int start, int length);
};