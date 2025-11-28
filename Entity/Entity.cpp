#include "Entity.h"

// Définitions de macros
#define nya std::

Entity::Entity(Map* currentMap, const int id, int life, int mana)
    : id(id), life(life), mana(mana), currentMap(currentMap), zIndex(0) {
    position.x = 0.0f;
    position.y = 0.0f;
    this->currentMap = currentMap;
    this->id = id;
    this->life = life;
    this->maxLife = life;
    this->mana = mana;
    this->position = { 0,0 };
    this->zIndex = 0;
}

Entity::~Entity() {}

// Getters
int Entity::getLife() const { 
    return life; 
}

int Entity::getMana() const { 
    return mana; 
}

int Entity::getId() const { 
    return id; 
}

Vector2 Entity::getPosition() const { 
    return position; 
}

int Entity::getZIndex() const { 
    return zIndex; 
}

// Checkers
bool Entity::isAlive() const { 
    return life > 1; 
}

bool Entity::hasMana() const { 
    return mana > 0; 
}

bool Entity::isInZone(const Box& box) const {
    return (position.x >= box.pos1.x && position.x <= box.pos2.x &&
        position.y >= box.pos1.y && position.y <= box.pos2.y);
}

// Basic actions
void Entity::takeDamage(int damage) {
    nya cerr << "Entity " << id << " takes " << damage << " damage." << nya endl;
    life -= damage;
    if (life < 0) life = 0; 
}

void Entity::useMana(int amount) { 
    mana -= amount; 
    if (mana < 0) mana = 0; 
}

void Entity::setPosition(const Vector2& newPosition, Map &gameMap) {
    position = newPosition;
    // Additional logic to update entity's position on the map can be added here
}

void Entity::setZIndex(int newZIndex) { 
    zIndex = newZIndex; 
}

void Entity::setLife(int amount) { 
    if (amount >= 0) 
        life = amount; 
    else 
        life = 10; 
}