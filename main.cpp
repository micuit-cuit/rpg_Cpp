// Includes standard C++
#include <codecvt>
#include <fstream>
#include <iostream>
#include <locale>
#include <memory>
#include <string>
#ifdef __EMSCRIPTEN__
#include <chrono>
#include <thread>
#endif

// Includes locaux - Entités
#include "Entity/Entity.h"
#include "Entity/Monster/Monster.h"
#include "Entity/Player/Player.h"

// Includes locaux - Items
#include "Item/Item.h"
#include "Item/Weapon/Bow.h"
#include "Item/Weapon/Staff.h"
#include "Item/Weapon/Sword.h"
#include "Item/Weapon/Weapon.h"

// Includes locaux - Système
#include "Map/Map.h"
#include "UTF8.h"
#include "attackConst.h"
#include "clearscreen.h"
#include "term_getchar.h"
#include "utilities.h"

// Définitions de macros
#define nya std::

// Utility functions
bool strStartsWith(const nya string& str, const nya string& prefix) {
    return str.rfind(prefix, 0) == 0;
}

nya string strKeep(const nya string& str, const nya string& table) {
    nya string result;
    for (char c : str) {
        if (table.find(c) != nya string::npos) {
            result += c;
        }
    }
    return result;
}

// Load map from file
nya vector<Layer> loadMap(nya string filePath) {
    nya ifstream file(filePath);
    if (!file.is_open()) {
        nya cerr << "Error: Unable to open file " << filePath << nya endl;
        return {};
    }
    
    bool inMap = false;
    bool inLights = false;
    int layerIndex = -1;
    nya vector<Layer> layers;
    nya string line;
    
    while (nya getline(file, line)) {
        if (strStartsWith(line, "<<light")) {
            inLights = true;
            inMap = false;
            layerIndex = nya stoi(strKeep(line, "0123456789"));
        } else if (strStartsWith(line, "<<l")) {
            inMap = true;
            inLights = false;
            layerIndex = nya stoi(strKeep(line, "0123456789"));
        } else {
            if (inMap || inLights) {
                while (static_cast<int>(layers.size()) <= layerIndex) {
                    Layer newLayer;
                    newLayer.zIndex = static_cast<int>(layers.size());
                    layers.push_back(newLayer);
                }
                if (inMap) {
                    layers[layerIndex].tiles.push_back(UTF8(line));
                } else {
                    layers[layerIndex].lights.push_back(UTF8(line));
                }
            }
        }
    }
    file.close();
    nya cout << "Map loaded with " << layers.size() << " layers." << nya endl;
    return layers;
}

// Move entity with collision detection
bool moveEntity(Entity& entity, const Vector2& newPosition, Map& gameMap) {
    Vector2 lastPosition = entity.getPosition();
    entity.setPosition(newPosition, gameMap);
    
    // Check map bounds
    if (entity.isInZone({ {0, 0}, gameMap.getSize() }) == false) {
        entity.setPosition(lastPosition, gameMap);
        return false;
    }
    
    // Check collision with obstacles in the map
    if (gameMap.checkCollision(newPosition)) {
        entity.setPosition(lastPosition, gameMap);
        return false;
    }
    
    entity.setZIndex(newPosition.y);
    return true;
}

// Load obstacles from hardcoded data
nya vector<Box> loadObstacles() {
    return {
        {{0,0}, {46,2}},
        {{0,3}, {28,3}},
        {{0,4}, {12,4}},
        {{52,3}, {57,3}},
        {{6,10}, {11,10}}
    };
}

// Initialize all game items
nya vector<Item*> initializeItems() {
    nya vector<Item*> allItemsList;
    allItemsList.push_back(new Sword(1, "Épée en Fer", 20, 150, "Une solide épée en fer."));
    allItemsList.push_back(new Bow(3, "Arc en Bois", 15, 100, "Un simple arc en bois."));
    allItemsList.push_back(new Staff(4, "Bâton de Mage", 25, 80, "Un bâton utilisé par les mages.", "🪄"));
    allItemsList.push_back(new Sword(2, "Épée Rouillée", 2, 80, "Une épée rouillée mais toujours tranchante."));
    allItemsList.push_back(new Bow(5, "Arc Cassé", 1, 50, "Un arc cassé, difficile à utiliser."));
    allItemsList.push_back(new Staff(6, "Bâton Tordu", 2, 60, "Un bâton tordu mais magique.", "🪄"));
    return allItemsList;
}

