#include "Player.h"

// Includes locaux
#include "../../Item/Weapon/Weapon.h"
#include "../../UTF8.h"
#include "../../attackConst.h"
#include "../../utilities.h"

// Définitions de macros
#define nya std::


Player::Player(Map* currentMap, const int id, int life, int mana, const nya string& name)
    : Entity(currentMap, id, life, mana), name(name), experience(0), force(10), handItemIndex(-1) {
}

Player::~Player() {
}

nya string Player::getName() const { 
    return name; 
}

void Player::attack(Entity& target, int method, int itemIndex) {
    int strike = 0;
    
    auto processWeapon = [&](Weapon* weapon, float multiplier, int damage) {
        if (weapon && !weapon->isBroken()) {
            strike += static_cast<int>(weapon->getStrike() * multiplier);
            weapon->use(damage);
        }
    };
    
    if (method == ATTACK_MELEE) {
        strike += force;
        // Check if we have a weapon in hand
        if (handItemIndex >= 0 && handItemIndex < static_cast<int>(inventory.size())) {
            Weapon* weapon = dynamic_cast<Weapon*>(inventory[handItemIndex]);
            if (weapon) {
                // Choose multiplier and damage according to type
                if (weapon->isWeapon() == "sword")      
                    processWeapon(weapon, MELEE_STRIKE_SWORD, MELEE_DAMAGE_SWORD);
                else if (weapon->isWeapon() == "bow")   
                    processWeapon(weapon, MELEE_STRIKE_BOW, MELEE_DAMAGE_BOW);
                else if (weapon->isWeapon() == "staff") 
                    processWeapon(weapon, MELEE_STRIKE_STAFF, MELEE_DAMAGE_STAFF);
            }
        }
    } else if (method == ATTACK_RANGED) {
        strike += force / RANGED_DAMAGE_BASE; // Ranged attacks are less influenced by force
        if (itemIndex >= 0 && itemIndex < static_cast<int>(inventory.size())) {
            Weapon* weapon = dynamic_cast<Weapon*>(inventory[itemIndex]);
            if (weapon && weapon->isWeapon() == "bow") {
                processWeapon(weapon, RANGED_STRIKE_BOW, RANGED_DAMAGE_BOW);
            }
        }
    } else if (method == ATTACK_MAGIC) {
        if (itemIndex >= 0 && itemIndex < static_cast<int>(inventory.size())) {
            Weapon* weapon = dynamic_cast<Weapon*>(inventory[itemIndex]);
            if (weapon && weapon->isWeapon() == "staff") {
                processWeapon(weapon, 1, MAGIC_DAMAGE_STAFF);
            }
        }
    }
    target.takeDamage(strike);
}

int Player::getForce() const { 
    return force; 
}
bool Player::isSelected() const {
    return selected;
}
void Player::setSelected(bool state, nya string colorCode) {
    selected = state;
    selectColorCode = colorCode;
}
int Player::getExperience() const {
    return experience; 
}

int Player::getLevel() const {
    // Simple level calculation based on experience
    return experience / 100 + 1;
}

Item *Player::getItem(int index) const { 
    if (index >= 0 && index < static_cast<int>(inventory.size())) {
        return inventory[index]; 
    }
    return nullptr; // Return nullptr if index is invalid
}

nya vector<Item*> Player::getInventory() const { 
    nya vector<Item*> result;
    for (const auto& item : inventory) {
        if (item) {
            result.push_back(item);
        }
    }
    return result;
}
void Player::update() {
    // fais augmenter le mana au max
    if (mana < 100) mana++;
    if (life < 100) life++;
    // Placeholder for any player-specific updates
    if (animStep >= 0) animStep--;
    if (animStep == 0) selected = false;
}
void Player::takeDamage(int damage) {
    nya cerr << "Player " << name << " takes " << damage << " damage." << nya endl; 
    life -= damage;
    if (life < 0) life = 0;
    animStep = 6;
    selected = true;
    selectColorCode = "╘";
}
void Player::gainExperience(int amount) {
    experience += amount; 
}

void Player::increaseForce(int amount) { 
    force += amount; 
}

void Player::addItem(Item* item) { 
    inventory.push_back(item); 
}

