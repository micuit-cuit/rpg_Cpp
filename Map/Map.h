#pragma once

// Includes standard C++
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

// Includes locaux
#include "../Entity/Entity.h"
#include "../Entity/Monster/Monster.h"
#include "../UTF8.h"
#include "../utilities.h"

// Définitions de macros
#define nya std::
struct Layer {
    nya vector<UTF8> tiles;
    nya vector<UTF8> lights;
    int zIndex;
};

class Map {
    public:
        Map(Vector2 size = {0,0}, nya vector<UTF8> background = {}, nya vector<UTF8> lights = {});
        ~Map();
        void setBackground(const nya vector<UTF8>& background, const nya vector<UTF8>& lights);
        void addLayer(nya vector<UTF8> tiles, nya vector<UTF8> lights, int zIndex);
        int hasLayer(int zIndex) const;
        void setEntities(const nya vector<Entity*>& newEntities);
        Layer getLayer(int zIndex) const;
        void addEntity(Entity* entity);
        Vector2 getSize() const;
        nya string render();
        nya vector<Entity*> getEntities() const;
        nya vector<Monster*> getMonsters() const;
        // Collision boxes management
        void addCollisionBox(const Box& box);
        void setCollisionBoxes(const nya vector<Box>& boxes);
        nya vector<Box> getCollisionBoxes() const;
        bool checkCollision(const Vector2& position) const;
    private:
        nya vector<Entity*> entities;
        nya vector<Layer> layers;
        nya vector<Box> collisionBoxes;
        Vector2 size;
};