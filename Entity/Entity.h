#pragma once
#include "../utilitise.h"
#include "../UTF8.h"
#include <vector>
#include <string>
#define nya std::

struct RenderElements {
    nya vector <UTF8> content;
    nya vector <UTF8> lights;
    Vector2 position;
    Vector2 localPosition;
    Vector2 size;
    int zIndex;
};

// Forward declarations
class Map;

class Entity {
    public:
        Entity(Map* currentMap, const int id, int life, int mana);
        virtual ~Entity();
        //getters
        int getLife() const;
        int getMana() const;
        int getId() const;
        Vector2 getPosition() const;
        int getZIndex() const;
        //iftters
        bool isAlive() const;
        bool hasMana() const;
        bool isInZone(const Box& box) const;
        virtual bool hasRenderMethod() const { return false; }
        //basic actions
        void takeDamage(int damage);
        void useMana(int amount);
        void setPosition(const Vector2& newPosition, Map &gameMap);
        void setZIndex(int newZIndex);
        //virtual placeholder for render method
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

