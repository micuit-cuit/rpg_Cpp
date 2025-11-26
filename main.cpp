#include <iostream>
#include <memory>
#include <fstream>
#ifdef __EMSCRIPTEN__
#include <thread>
#include <chrono>
#endif
#include "Entity/Entity.h"
#include "Entity/Player/Player.h"
#include "Entity/Monstre/Monstre.h"
#include "Item/Item.h"
#include "Item/Weapon/Weapon.h"
#include "Item/Weapon/Sword.h"
#include "Item/Weapon/Staff.h"
#include "Item/Weapon/Bow.h"
#include "attackConst.h"
#include "Map/Map.h"
#include "UTF8.h"
#include "term_getchar.h"
#include "clearscreen.h"
#include "utilitise.h"


#include <string>
#include <locale>
#include <codecvt>
#define nya std::




bool strStartsWith(const nya string& str, const nya string& prefix) {
    return str.rfind(prefix, 0) == 0;
}
nya string strKep(const nya string& str, const nya string& table) {
    nya string result;
    for (char c : str) {
        if (table.find(c) != nya string::npos) {
            result += c;
        }
    }
    return result;
}


nya vector<Layer> mapLoader(nya string filePath) {
    //test si le fichier existe
    nya ifstream file(filePath);
    if (!file.is_open()) {
        nya cerr << "Erreur: Impossible d'ouvrir le fichier " << filePath << nya endl;
        return {};
    }
    //lire le fichier ligne par ligne
    bool inMap = false;
    bool inLights = false;
    int layerIndex = -1;
    nya vector<Layer> layers;
    nya string line;
    while (nya getline(file, line)) {
        if (strStartsWith(line, "<<light")) {
            inLights = true;
            inMap = false;
            //lire l'index
            layerIndex = nya stoi(strKep(line, "0123456789"));
        } else if (strStartsWith(line, "<<l")) {
            inMap = true;
            inLights = false;
            //lire l'index
            layerIndex = nya stoi(strKep(line, "0123456789"));
        } else {
            //get la ligne et add la au layer correspondant
            if (inMap) {
                //trouver le layer
                while (static_cast<int>(layers.size()) <= layerIndex) {
                    Layer newLayer;
                    newLayer.zIndex = static_cast<int>(layers.size());
                    layers.push_back(newLayer);
                }
                layers[layerIndex].tiles.push_back(UTF8(line));
            }else if (inLights) {
                //trouver le layer
                while (static_cast<int>(layers.size()) <= layerIndex) {
                    Layer newLayer;
                    newLayer.zIndex = static_cast<int>(layers.size());
                    layers.push_back(newLayer);
                }
                layers[layerIndex].lights.push_back(UTF8(line));
            }
        }
    }
    file.close();
    nya cout << "Map chargée avec " << layers.size() << " layers." << nya endl;
    return layers;
}
bool moveEntity(Entity& entity, const Vector2& newPosition, Map& gameMap, Vector2 mapSize, nya vector<Box> obstacles = {}) {
    //vérifier si la nouvelle position est dans les limites de la map
    Vector2 lastPosition = entity.getPosition();
    entity.setPosition(newPosition, gameMap);
    if (entity.isInZone({ {0, 0}, mapSize }) == false) {
        entity.setPosition(lastPosition, gameMap);
        return false;
    }
    //vérifier si la nouvelle position est dans les obstacles
    for (const Box& obstacle : obstacles) {
        if (entity.isInZone(obstacle)) {
            entity.setPosition(lastPosition, gameMap);
            return false;
        }
    }
    //déplacer l'entité
    entity.setZIndex(newPosition.y); //mettre a jour le zIndex en fonction de la position y
    return true;
}
/*
<<o0>>
0,0,2,46
3,0,3,28
4,0,4,12
4,52,4,57
11,6,11,11
*/