// Setup player with initial items
void setupPlayer(Player& player, Map& gameMap, nya vector<Item*>& items) {
    player.addItem(items[0]); // Iron Sword
    player.addItem(items[1]); // Wooden Bow
    player.addItem(items[2]); // Mage Staff
    player.setHandItemIndex(0);
    player.setPosition({ 12, 10 }, gameMap);
    player.setZIndex(10);
}

// Setup monsters in the game
void setupMonsters(nya vector<Monster*>& monsters, Map& gameMap, nya vector<Item*>& items) {
    monsters[0]->setPosition({ 59, 4 }, gameMap);
    monsters[0]->setHandItem(items[3]); // Rusty Sword
    monsters[0]->setZIndex(1);
    monsters[0]->setLife(100);
    
    monsters[1]->setPosition({ 70, 8 }, gameMap);
    monsters[1]->setHandItem(items[4]); // Broken Bow
    monsters[1]->setZIndex(1);
    monsters[1]->setLife(100);
    
    monsters[2]->setPosition({ 60, 9 }, gameMap);
    monsters[2]->setHandItem(items[5]); // Twisted Staff
    monsters[2]->setZIndex(1);
    monsters[2]->setLife(100);
}

// Update weapon reload times
void updateWeaponReloads(nya vector<Item*>& items, float deltaTime) {
    for (Item* item : items) {
        if (item->isWeapon() == "staff" || item->isWeapon() == "bow") {
            Weapon* weapon = dynamic_cast<Weapon*>(item);
            if (weapon) {
                weapon->decreaseReloadTime(deltaTime);
            }
        }
    }
}

// Handle attack animation steps
void handleAttackAnimation(float& attackTimer, int& attackAnimStep, int& monsterSelectedAttack, Map& gameMap, float deltaTime, Player* player) {
    if (attackAnimStep == 1) {
        attackTimer += deltaTime;
        const float blinkInterval = 1.0f / 60.0f;
        if (attackTimer >= blinkInterval) {
            attackTimer = 0;
            Monster* targetMonster = gameMap.getMonsters()[monsterSelectedAttack];
            static bool blinkState = false;
            blinkState = !blinkState;
            if (blinkState) {
                targetMonster->setSelected(false);
            } else {
                targetMonster->setSelected(true, "╘");
            }
            
            static int blinkCount = 0;
            blinkCount++;
            if (blinkCount >= 8) {
                blinkCount = 0;
                attackAnimStep = 2;
                targetMonster->setSelected(false);
                
                if (targetMonster->isAlive() == false) {
                    nya vector<Entity*> currentEntities = gameMap.getEntities();
                    nya vector<Entity*> newEntities;
                    for (Entity* entity : currentEntities) {
                        if (entity != targetMonster) {
                            newEntities.push_back(entity);
                        }
                    }
                    gameMap.setEntities(newEntities);
                    targetMonster = nullptr;
                    player->gainExperience(34);
                }
                monsterSelectedAttack = -1;
            }
        }
    }
}

// Update background animation
void updateBackground(float& backgroundTimer, int& lastBackgroundID, Map& gameMap, nya vector<Layer>& layers, float deltaTime) {
    backgroundTimer += deltaTime;
    const float backgroundInterval = 2.0f / 60.0f;
    if (backgroundTimer >= backgroundInterval) {
        backgroundTimer = 0;
        lastBackgroundID = (lastBackgroundID == 0) ? 1 : 0;
        gameMap.setBackground(layers[lastBackgroundID].tiles, layers[lastBackgroundID].lights);
    }
}

