#pragma once

// Includes standard C++
#include <string>
#include <vector>

// Includes locaux
#include "../Entity.h"
#include "../../Item/Item.h"

// Définitions de macros
#define nya std::

// Forward declarations
class Map;

class Player : public Entity {
    public:
        Player(Map* currentMap, const int id, int life, int mana, const nya string& name);
        virtual ~Player();
        void update();
        nya string getName() const;
        // Attack function
        void attack(Entity& target, int method, int itemIndex = -1);
        // Getters
        int getForce() const;
        int getExperience() const;
        int getLevel() const;
        int getHandItemIndex() const { return handItemIndex; }
        Item *getItem(int index) const;
        nya vector<Item*> getInventory() const;
        bool isSelected() const;
        // Setters
        void takeDamage(int damage) override;
        void gainExperience(int amount);
        void increaseForce(int amount);
        void addItem(Item* item);
        void heal(int amount);
        void setHandItemIndex(int index);
        void setSelected(bool state, nya string colorCode);
        // Checkers
        bool hasRenderMethod() const { return true; }
        // Placeholder
        RenderElements render() const;
        nya string renderInventory() const;
private:
        nya string name;
        int experience;
        int force;
        nya vector<Item*> inventory;
        int handItemIndex;
        bool selected = false;
        nya string selectColorCode = "┎";
        int animStep = 0;
};