int main() {
    TermiosGuard termGuard; // RAII pour restaurer les paramètres du terminal
    termGuard.enableRaw();  // Activer le mode raw
    nya vector<Layer> layers = mapLoader("assets/ground.map");
    nya vector<Box> obstacles = {
        {{0,0}, {46,2}},
        {{0,3}, {28,3}},
        {{0,4}, {12,4}},
        {{52,3}, {57,3}},
        {{6,10}, {11,10}}
    };
    //display map
    for (int y = 0; y <= 15; y++) {
        for (int x = 0; x <= 95; x++) {
            //if in Box obstacles
            bool inObstacle = false;
            for (const Box& obstacle : obstacles) {
                if (x >= obstacle.pos1.x && x <= obstacle.pos2.x &&
                    y >= obstacle.pos1.y && y <= obstacle.pos2.y) {
                    nya cout << "#";
                    inObstacle = true;
                    break;
                }
            }
            if (inObstacle == false) nya cout << ".";
        }
        nya cout << nya endl;
    }
    // return 0;
    Map gameMap({ 95, 15 }, layers[0].tiles, layers[0].lights);
    gameMap.addLayer(layers[2].tiles, layers[2].lights, 3);
    gameMap.addLayer(layers[3].tiles, layers[3].lights, 10);
    nya vector<Item*> allItemsList;
    allItemsList.push_back(new Sword(1, "Épée de fer", 20, 150, nya string("Une épée solide en fer.")));
    allItemsList.push_back(new Bow(3, "Arc en bois", 15, 100, nya string("Un arc simple en bois.")));
    allItemsList.push_back(new Staff(4, "Bâton de mage", 25, 80, nya string("Un bâton utilisé par les mages."), "🪄"));
    allItemsList.push_back(new Sword(2, "Épée rouillée", 10, 80, nya string("Une épée rouillée mais encore tranchante.")));
    allItemsList.push_back(new Bow(5, "Arc cassé", 5, 50, nya string("Un arc cassé, difficile à utiliser.")));
    allItemsList.push_back(new Staff(6, "Bâton tordu", 8, 60, nya string("Un bâton tordu mais magique."), "🪄"));
    Player player(&gameMap, 1, 100, 80, "Héros");
    Monstre monster(&gameMap, 2, 60, 10, "Orc");
    Monstre monster2(&gameMap, 2, 60, 10, "Gobelin");
    Monstre monster3(&gameMap, 3, 80, 20, "Troll");
    gameMap.addEntity(&player);
    gameMap.addEntity(&monster);
    gameMap.addEntity(&monster2);
    gameMap.addEntity(&monster3);
    player.addItem(allItemsList[0]);//Épée de fer
    player.addItem(allItemsList[1]);//Arc en bois
    player.addItem(allItemsList[2]);//Bâton de mage
    player.setHandItemIndex(0);
    player.setPosition({ 12,10 }, gameMap);
    player.setZIndex(10);
    monster.setPosition({ 59,4 }, gameMap);
    monster.setHandItem(allItemsList[3]);//Épée rouillée
    monster.setZIndex(1);
    monster.setLife(30);
    monster2.setPosition({ 70,8 }, gameMap);
    monster2.setHandItem(allItemsList[4]);//Arc cassé
    monster2.setZIndex(1);
    monster2.setLife(20);
    monster3.setPosition({ 80,5 }, gameMap);
    monster3.setHandItem(allItemsList[5]);//Bâton tordu
    monster3.setZIndex(1);
    monster3.setLife(50);

    float fpsCounter = 0.0f;
    int lastTime = 0;
    int nextBackgroundUpdate = 2;
    int lastBackgroundID = 0;
    int stepAtack = 0;
    int monsterSelectedAtack = -1;
    int fpsTab[60] = {0};
    while (true) {
        for (Item* item : allItemsList) {
            if (item->isWeapon() == "staff" || item->isWeapon() == "bow") {
                Weapon* weapon = dynamic_cast<Weapon*>(item);
                if (weapon) {
                    weapon->decreaseReloadTime();
                }
            }
        }
        if (stepAtack >= 2 && stepAtack < 10) {
            stepAtack++;
            if (stepAtack % 2 == 0) {
                Monstre* targetMonster = gameMap.getMonstres()[monsterSelectedAtack];
                targetMonster->setSelected(false);
            }
            else {
                Monstre* targetMonster = gameMap.getMonstres()[monsterSelectedAtack];
                targetMonster->setSelected(true, "╘");
            }
        }else if (stepAtack >= 10) {
            stepAtack = 0;
            Monstre* targetMonster = gameMap.getMonstres()[monsterSelectedAtack];
            targetMonster->setSelected(false);
            if (targetMonster->isAlive() == false) {
                //retirer le monstre de la map
                nya vector<Entity*> currentEntities = gameMap.getEntities();
                nya vector<Entity*> newEntities;
                for (Entity* entity : currentEntities) {
                    if (entity != targetMonster) {
                        newEntities.push_back(entity);
                    }
                }
                gameMap.setEntities(newEntities);
                targetMonster = nullptr;
            }
            monsterSelectedAtack = -1;
        }

        //efface l'écran
        ClearScreen();
        nextBackgroundUpdate--;
        if (nextBackgroundUpdate <= 0) {
            //mettre a jour le background
            if (lastBackgroundID == 0) lastBackgroundID = 1; else lastBackgroundID = 0;
                
            gameMap.setBackground(layers[lastBackgroundID].tiles, layers[lastBackgroundID].lights);
            nextBackgroundUpdate = 2;
        }
        //add fps to fpsTab
        for (int i = 58; i >= 0; i--) {
            fpsTab[i + 1] = fpsTab[i];
        }
        fpsTab[0] = static_cast<int>(fpsCounter);
        //calculate average fps
        int fpsSum = 0;
        for (int i = 0; i < 60; i++) {
            fpsSum += fpsTab[i];
        }
        fpsCounter = static_cast<float>(fpsSum) / 60.0f;
        //
        lastTime = clock();
        nya cout << gameMap.render();
        int currentTime = clock();
        nya cout << "FPS: " << static_cast<int>(fpsCounter) << nya endl;
        // nya cout << "Vie: " << player.getLife() << " | Mana: " << player.getMana() << " | Exp: " << player.getExperience() << " | Force: " << player.getForce() << " | Position: (" << static_cast<int>(player.getPosition().x) << "," << static_cast<int>(player.getPosition().y) << ") | FPS: " << static_cast<int>(fpsCounter) << nya endl; 
        nya cout << player.renderInventory() << nya endl;
        //attendre une entrée pour continuer
        float deltaTime = static_cast<float>(currentTime - lastTime) / CLOCKS_PER_SEC;
        fpsCounter = 1.0f / deltaTime;
        #ifdef __EMSCRIPTEN__
        char ch = getch();
        nya this_thread::sleep_for(nya chrono::milliseconds(100));
        #else
        char ch = getch_timeout(100);
        #endif
        if (ch == -1) continue; //si pas d'entrée, continuer
        switch (ch) {
            case 'c':
                return 0;
            case 'z':
                moveEntity(player, { player.getPosition().x, player.getPosition().y - 1 }, gameMap, gameMap.getSize(), obstacles);
                break;
            case 's':
                moveEntity(player, { player.getPosition().x, player.getPosition().y + 1 }, gameMap, gameMap.getSize(), obstacles);
                break;
            case 'd':
                moveEntity(player, { player.getPosition().x + 1, player.getPosition().y }, gameMap, gameMap.getSize(), obstacles);
                break;
            case 'q':
                moveEntity(player, { player.getPosition().x - 1, player.getPosition().y }, gameMap, gameMap.getSize(), obstacles);
                break;
            case ',':
                player.setHandItemIndex(player.getHandItemIndex() - 1);
                break;
            case ':':
                player.setHandItemIndex(player.getHandItemIndex() + 1);
                break;
            case 'k':
                if (stepAtack == 0) {
                    monsterSelectedAtack = 0;
                    Monstre* targetMonster = gameMap.getMonstres()[monsterSelectedAtack];
                    targetMonster->setSelected(true);
                    stepAtack = 1;
                }else {
                    Monstre* targetMonster = gameMap.getMonstres()[monsterSelectedAtack];
                    targetMonster->setSelected(true, "╘");
                    player.attack(*targetMonster, monsterSelectedAtack, player.getHandItemIndex());
                    stepAtack = 2;
                }
                break;
            case 'o':
                stepAtack = 0;
                if (monsterSelectedAtack != -1) {
                    Monstre* targetMonster = gameMap.getMonstres()[monsterSelectedAtack];
                    targetMonster->setSelected(false);
                }
                monsterSelectedAtack = -1;
                break;
            case 'l':
                if (stepAtack == 1) {
                    Monstre* targetMonster = gameMap.getMonstres()[monsterSelectedAtack];
                    targetMonster->setSelected(false);
                    monsterSelectedAtack++;
                    if (monsterSelectedAtack >= static_cast<int>(gameMap.getMonstres().size()))
                        monsterSelectedAtack = 0;
                    targetMonster = gameMap.getMonstres()[monsterSelectedAtack];
                    targetMonster->setSelected(true);
                }
                break;
            case 'j':
                if (stepAtack == 1) {
                    Monstre* targetMonster = gameMap.getMonstres()[monsterSelectedAtack];
                    targetMonster->setSelected(false);
                    monsterSelectedAtack--;
                    if (monsterSelectedAtack < 0)
                        monsterSelectedAtack = static_cast<int>(gameMap.getMonstres().size()) - 1;
                    targetMonster = gameMap.getMonstres()[monsterSelectedAtack];
                    targetMonster->setSelected(true);
                }
                break;
            default:
                break;
        }
        //calculer le fps

    }
    return 0;
}