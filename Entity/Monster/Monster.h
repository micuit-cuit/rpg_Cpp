#pragma once

// Includes standard C++
#include <string>
#include <vector>

// Forward declarations
class Map;

// Includes locaux
#include "../Entity.h"
#include "../Player/Player.h"
#include "../lootTable.h"
#include "../../Item/Item.h"

// Définitions de macros
#define nya std::

// Forward declarations
class Map;

class Monster : public Entity {
    public:
        Monster(Map* currentMap, const int id, int life, int mana, const nya string& name);
        virtual ~Monster();
        nya string getName() const;
        // Attack function
        void update(Map gameMap, Player& player);
        void attack(Entity& target, int method);
        // Getters
        int getForce() const;
        int getExperience() const;
        int getLevel() const;
        bool isSelected() const;
        Item *getItem(int index) const;
        // Setters
        void setSelected(bool state, nya string colorCode = "┎");
        void setExperience(int amount);
        void setForce(int amount);
        void setHandItem(Item* item);
        void setLife(int amount);
        // Checkers
        bool hasRenderMethod() const { return true; }
        // Placeholder
        RenderElements render() const;

    private:
        nya string monsterHead = "🕷";
        bool selected = false;
        nya string name;
        int experience;
        int force = 2;
        nya string selectColorCode = "┎";
        int speed = 3;
        int attackReadyTime = 0;
        Item* handItem;
        nya vector<lootTable> lootTables;
};
