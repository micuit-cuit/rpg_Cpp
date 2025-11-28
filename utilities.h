#pragma once

// Includes standard C++
#include <string>
#include <vector>

// Includes locaux
#include "UTF8.h"

// Définitions de macros
#define nya std::

// Structures de base
struct Vector2 {
    int x;
    int y;
};

struct Box
{
    Vector2 pos1;
    Vector2 pos2;
};

// Structure de rendu
struct RenderElements {
    nya vector <UTF8> content;
    nya vector <UTF8> lights;
    Vector2 position;
    Vector2 localPosition;
    Vector2 size;
    int zIndex;
};

int calculateDistance(Vector2 p1, Vector2 p2);
int roundCustom(float number);
nya string textFixedLength(const nya string& text, int length);
nya string toFixedNumber(int number, int n);
nya string calculateCharProgressBar(int current, int max, int barLength, int currentHeight, bool horizontal = false);
nya string lightCalculateProgressBar(float current, float max);
RenderElements RenderElementsSelected(RenderElements elements, nya string selectColorCode = "┎", int life = 100, int maxLife = 100, bool withLifeBar = true);
