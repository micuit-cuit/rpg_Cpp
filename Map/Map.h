#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#define nya std::

#include "../Entity/Entity.h"
#include "../Entity/Monstre/Monstre.h"
#include "../UTF8.h"

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
        nya vector<Monstre*> getMonstres() const;
    private:
        nya vector<Entity*> entities;
        nya vector<Layer> layers;
        Vector2 size;
};