// Includes standard C++
#include <cmath>

// Includes locaux
#include "Monster.h"
#include "../../Item/Weapon/Bow.h"
#include "../../Item/Weapon/Staff.h"
#include "../../Item/Weapon/Weapon.h"
#include "../../Map/Map.h"
#include "../../UTF8.h"
#include "../../attackConst.h"
#include "../../utilities.h"

// Constantes spécifiques au monstre
#define MONSTER_SWORD_RANGE 7
#define MONSTER_BOW_RANGE 20
#define MONSTER_STAFF_RANGE 25
#define FUITE_DISTANCE 10

// Définitions de macros
#define nya std::


Monster::Monster(Map* currentMap, const int id, int life, int mana, const nya string& name)
    : Entity(currentMap, id, life, mana), name(name), experience(0), force(10), handItem(nullptr) {
    // Initialization of monster-specific attributes
}

Monster::~Monster() {
}
// void Monster::update(Map gameMap, Player player) {
//     //fonction qui s'ocupe de deplacer le monstre et de le faire ataquer
//     //les ataque varie en fonction du type d'arme
//     //calcule la distence au joueur et se raproche ou s'eloigne en fonction
//     speed--;
//     if (speed != 0) return;
//     Vector2 lastPos = position;
//     speed = 3;
//     Vector2 diff = {
//         x : player.getPosition().x - position.x,
//         y : player.getPosition().y - position.y
//     };
//     nya cerr << "posDiff : " << diff.x << " " << diff.y << nya endl;
//     if (diff.x < 0 && diff.y < 0) this->setPosition({ position.x - 1, position.y - 1, }, gameMap);
//     if (diff.x > 0 && diff.y < 0) this->setPosition({ position.x + 1, position.y - 1, }, gameMap);
//     if (diff.x > 0 && diff.y > 0) this->setPosition({ position.x + 1, position.y + 1, }, gameMap);
//     if (diff.x < 0 && diff.y > 0) this->setPosition({ position.x - 1, position.y + 1, }, gameMap);
//     if (diff.x != 0 && diff.y == 0) this->setPosition({ position.x + (diff.x < 0 ? -1 : +1),position.y }, gameMap);
//     if (diff.x == 0 && diff.y != 0) this->setPosition({ position.x,position.y + (diff.y < 0 ? -1 : +1) }, gameMap);
//     // Check map bounds
//     if (isInZone({ {0, 0}, gameMap.getSize() }) == false) {
//         position = lastPos;
//     }
//     // Check collision with obstacles in the map
//     if (gameMap.checkCollision(position)) {
//         position = lastPos;
//     }

// }

void Monster::update(Map gameMap, Player& player) {
    // Réinitialisation du timer de déplacement
    speed--;
    if (attackReadyTime > 0) attackReadyTime--;
    if (speed != 0) return;
    speed = 3; // Vitesse de déplacement

    // Position actuelle du monstre
    Vector2 lastPos = position;

    // Calcul de la différence de position entre le monstre et le joueur
    Vector2 diff = {
        player.getPosition().x - position.x,
        player.getPosition().y - position.y
    };

    // Calcul de la distance euclidienne entre le monstre et le joueur
    float distanceToPlayer = sqrt(diff.x * diff.x + diff.y * diff.y);

    // Détermination de la portée d'attaque en fonction de l'arme
    int attackRange = MONSTER_SWORD_RANGE; // Par défaut, épée
    if (handItem != nullptr) {
        if (dynamic_cast<Bow*>(handItem)) {
            attackRange = MONSTER_BOW_RANGE;
        }
        else if (dynamic_cast<Staff*>(handItem)) {
            attackRange = MONSTER_STAFF_RANGE;
        }
    }

    // Si le joueur est à portée d'attaque
    if (distanceToPlayer <= attackRange && attackReadyTime == 0) {
        // Attaquer le joueur
        if (handItem != nullptr) {
            if (dynamic_cast<Bow*>(handItem)) {
                attack(player, ATTACK_RANGED);
            }
            else if (dynamic_cast<Staff*>(handItem)) {
                attack(player, ATTACK_MAGIC);
            }
            else {
                attack(player, ATTACK_MELEE);
            }
        }
        else {
            attack(player, ATTACK_MELEE);
        }
        attackReadyTime = 10; // Temps de recharge avant la prochaine attaque
    }
    if ((int)distanceToPlayer == attackRange) return;
    // Si le joueur est trop proche, s'éloigner
    if (distanceToPlayer - FUITE_DISTANCE < attackRange) {
        diff.x = -diff.x;
        diff.y = -diff.y;
    }

    // Normalisation du vecteur de déplacement (pour un mouvement fluide)
    float magnitude = sqrt(diff.x * diff.x + diff.y * diff.y);
    if (magnitude > 0) {
        diff.x = round(diff.x / magnitude);
        diff.y = round(diff.y / magnitude);
    }

    // Calcul de la nouvelle position
    Vector2 newPos = {
        position.x + diff.x,
        position.y + diff.y
    };

    // Vérification des limites de la carte
    position = newPos;
    if (!isInZone({ {0, 0}, gameMap.getSize() })) {
        position = lastPos;
        return;
    }

    // Vérification des collisions
    if (gameMap.checkCollision(newPos)) {
        position = lastPos;
        return;
    }
}
nya string Monster::getName() const {
    return name;
}

