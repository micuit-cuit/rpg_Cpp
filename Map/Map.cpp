#include "Map.h"

// Includes locaux
#include "../UTF8.h"

// Définitions de macros
#define nya std::

#define COLOR_ASCII_CHAR "─━│┃┄┅┆┇┈┉┊┋┌┍┎┏┐┑┒┓└┕┖┗┘┙┚┛├┝┞┟┠┡┢┣┤┥┦┧┨┩┪┫┬┭┮┯┰┱┲┳┴┵┶┷┸┹┺┻┼┽┾┿╀╁╂╃╄╅╆╇╈╉╊╋╌╍╎╏═║╒╓╔╕╖╗╘╙╚╛╜╝╞╟╠╡╢╣╤╥╦╧╨╩╪╫╬╭╮╯╰╱╲╳╴╵╶╷╸╹╺╻╼╽╾╿▀▁▂▃▄▅▆▇█▉▊▋▌▍▎▏▐░▒▓▔▕▖▗▘▙▚▛▜▝▞▟■□▢▣▤▥▦▧▨▩▪▫▬▭▮▯▰▱▲△▴▵▶▷▸▹►▻▼▽▾▿◀◁◂◃◄◅◆◇◈◉◊○◌◍◎●◐◑◒◓◔◕◖◗◘◙◚◛◜◝◞◟◠◡◢◣◤◥◦◧◨◩◪◫◬◭◮◯◰◱◲◳◴◵◶◷◸◹◺◻◼◽◾+"

Map::Map(Vector2 size, nya vector<UTF8> background, nya vector<UTF8> lights) {
    this->size = size;
    Layer baseLayer;
    baseLayer.tiles = background;
    baseLayer.lights = lights;
    baseLayer.zIndex = -1;
    layers.push_back(baseLayer);
}

Map::~Map() {}

void Map::setBackground(const nya vector<UTF8>& background, const nya vector<UTF8>& lights) {
    if (!layers.empty()) {
        layers[0].tiles = background;
        layers[0].lights = lights;
    }
}

void Map::addLayer(nya vector<UTF8> tiles, nya vector<UTF8> lights, int zIndex) {
    if (hasLayer(zIndex)) {
        return;
    }
    Layer layer;
    layer.tiles = tiles;
    layer.lights = lights;
    layer.zIndex = zIndex;
    layers.push_back(layer);
}

int Map::hasLayer(int zIndex) const {
    for (const auto& layer : layers) {
        if (layer.zIndex == zIndex) {
            return true;
        }
    }
    return false;
}

void Map::setEntities(const nya vector<Entity*>& newEntities) {
    entities = newEntities;
}

Layer Map::getLayer(int zIndex) const {
    for (const auto& layer : layers) {
        if (layer.zIndex == zIndex) {
            return layer;
        }
    }
    Layer emptyLayer;
    emptyLayer.zIndex = -9999;
    return emptyLayer;
}

void Map::addEntity(Entity* entity) {
    entities.push_back(entity);
}

Vector2 Map::getSize() const {
    return size;
}

nya vector<Entity*> Map::getEntities() const {
    return entities;
}

nya vector<Monster*> Map::getMonsters() const {
    nya vector<Monster*> monsters;
    for (const auto& entity : entities) {
        Monster* monster = dynamic_cast<Monster*>(entity);
        if (monster != nullptr) {
            monsters.push_back(monster);
        }
    }
    return monsters;
}

// Collision boxes management
void Map::addCollisionBox(const Box& box) {
    collisionBoxes.push_back(box);
}

void Map::setCollisionBoxes(const nya vector<Box>& boxes) {
    collisionBoxes = boxes;
}

nya vector<Box> Map::getCollisionBoxes() const {
    return collisionBoxes;
}

bool Map::checkCollision(const Vector2& position) const {
    for (const Box& obstacle : collisionBoxes) {
        if (position.x >= obstacle.pos1.x && position.x <= obstacle.pos2.x &&
            position.y >= obstacle.pos1.y && position.y <= obstacle.pos2.y) {
            return true;
        }
    }
    return false;
}

