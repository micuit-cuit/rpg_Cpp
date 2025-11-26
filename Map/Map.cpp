
#include "Map.h"
#include "../UTF8.h"
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
nya vector<Monstre*> Map::getMonstres() const {
    nya vector<Monstre*> monstres;
    for (const auto& entity : entities) {
        Monstre* monstre = dynamic_cast<Monstre*>(entity);
        if (monstre != nullptr) {
            monstres.push_back(monstre);
        }
    }
    return monstres;
}


nya string Map::render()
{
    UTF8 colorAsciiChar(COLOR_ASCII_CHAR);
    nya string renderedMap;
    //coppie des layers de la map

    nya vector<Layer> layers = this->layers;
    //pour chaque entity, je verifie si elle a une methode de rendu et je l'appelle
    for (const auto& entity : entities) {
        if (entity->hasRenderMethod()) {
            RenderElements elements = entity->render();
            // if l'entity est a l'exterieur de la map, on l'ignore
            if (elements.position.x < 0 || elements.position.x > size.x ||
                elements.position.y < 0 || elements.position.y > size.y) {
                continue;
            }
            Layer layer;
            int zIndex = elements.zIndex;
            while (hasLayer(zIndex))
                zIndex++; //éviter les collisions de zIndex
            layer.tiles = {};
            layer.zIndex = zIndex;
            //ajouter le contenu a la layer a la position de l'entity
            int posX = static_cast<int>(elements.position.x - elements.localPosition.x);
            int posY = static_cast<int>(elements.position.y - elements.localPosition.y);

            if (layer.zIndex != -9999) {
                for (size_t i = 0; i < elements.content.size(); i++) {
                    //test si la ligne existe et si elle est assez longue
                    if (posY + i < 0 || posY + i >= size.y) {
                        continue; //ignorer les lignes hors de la map
                    }
                    while (layer.tiles.size() <= static_cast<size_t>(posY + i)) {
                        layer.tiles.push_back(nya string(50, ' ')); //ajouter des lignes vides si necessaire
                        layer.lights.push_back(nya string(50, ' ')); //ajouter des lignes remplies de '<' si necessaire (indique case vide)
                    }
                    while (layer.tiles[posY + i].getLength() <= static_cast<size_t>(posX + elements.content[i].getLength())) {
                        layer.tiles[posY + i].appendData(nya string(50, ' '));
                        layer.lights[posY + i].appendData(nya string(50, ' '));
                    }
                    //ajouter le contenu
                    for (size_t j = 0; j < elements.content[i].getLength(); j++) {
                        layer.tiles[posY + i].setCharAt(posX + j, elements.content[i].getCharAt(j));
                        layer.lights[posY + i].setCharAt(posX + j, elements.lights[i].getCharAt(j));
                    }
                }
            }
            //mettre a jour ou ajouter la layer
            if (!hasLayer(layer.zIndex)) {
                layers.push_back(layer);
            }
        }
    }
    //fusionner les layers par zIndex et afficher
    //tri des layers par zIndex
    nya vector<Layer> sortedLayer = layers;
    nya sort(sortedLayer.begin(), sortedLayer.end(), [](const Layer& a, const Layer& b) {
        return a.zIndex > b.zIndex;
        });
    //lister les layers triés pour le rendu
    //affichage
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
                        break; //prendre le premier caractere non vide
                    }
                }
            }
            for (auto& layer : sortedLayer) {
                if (y < static_cast<int>(layer.lights.size()) && x < static_cast<int>(layer.lights[y].getLength())) {
                    nya string currentstring = layer.lights[y].getCharAt(x);
                    if (currentstring != " ") {
                        lightstring = currentstring;
                        break; //prendre le premier caractere non vide
                    }
                }
            }
            line.appendData(tilestring);
            lightLine.appendData(lightstring);
        }
        renderedLayer.tiles.push_back(line);
        renderedLayer.lights.push_back(lightLine);
    }
    //fais le rendue de la lumiere
    for (int y = 0; y < size.y; y++) {
        for (int x = 0; x < size.x; x++) {
            UTF8 lightChar(renderedLayer.lights[y].getCharAt(x));
            if (lightChar.getCharAt(0) == " ") {
                renderedMap += " ";
            }
            else {
                nya string colorCode = "\033[38;5;" + nya to_string(colorAsciiChar.find(lightChar)) + "m"; //default
                // nya string colorCode = nya to_string((unsigned int)lightChar); //default
                renderedMap += colorCode + renderedLayer.tiles[y].getCharAt(x) + "\033[0m";
            }
        }
        renderedMap += "\n";
    }
    //renvois sur la sortie d'ereur le rendu de la lumiere pour debug
    // nya string debugLightRender;
    // for (int y = 0; y < size.y; y++) {
    //     for (int x = 0; x < size.x; x++) {
    //         UTF8 lightChar(renderedLayer.lights[y].getCharAt(x));
    //         if (lightChar.getCharAt(0) == " ") {
    //             debugLightRender += " ";
    //         }
    //         else {
    //             nya string colorCode = "\033[38;5;" + nya to_string(colorAsciiChar.find(lightChar)) + "m"; //default
    //             // nya string colorCode = nya to_string((unsigned int)lightChar); //default
    //             debugLightRender += colorCode + lightChar.getData() + "\033[0m";
    //         }
    //     }
    //     debugLightRender += "\n";
    // }
    // nya cerr << debugLightRender;
    return renderedMap;
}
// nya string Map::render()
// {
//     UTF8 colorAsciiChar(COLOR_ASCII_CHAR);
//     nya string renderedMap;
//     //coppie des layers de la map