// Calculate FPS
void calculateFPS(float& fpsCounter, int fpsTab[60]) {
    for (int i = 58; i >= 0; i--) {
        fpsTab[i + 1] = fpsTab[i];
    }
    fpsTab[0] = static_cast<int>(fpsCounter);
    
    int fpsSum = 0;
    for (int i = 0; i < 60; i++) {
        fpsSum += fpsTab[i];
    }
    fpsCounter = static_cast<float>(fpsSum) / 60.0f;
}

// Handle player input
void handleInput(char ch, Player& player, Map& gameMap, int& attackAnimStep, int& monsterSelectedAttack, bool& running) {
    switch (ch) {
        case 'c':
            running = false;
            break;
        case 'z':
            moveEntity(player, { player.getPosition().x, player.getPosition().y - 1 }, gameMap);
            break;
        case 's':
            moveEntity(player, { player.getPosition().x, player.getPosition().y + 1 }, gameMap);
            break;
        case 'd':
            moveEntity(player, { player.getPosition().x + 1, player.getPosition().y }, gameMap);
            break;
        case 'q':
            moveEntity(player, { player.getPosition().x - 1, player.getPosition().y }, gameMap);
            break;
        case 'a':
            player.setHandItemIndex(player.getHandItemIndex() - 1);
            break;
        case 'e':
            player.setHandItemIndex(player.getHandItemIndex() + 1);
            break;
        case 'k':
            if (attackAnimStep <= 0) {
                if (monsterSelectedAttack >= 0) {
                    Monster* targetMonster = gameMap.getMonsters()[monsterSelectedAttack];
                    targetMonster->setSelected(false);
                }
                if (monsterSelectedAttack == -1)
                    monsterSelectedAttack = 0;
                else if (monsterSelectedAttack + 1 >= static_cast<int>(gameMap.getMonsters().size()))
                    monsterSelectedAttack = 0;
                else
                    monsterSelectedAttack++;
                Monster* targetMonster = gameMap.getMonsters()[monsterSelectedAttack];
                targetMonster->setSelected(true);
                attackAnimStep = -1;
            }
            else { // Reset attack animation
                attackAnimStep = 0;
                if (monsterSelectedAttack != -1) {
                    Monster* targetMonster = gameMap.getMonsters()[monsterSelectedAttack];
                    targetMonster->setSelected(false);
                }
                monsterSelectedAttack = -1;
            }
            break;
        case ' ': {
            if (attackAnimStep != -1) break;
            Monster* targetMonster = gameMap.getMonsters()[monsterSelectedAttack];
            int distanceToTarget = calculateDistance(player.getPosition(), targetMonster->getPosition());
            if (distanceToTarget > ATTACK_RANGE_SWORD && player.getItem(player.getHandItemIndex())->isWeapon() == "sword") {
                nya cerr << "Target too far for sword attack!" << nya endl;
                targetMonster->setSelected(false);
                attackAnimStep = 0;
                monsterSelectedAttack = -1;
            }
            else {
                int method = 0;
                if (distanceToTarget <= RANGE_MELEE)
                    method = ATTACK_MELEE;
                else if (distanceToTarget <= RANGE_RANGED)
                    method = ATTACK_RANGED;
                else
                    method = ATTACK_MAGIC;
                Item* weapon = player.getItem(player.getHandItemIndex());
                if (weapon->isWeapon() == "bow") {
                    Bow* bow = dynamic_cast<Bow*>(weapon);
                    if (bow && !bow->isReloaded()) {
                        nya cerr << "Bow is reloading!" << bow->getReloadTime() << nya endl;
                        break;
                    }
                    else if (bow && bow->isReloaded()) {
                        bow->setReloadTime(bow->getMaxReloadTime());
                    }
                }
                else if (weapon->isWeapon() == "staff") {
                    Staff* staff = dynamic_cast<Staff*>(weapon);
                    if ((staff && !staff->isReloaded() )|| player.getMana() < 55) {
                        nya cerr << "Staff is reloading!" << nya endl;
                        break;
                    }else if (staff && staff->isReloaded()) {
                        staff->setReloadTime(staff->getMaxReloadTime());
                        //consome du mana (55)
                        player.useMana(55);
                    }
                }
                targetMonster->setSelected(true, "╘");
                player.attack(*targetMonster, method, player.getHandItemIndex());
                attackAnimStep = 1;
            }
            nya cerr << "Attack initiated on monster " << targetMonster->getName() << " new life: " << targetMonster->getLife() << nya endl;
            break;
        }
        case 'o':
            attackAnimStep = 0;
            if (monsterSelectedAttack != -1) {
                Monster* targetMonster = gameMap.getMonsters()[monsterSelectedAttack];
                targetMonster->setSelected(false);
            }
            monsterSelectedAttack = -1;
            break;
        default:
            break;
    }
}