nya string Map::render()
{
    UTF8 colorAsciiChar(COLOR_ASCII_CHAR);
    nya string renderedMap;
    // Copy map layers

    nya vector<Layer> layers = this->layers;
    // For each entity, check if it has a render method and call it
    for (const auto& entity : entities) {
        if (entity->hasRenderMethod()) {
            RenderElements elements = entity->render();
            // If the entity is outside the map, ignore it
            if (elements.position.x < 0 || elements.position.x > size.x ||
                elements.position.y < 0 || elements.position.y > size.y) {
                continue;
            }
            Layer layer;
            int zIndex = elements.zIndex;
            while (hasLayer(zIndex))
                zIndex++; // Avoid zIndex collisions
            layer.tiles = {};
            layer.zIndex = zIndex;
            // Add content to layer at entity position
            int posX = static_cast<int>(elements.position.x - elements.localPosition.x);
            int posY = static_cast<int>(elements.position.y - elements.localPosition.y);

            if (layer.zIndex != -9999) {
                for (size_t i = 0; i < elements.content.size(); i++) {
                    // Test if the line exists and is long enough
                    if (posY + i < 0 || posY + i >= size.y) {
                        continue; // Ignore lines outside the map
                    }
                    while (layer.tiles.size() <= static_cast<size_t>(posY + i)) {
                        layer.tiles.push_back(nya string(50, ' ')); // Add empty lines if necessary
                        layer.lights.push_back(nya string(50, ' ')); // Add lines filled with ' ' if necessary
                    }
                    while (layer.tiles[posY + i].getLength() <= static_cast<size_t>(posX + elements.content[i].getLength())) {
                        layer.tiles[posY + i].appendData(nya string(50, ' '));
                        layer.lights[posY + i].appendData(nya string(50, ' '));
                    }
                    // Add content
                    for (size_t j = 0; j < elements.content[i].getLength(); j++) {
                        layer.tiles[posY + i].setCharAt(posX + j, elements.content[i].getCharAt(j));
                        layer.lights[posY + i].setCharAt(posX + j, elements.lights[i].getCharAt(j));
                    }
                }
            }
            // Update or add layer
            if (!hasLayer(layer.zIndex)) {
                layers.push_back(layer);
            }
        }
    }
    // Merge layers by zIndex and display
    // Sort layers by zIndex
    nya vector<Layer> sortedLayer = layers;
    nya sort(sortedLayer.begin(), sortedLayer.end(), [](const Layer& a, const Layer& b) {
        return a.zIndex > b.zIndex;
        });
    // List sorted layers for rendering
    // Display
    Layer renderedLayer;
    for (int y = 0; y < size.y; y++) {
        UTF8 line("");
        UTF8 lightLine("");
        for (int x = 0; x < size.x; x++) {
            nya string tilestring = " ";
            nya string lightstring = " ";
            for (auto& layer : sortedLayer) {
                if (y < static_cast<int>(layer.tiles.size()) && x < static_cast<int>(layer.tiles[y].getLength())) {
                    nya string currentstring = layer.tiles[y].getCharAt(x);
                    if (currentstring != " ") {
                        tilestring = currentstring;
                        break; // Take the first non-empty character
                    }
                }
            }
            for (auto& layer : sortedLayer) {
                if (y < static_cast<int>(layer.lights.size()) && x < static_cast<int>(layer.lights[y].getLength())) {
                    nya string currentstring = layer.lights[y].getCharAt(x);
                    if (currentstring != " ") {
                        lightstring = currentstring;
                        break; // Take the first non-empty character
                    }
                }
            }
            line.appendData(tilestring);
            lightLine.appendData(lightstring);
        }
        renderedLayer.tiles.push_back(line);
        renderedLayer.lights.push_back(lightLine);
    }
    // Render the lighting
    for (int y = 0; y < size.y; y++) {
        for (int x = 0; x < size.x; x++) {
            UTF8 lightChar(renderedLayer.lights[y].getCharAt(x));
            if (lightChar.getCharAt(0) == " ") {
                renderedMap += " ";
            }
            else {
                nya string colorCode = "\033[38;5;" + nya to_string(colorAsciiChar.find(lightChar)) + "m"; // default
                renderedMap += colorCode + renderedLayer.tiles[y].getCharAt(x) + "\033[0m";
            }
        }
        renderedMap += "\n";
    }
    return renderedMap;
}
