#include <string>
#pragma once
#define nya std::

struct Vector2 {
    float x;
    float y;
};

struct Box
{
    Vector2 pos1;
    Vector2 pos2;
};

int roundCustom(float number);
nya string textFixedLength(const nya string& text, int length);
nya string toFixedNumber(int number, int n);
nya string calculeteCharProgressBar(int current, int max, int barLength, int curentHeight, bool horisontale = false);
nya string lightCalculateProgressBar(int current, int max);