//     nya vector<Layer> layers = this->layers;
//     //pour chaque entity, je verifie si elle a une methode de rendu et je l'appelle
//     for (const auto& entity : entities) {
//         if (entity->hasRenderMethod()) {
//             RenderElements elements = entity->render();
//             // if l'entity est a l'exterieur de la map, on l'ignore
//             if (elements.position.x < 0 || elements.position.x > size.x ||
//                 elements.position.y < 0 || elements.position.y > size.y) {
//                 continue;
//             }
//             Layer layer;
//             if (!hasLayer(elements.zIndex)) {
//                 layer.tiles = {};
//                 layer.zIndex = elements.zIndex;
//             } else {
//                 layer = getLayer(elements.zIndex);
//             }
//             //ajouter le contenu a la layer a la position de l'entity
//             int posX = static_cast<int>(elements.position.x- elements.localPosition.x);
//             int posY = static_cast<int>(elements.position.y - elements.localPosition.y);

//             if (layer.zIndex != -9999) {
//                 for (size_t i = 0; i < elements.content.size(); i++) {
//                     //test si la ligne existe et si elle est assez longue
//                     if (posY + i < 0 || posY + i >= size.y) {
//                         continue; //ignorer les lignes hors de la map
//                     }
//                     while (layer.tiles.size() <= static_cast<size_t>(posY + i)) {
//                         layer.tiles.push_back(nya string(50, ' ')); //ajouter des lignes vides si necessaire
//                         layer.lights.push_back(nya string(50, ' ')); //ajouter des lignes remplies de '<' si necessaire (indique case vide)
//                     }
//                     while (layer.tiles[posY + i].getLength() <= static_cast<size_t>(posX + elements.content[i].getLength())) {
//                         layer.tiles[posY + i].appendData(nya string(50, ' '));
//                         layer.lights[posY + i].appendData(nya string(50, ' '));
//                     }
//                     //ajouter le contenu
//                     for (size_t j = 0; j < elements.content[i].getLength(); j++) {
//                         layer.tiles[posY + i].setCharAt(posX + j, elements.content[i].getCharAt(j));
//                         layer.lights[posY + i].setCharAt(posX + j, elements.lights[i].getCharAt(j));
//                     }
//                 }
//             }
//             //mettre a jour ou ajouter la layer
//             if (!hasLayer(layer.zIndex)) {
//                 layers.push_back(layer);
//             }
//         }
//     }
//     //fusionner les layers par zIndex et afficher
//     //tri des layers par zIndex
//     nya vector<Layer> sortedLayer = layers;
//     nya sort(sortedLayer.begin(), sortedLayer.end(), [](const Layer& a, const Layer& b) {
//         return a.zIndex > b.zIndex;
//     });
//     //lister les layers triés pour le rendu
//     //affichage
//     Layer renderedLayer;
//     for (int y = 0; y < size.y; y++) {
//         UTF8 line("");
//         UTF8 lightLine("");
//         for (int x = 0; x < size.x; x++) {
//             nya string tilestring = " ";
//             nya string lightstring = " ";
//             for (auto& layer : sortedLayer) {
//                 if (y < static_cast<int>(layer.tiles.size()) && x < static_cast<int>(layer.tiles[y].getLength())) {
//                     nya string currentstring = layer.tiles[y].getCharAt(x);
//                     if (currentstring != " ") {
//                         tilestring = currentstring;
//                         break; //prendre le premier caractere non vide
//                     }
//                 }
//             }
//             for (auto& layer : sortedLayer) {
//                 if (y < static_cast<int>(layer.lights.size()) && x < static_cast<int>(layer.lights[y].getLength())) {
//                     nya string currentstring = layer.lights[y].getCharAt(x);
//                     if (currentstring != " ") {
//                         lightstring = currentstring;
//                         break; //prendre le premier caractere non vide
//                     }
//                 }
//             }
//             line.appendData(tilestring);
//             lightLine.appendData(lightstring);
//         }
//         renderedLayer.tiles.push_back(line);
//         renderedLayer.lights.push_back(lightLine);
//     }
//     //fais le rendue de la lumiere
//     for (int y = 0; y < size.y; y++) {
//         for (int x = 0; x < size.x; x++) {
//             UTF8 lightChar(renderedLayer.lights[y].getCharAt(x));
//             if (lightChar.getCharAt(0) == " ") {
//                 renderedMap += " ";
//             }else {
//                 nya string colorCode = "\033[38;5;" + nya to_string(colorAsciiChar.find(lightChar)) + "m"; //default
//                 // nya string colorCode = nya to_string((unsigned int)lightChar); //default
//                 renderedMap += colorCode + renderedLayer.tiles[y].getCharAt(x) + "\033[0m";
//             }
//         }
//         renderedMap += "\n";
//     }
//     //renvois sur la sortie d'ereur le rendu de la lumiere pour debug
//     // nya string debugLightRender;
//     // for (int y = 0; y < size.y; y++) {
//     //     for (int x = 0; x < size.x; x++) {
//     //         UTF8 lightChar(renderedLayer.lights[y].getCharAt(x));
//     //         if (lightChar.getCharAt(0) == " ") {
//     //             debugLightRender += " ";
//     //         }
//     //         else {
//     //             nya string colorCode = "\033[38;5;" + nya to_string(colorAsciiChar.find(lightChar)) + "m"; //default
//     //             // nya string colorCode = nya to_string((unsigned int)lightChar); //default
//     //             debugLightRender += colorCode + lightChar.getData() + "\033[0m";
//     //         }
//     //     }
//     //     debugLightRender += "\n";
//     // }
//     // nya cerr << debugLightRender;
//     return renderedMap;
// }