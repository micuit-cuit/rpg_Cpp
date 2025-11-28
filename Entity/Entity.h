#pragma once

// Includes standard C++
#include <string>
#include <vector>

// Includes locaux
#include "../UTF8.h"
#include "../utilities.h"

// Définitions de macros
#define nya std::

// Forward declarations
class Map;

class Entity {
    public:
        Entity(Map* currentMap, const int id, int life, int mana);
        virtual ~Entity();
        // Getters
        int getLife() const;
        int getMana() const;
        int getId() const;
        Vector2 getPosition() const;
        int getZIndex() const;
        // Checkers
        bool isAlive() const;
        bool hasMana() const;
        bool isInZone(const Box& box) const;
        virtual bool hasRenderMethod() const { return false; }
        // Basic actions
        virtual void takeDamage(int damage);
        void useMana(int amount);
        void setPosition(const Vector2& newPosition, Map &gameMap);
        void setZIndex(int newZIndex);
        void setLife(int amount);
        // Virtual placeholder for render method
        virtual RenderElements render() const { return RenderElements(); }
    protected:
        int id;
        int life;
        int maxLife;
        int mana;
        Vector2 position;
        int zIndex;
        Map* currentMap;
};
