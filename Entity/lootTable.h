#pragma once

// Includes standard C++
#include <vector>

// Includes locaux
#include "../Item/Item.h"

// Définitions de macros
#define nya std::

struct lootTableIndex {
    Item item;       // The item to be dropped
    float dropChance; // Probability of dropping the item (0.0 to 1.0)
};

struct lootTable {
    nya vector<lootTableIndex> items;
};