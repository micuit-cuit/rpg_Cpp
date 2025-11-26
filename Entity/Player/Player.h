#pragma once
#include <string>
#include <vector>
#include "../Entity.h"
#include "../../Item/Item.h"
#define nya std::

// Forward declarations
class Map;

class Player : public Entity {
    public:
        Player(Map* currentMap, const int id, int life, int mana, const nya string& name);
        virtual ~Player();
        nya string getName() const;
        //fonction d'ataque
        void attack(Entity& target, int method, int itemIndex = -1);
        //getters
        int getForce() const;
        int getExperience() const;
        int getLevel() const;
        int getHandItemIndex() const { return handItemIndex; }
        Item getItem(int index) const;
        nya vector<Item> getInventory() const;
        //setters
        void gainExperience(int amount);
        void increaseForce(int amount);
        void addItem(Item* item);
        void heal(int amount);
        void setHandItemIndex(int index);
        //iftters
        bool hasRenderMethod() const { return true; }
        //placeholder
        RenderElements render() const;
        nya string renderInventory() const;
private:
        nya string name;
        int experience;
        int force;
        nya vector<Item*> inventory;
        int handItemIndex;
};

        