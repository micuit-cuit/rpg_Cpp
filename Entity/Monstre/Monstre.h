#pragma once
#include <string>
#include <vector>
#include "../Entity.h"
#include "../lootTable.h"
#include "../../Item/Item.h"
#define nya std::

// Forward declarations
class Map;

class Monstre : public Entity {
    public:
        Monstre(Map* currentMap, const int id, int life, int mana, const nya string& name);
        virtual ~Monstre();
        nya string getName() const;
        //fonction d'ataque
        void attack(Entity& target, int method);
        //getters
        int getForce() const;
        int getExperience() const;
        int getLevel() const;
        bool isSelected() const;
        Item *getItem(int index) const;
        //setters
        void setSelected(bool state, nya string colorCode = "┎");
        void setExperience(int amount);
        void setForce(int amount);
        void setHandItem(Item* item);
        void setLife(int amount);
        //iftters
        bool hasRenderMethod() const { return true; }
        //placeholder
        RenderElements render() const;

    private:
        nya string monstreHead = "🕷";
        bool selected = false;
        nya string name;
        int experience;
        int force;
        nya string selectColorCode = "┎";
        Item *handItem;
        nya vector<lootTable> lootTables;
};

        