void Player::setHandItemIndex(int index) {
    if (index < 0)
        handItemIndex = 14;
    else
        handItemIndex = index % 15;
}

void Player::heal(int amount) {
    life += amount; 
}

RenderElements Player::render() const {
    RenderElements elements;
    elements.zIndex = this->getZIndex();
    elements.position = this->getPosition();
    elements.size = { 4,3 };
    if (handItemIndex >= 0 && handItemIndex < static_cast<int>(inventory.size())) {
        const Weapon* weapon = dynamic_cast<const Weapon*>(inventory[handItemIndex]);
        if (weapon) {
            if (weapon->isWeapon() == "sword") {
                elements.content.push_back(UTF8(" °  "));
                elements.content.push_back(UTF8("/|\\𐃉"));
                elements.content.push_back(UTF8("/ \\ "));
                elements.lights.push_back(UTF8(" ┏  "));
                elements.lights.push_back(UTF8("┏┏┏┉"));
                elements.lights.push_back(UTF8("┏ ┏ "));
                elements.localPosition = { 2, 2 }; // Set the local position for a foot of the player
                    return selected ? RenderElementsSelected(elements, selectColorCode, life, maxLife, false) : elements;

            } else if (weapon->isWeapon() == "bow") {
                elements.content.push_back(UTF8(" °   "));
                elements.content.push_back(UTF8("/|\\|}"));
                elements.content.push_back(UTF8("/ \\  "));
                elements.lights.push_back(UTF8(" ┏   "));
                elements.lights.push_back(UTF8("┏┏┏┏┏"));
                elements.lights.push_back(UTF8("┏ ┏  "));
                elements.size = { 5,3 };
                elements.localPosition = {2, 2}; // Set the local position for a foot
                    return selected ? RenderElementsSelected(elements, selectColorCode, life, maxLife, false) : elements;

            } else if (weapon->isWeapon() == "staff") {
                elements.content.push_back(UTF8(" ° ❖"));
                elements.content.push_back(UTF8("/|\\|"));
                elements.content.push_back(UTF8("/ \\|"));
                elements.lights.push_back(UTF8(" ┏ ┎"));
                elements.lights.push_back(UTF8("┏┏┏┏"));
                elements.lights.push_back(UTF8("┏ ┏┏"));
                elements.localPosition = { 2, 2 }; // Set the local position for a foot
                    return selected ? RenderElementsSelected(elements, selectColorCode, life, maxLife, false) : elements;

            }
        }
    }
    elements.content.push_back(UTF8(" ° "));
    elements.content.push_back(UTF8("/|\\"));
    elements.content.push_back(UTF8("/ \\"));
    elements.lights.push_back(UTF8(" ┏ "));
    elements.lights.push_back(UTF8("┏┏┏"));
    elements.lights.push_back(UTF8("┏ ┏"));
    elements.size = { 3,3 };
    elements.localPosition = {2, 2}; // Set the local position for a foot
        return selected ? RenderElementsSelected(elements, selectColorCode, life, maxLife, false) : elements;

}
nya string Player::renderInventory() const {
    nya string inventoryDisplay = " ";
    for (int i = 0; i < 15; ++i) {
        if (i == handItemIndex) {
            if (i == 0) {
                inventoryDisplay += "┏━━━━┳";
            }
            else if (i == 14) {
                inventoryDisplay += "┳━━━━┓";
            }
            else {
                inventoryDisplay += "┳━━━━┳";
            }
        } else {
            if (i == 0) {
                inventoryDisplay += "┌────";
            }else if (i == 14) {
                
                inventoryDisplay += "────┐";
            }
            else {
                inventoryDisplay += "────";
            }
            if (i + 1 != handItemIndex && i != 14) 
                inventoryDisplay += "┬";

        }
    }
    inventoryDisplay += "\033[38;5;9m  " + toFixedNumber(life,3) + " \033[38;5;14m" + toFixedNumber(mana,3) + " \033[38;5;10m" + toFixedNumber(experience,3 % 100) + "\033[38;5;91m " + toFixedNumber(force,3) + "\033[0m\n ";
    for (int i = 0; i < 15; ++i) {
        if (i == 0) {
            if (i == handItemIndex) {
                inventoryDisplay += "┃";
            }
            else {
                inventoryDisplay += "│";
            }
        }
        if (inventory.size() > static_cast<size_t>(i) && inventory[i] != nullptr) {
            inventoryDisplay += inventory[i]->render();
        } else {
            inventoryDisplay += "    ";
        }
        if (i == handItemIndex) {
            inventoryDisplay += "┃";
        }
        else if (i + 1 != handItemIndex) {
            inventoryDisplay += "│";
        }
        else 
            inventoryDisplay += "┃";

    }
    inventoryDisplay += "\033[38;5;9m  │" + calculateCharProgressBar(life, 100, 6, 6) + "│\033[38;5;14m │" + calculateCharProgressBar(mana, 100, 6, 6) + "│\033[38;5;10m │" + calculateCharProgressBar(experience, 100, 6, 6) + "│\033[38;5;91m │" + calculateCharProgressBar(force, 100, 6, 6) + "│\033[0m\n ";
    for (int i = 0; i < 15; ++i) {
        if (i == handItemIndex) {
            if (i == 0) {
                inventoryDisplay += "┗━━━━┻";
            }
            else if (i == 14) {
                inventoryDisplay += "┻━━━━┛";
            }
            else {
                inventoryDisplay += "┻━━━━┻";
            }
        } else {
            if (i == 0) {
                inventoryDisplay += "└────";
            }else if (i == 14) {
                inventoryDisplay += "────┘";
            }
            else {
                inventoryDisplay += "────";
            }
            if (i + 1 != handItemIndex && i != 14)
                inventoryDisplay += "┴";
        }
    }
    inventoryDisplay += "\033[38;5;9m  │" + calculateCharProgressBar(life, 100, 6, 5) + "│\033[38;5;14m │" + calculateCharProgressBar(mana, 100, 6, 5) + "│\033[38;5;10m │" + calculateCharProgressBar(experience, 100, 6, 5) + "│\033[38;5;91m │" + calculateCharProgressBar(force, 100, 6, 5) + "│\033[0m\n";
    inventoryDisplay += " ┌──────────────────────────────────────────────────────────────────────────┐\033[38;5;9m  │" + calculateCharProgressBar(life, 100, 6, 4) + "│\033[38;5;14m │" + calculateCharProgressBar(mana, 100, 6, 4) + "│\033[38;5;10m │" + calculateCharProgressBar(experience, 100, 6, 4) + "│\033[38;5;91m │" + calculateCharProgressBar(force, 100, 6, 4) + "│\033[0m\n";
    inventoryDisplay += " │ " + (inventory.size() > static_cast<size_t>(handItemIndex) && inventory[handItemIndex] != nullptr ? textFixedLength(inventory[handItemIndex]->getLore(), 72) : nya string(72, ' ')) + " │\033[38;5;9m  │" + calculateCharProgressBar(life, 100, 6, 3) + "│\033[38;5;14m │" + calculateCharProgressBar(mana, 100, 6, 3) + "│\033[38;5;10m │" + calculateCharProgressBar(experience, 100, 6, 3) + "│\033[38;5;91m │" + calculateCharProgressBar(force, 100, 6, 3) + "│\033[0m\n";
    inventoryDisplay += " │                                                                          │\033[38;5;9m  │" + calculateCharProgressBar(life, 100, 6, 2) + "│\033[38;5;14m │" + calculateCharProgressBar(mana, 100, 6, 2) + "│\033[38;5;10m │" + calculateCharProgressBar(experience, 100, 6, 2) + "│\033[38;5;91m │" + calculateCharProgressBar(force, 100, 6, 2) + "│\033[0m\n";
    inventoryDisplay += " └──────────────────────────────────────────────────────────────────────────┘\033[38;5;9m  │" + calculateCharProgressBar(life, 100, 6, 1) + "│\033[38;5;14m │" + calculateCharProgressBar(mana, 100, 6, 1) + "│\033[38;5;10m │" + calculateCharProgressBar(experience, 100, 6, 1) + "│\033[38;5;91m │" + calculateCharProgressBar(force, 100, 6, 1) + "│\033[0m";
    return inventoryDisplay;
}