void Monster::attack(Entity& target, int method) {
    // Implementation of attack method
    // This would use the method parameter to determine attack typetakeDamage
    nya cerr << "Monster " << name << " attacks using method " << method << " for " << force << " damage to target : " << target.getId() << nya endl;
    target.takeDamage(force);
    nya cerr << "Target new life: " << target.getLife() << nya endl;
}

int Monster::getForce() const { 
    return force; 
}

int Monster::getExperience() const { 
    return experience; 
}

int Monster::getLevel() const {
    // Simple level calculation based on experience
    return experience / 100 + 1;
}

Item* Monster::getItem(int index) const { 
    return handItem; 
}

void Monster::setExperience(int amount) { 
    experience += amount; 
}

void Monster::setForce(int amount) { 
    force = amount; 
}

void Monster::setHandItem(Item* item) {
    handItem = item;
}

void Monster::setLife(int amount) { 
    if (amount >= 0) 
        life = amount; 
    else 
        life = 10; 
}

bool Monster::isSelected() const {
    return selected;
}

void Monster::setSelected(bool state, nya string colorCode) {
    selected = state;
    selectColorCode = colorCode;
}

RenderElements Monster::render() const  {
    RenderElements elements;
    elements.zIndex = this->getZIndex();
    elements.position = this->getPosition();
    elements.size = { 2,2 };
    if (handItem->getId() != 0) {
        const Weapon* weapon = dynamic_cast<const Weapon*>(handItem);
        if (weapon) {
            if (weapon->isWeapon() == "sword") {
                elements.content.push_back(UTF8("🗡" + monsterHead));
                elements.content.push_back(UTF8(" 🗥"));
                elements.lights.push_back(UTF8("┉┏"));
                elements.lights.push_back(UTF8(" ┋"));
                elements.localPosition = { 2, 2 }; // Set the local position for a foot of the monster
                return selected ? RenderElementsSelected(elements, selectColorCode, life, maxLife) : elements;
            } else if (weapon->isWeapon() == "bow") {
                elements.content.push_back(UTF8("{|" + monsterHead));
                elements.content.push_back(UTF8("  🗥"));
                elements.lights.push_back(UTF8("┏┏┏"));
                elements.lights.push_back(UTF8("  ┋"));
                elements.size = { 3, 2 };
                elements.localPosition = {1, 3}; // Set the local position for a foot of the monster
                return selected ? RenderElementsSelected(elements, selectColorCode, life, maxLife) : elements;
            } else if (weapon->isWeapon() == "staff") {
                elements.content.push_back(UTF8("❖"+monsterHead));
                elements.content.push_back(UTF8("|🗥"));
                elements.lights.push_back(UTF8("┎┏"));
                elements.lights.push_back(UTF8("┏┋"));
                elements.localPosition = { 2, 2 }; // Set the local position for a foot of the monster
                return selected ? RenderElementsSelected(elements, selectColorCode, life, maxLife) : elements;
            }
        }
    }
    elements.content.push_back(UTF8(monsterHead));
    elements.content.push_back(UTF8("🗥"));
    elements.lights.push_back(UTF8("┏"));
    elements.lights.push_back(UTF8("┋"));
    elements.size = { 1,2 };
    elements.localPosition = { 0, 0 }; // Set the local position for a foot of the monster
    return selected ? RenderElementsSelected(elements, selectColorCode, life, maxLife) : elements;
}