int main() {
    TermiosGuard termGuard;
    termGuard.enableRaw();
    
    // Load map and obstacles
    nya vector<Layer> layers = loadMap("assets/ground.map");
    nya vector<Box> obstacles = loadObstacles();
    
    // Initialize map
    Map gameMap({ 95, 15 }, layers[0].tiles, layers[0].lights);
    gameMap.addLayer(layers[2].tiles, layers[2].lights, 3);
    gameMap.addLayer(layers[3].tiles, layers[3].lights, 10);
    Box winBox = { {77,5}, {84,8} };

    // Set collision boxes in the map
    gameMap.setCollisionBoxes(obstacles);
    
    // Initialize items and entities
    nya vector<Item*> allItemsList = initializeItems();
    
    Player player(&gameMap, 42, 100, 80, "Hero");
    nya vector<Monster*> monsters = {
        new Monster(&gameMap, 2, 60, 10, "Orc"),
        new Monster(&gameMap, 3, 60, 10, "Goblin"),
        new Monster(&gameMap, 4, 80, 20, "Troll")
    };
    
    gameMap.addEntity(&player);
    for (auto* monster : monsters) {
        gameMap.addEntity(monster);
    }
    
    setupPlayer(player, gameMap, allItemsList);
    setupMonsters(monsters, gameMap, allItemsList);
    
    // Game state
    float fpsCounter = 0.0f;
    int lastTime = 0;
    float backgroundTimer = 0.0f;
    int lastBackgroundID = 0;
    int attackAnimStep = 0;
    float attackTimer = 0.0f;
    int monsterSelectedAttack = -1;
    int fpsTab[60] = {0};
    bool running = true;
    
    // Main game loop
    while (running) {
        lastTime = clock();

        ClearScreen();

        nya cout << gameMap.render();
        nya cout << player.renderInventory() << nya endl;

        int currentTime = clock();

        calculateFPS(fpsCounter, fpsTab);
        nya cout << "FPS: " << static_cast<int>(fpsCounter) << nya endl;

        float deltaTime = static_cast<float>(currentTime - lastTime) / CLOCKS_PER_SEC;
        fpsCounter = 1.0f / deltaTime;

        updateWeaponReloads(allItemsList, deltaTime);
        handleAttackAnimation(attackTimer, attackAnimStep, monsterSelectedAttack, gameMap, deltaTime, &player);
        updateBackground(backgroundTimer, lastBackgroundID, gameMap, layers, deltaTime);
        for (auto* monster : monsters) {
            monster->update(gameMap, player);
        }
        player.update();
#ifdef __EMSCRIPTEN__
        char ch = getch();
        nya this_thread::sleep_for(nya chrono::milliseconds(100));
#else
        char ch = getch_timeout(100);
#endif

        if (ch == -1) continue;
        handleInput(ch, player, gameMap, attackAnimStep, monsterSelectedAttack, running);
        if (!player.isAlive()) {
            nya cout << nya string(50, '\n');
            nya cout << "You have been defeated! Game Over." << nya endl;
            break;
        }
        if (player.getExperience() >= 100 && player.isInZone(winBox)) {
            nya cout << nya string(50, '\n');
            nya cout << "Congratulations! You have won the game!" << nya endl;
            break;
        }
    }

    // Cleanup
    for (auto* item : allItemsList) {
        delete item;
    }
    for (auto* monster : monsters) {
        delete monster;
    }
    
    return 0;
}
