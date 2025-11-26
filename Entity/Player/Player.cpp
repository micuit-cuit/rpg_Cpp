#include "Player.h"
#include "../../attackConst.h"
#include "../../Item/Weapon/Weapon.h"
#include "../../UTF8.h"
#include "../../utilitise.h"

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
    int Strike = 0;
    
    auto processWeapon = [&](Weapon* weapon, float multiplier, int damage) {
        if (weapon && !weapon->isBroken()) {
            Strike += static_cast<int>(weapon->getStrike() * multiplier);
            weapon->use(damage);
        }
    };
    
    if (method == ATTACK_MELEE) {
        Strike += force;
        // On vérifie si on a une arme à la main
        if (handItemIndex >= 0 && handItemIndex < static_cast<int>(inventory.size())) {
            Weapon* weapon = dynamic_cast<Weapon*>(inventory[handItemIndex]);
            if (weapon) {
                // On choisit le multiplicateur et les dégâts selon le type
                if (weapon->isWeapon() == "sword")      
                    processWeapon(weapon, MELEE_STRIKE_SWORD, MELEE_DAMAGE_SWORD);
                else if (weapon->isWeapon() == "bow")   
                    processWeapon(weapon, MELEE_STRIKE_BOW, MELEE_DAMAGE_BOW);
                else if (weapon->isWeapon() == "staff") 
                    processWeapon(weapon, MELEE_STRIKE_STAFF, MELEE_DAMAGE_STAFF);
            }
        }
    } else if (method == ATTACK_RANGED) {
        Strike += force / RANGED_DAMAGE_BASE; // Ranged attacks are less influenced by force
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
    target.takeDamage(Strike);
}

int Player::getForce() const { 
    return force; 
}

int Player::getExperience() const { 
    return experience; 
}

int Player::getLevel() const {
    // Simple level calculation based on experience
    return experience / 100 + 1;
}

Item Player::getItem(int index) const { 
    if (index >= 0 && index < static_cast<int>(inventory.size())) {
        return *inventory[index]; 
    }
    return Item(0, "empty"); // Return empty item if index is invalid
}

nya vector<Item> Player::getInventory() const { 
    nya vector<Item> result;
    for (const auto& item : inventory) {
        if (item) {
            result.push_back(*item);
        }
    }
    return result;
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
                elements.content.push_back(UTF8(" ° "));
                elements.content.push_back(UTF8("/|\\𐃉"));
                elements.content.push_back(UTF8("/ \\"));
                elements.lights.push_back(UTF8(" ┏  "));
                elements.lights.push_back(UTF8("┏┏┏┉"));
                elements.lights.push_back(UTF8("┏ ┏ "));
                elements.localPosition = { 2, 2 }; // Set the local position for a foot of the player
                return elements;
            } else if (weapon->isWeapon() == "bow") {
                elements.content.push_back(UTF8(" ° "));
                elements.content.push_back(UTF8("/|\\|}"));
                elements.content.push_back(UTF8("/ \\"));
                elements.lights.push_back(UTF8(" ┏  "));
                elements.lights.push_back(UTF8("┏┏┏┏┏"));
                elements.lights.push_back(UTF8("┏ ┏ "));
                elements.size = { 5,3 };
                elements.localPosition = {2, 2}; // Set the local position for a foot
                return elements;
            } else if (weapon->isWeapon() == "staff") {
                elements.content.push_back(UTF8(" ° ❖"));
                elements.content.push_back(UTF8("/|\\|"));
                elements.content.push_back(UTF8("/ \\|"));
                elements.lights.push_back(UTF8(" ┏ ┎"));
                elements.lights.push_back(UTF8("┏┏┏┏"));
                elements.lights.push_back(UTF8("┏ ┏┏"));
                elements.localPosition = { 2, 2 }; // Set the local position for a foot
                return elements;
            }
        }
    }
    elements.content.push_back(UTF8(" °"));
    elements.content.push_back(UTF8("/|\\"));
    elements.content.push_back(UTF8("/ \\"));
    elements.lights.push_back(UTF8(" ┏"));
    elements.lights.push_back(UTF8("┏┏┏"));
    elements.lights.push_back(UTF8("┏ ┏"));
    elements.size = { 3,3 };
    elements.localPosition = {2, 2}; // Set the local position for a foot
    return elements;
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
    inventoryDisplay += "\033[38;5;9m  │" + calculeteCharProgressBar(life, 100, 6, 6) + "│\033[38;5;14m │" + calculeteCharProgressBar(mana, 100, 6, 6) + "│\033[38;5;10m │" + calculeteCharProgressBar(experience % 100, 100, 6, 6) + "│\033[38;5;91m │" + calculeteCharProgressBar(force, 100, 6, 6) + "│\033[0m\n ";
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
    inventoryDisplay += "\033[38;5;9m  │" + calculeteCharProgressBar(life, 100, 6, 5) + "│\033[38;5;14m │" + calculeteCharProgressBar(mana, 100, 6, 5) + "│\033[38;5;10m │" + calculeteCharProgressBar(experience % 100, 100, 6, 5) + "│\033[38;5;91m │" + calculeteCharProgressBar(force, 100, 6, 5) + "│\033[0m\n";
    inventoryDisplay += " ┌──────────────────────────────────────────────────────────────────────────┐\033[38;5;9m  │" + calculeteCharProgressBar(life, 100, 6, 4) + "│\033[38;5;14m │" + calculeteCharProgressBar(mana, 100, 6, 4) + "│\033[38;5;10m │" + calculeteCharProgressBar(experience % 100, 100, 6, 4) + "│\033[38;5;91m │" + calculeteCharProgressBar(force, 100, 6, 4) + "│\033[0m\n";
    inventoryDisplay += " │ " + (inventory.size() > static_cast<size_t>(handItemIndex) && inventory[handItemIndex] != nullptr ? textFixedLength(inventory[handItemIndex]->getLore(), 72) : nya string(72, ' ')) + " │\033[38;5;9m  │" + calculeteCharProgressBar(life, 100, 6, 3) + "│\033[38;5;14m │" + calculeteCharProgressBar(mana, 100, 6, 3) + "│\033[38;5;10m │" + calculeteCharProgressBar(experience % 100, 100, 6, 3) + "│\033[38;5;91m │" + calculeteCharProgressBar(force, 100, 6, 3) + "│\033[0m\n";
    inventoryDisplay += " │                                                                          │\033[38;5;9m  │" + calculeteCharProgressBar(life, 100, 6, 2) + "│\033[38;5;14m │" + calculeteCharProgressBar(mana, 100, 6, 2) + "│\033[38;5;10m │" + calculeteCharProgressBar(experience % 100, 100, 6, 2) + "│\033[38;5;91m │" + calculeteCharProgressBar(force, 100, 6, 2) + "│\033[0m\n";
    inventoryDisplay += " └──────────────────────────────────────────────────────────────────────────┘\033[38;5;9m  │" + calculeteCharProgressBar(life, 100, 6, 1) + "│\033[38;5;14m │" + calculeteCharProgressBar(mana, 100, 6, 1) + "│\033[38;5;10m │" + calculeteCharProgressBar(experience % 100, 100, 6, 1) + "│\033[38;5;91m │" + calculeteCharProgressBar(force, 100, 6, 1) + "│\033[0m";
    return inventoryDisplay;
}

//     // a rendre dinamique en fonction de l'inventaire
//     return " ┌────┬────┳━━━━┳────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬────┐  100⦁ 80⦁ 0 ⦁ 10\n\
//  │ 🪵  │ 🗡️  ┃ 🗡️  ┃ 🗡️  │ 🗡️  │ 🗡️  │ 🗡️  │ 🗡️  │ 🗡️️  │ 🗡️  │ 🗡️  │ 🗡️  │ 🗡️  │ 🗡️  │ 🗡️  │  │█│⦁│ │⦁│ │⦁│ │\n\
//  └────┴────┻━━━━┻────┴────┴────┴────┴────┴────┴────┴────┴────┴────┴────┴────┘  │█│⦁│▓│⦁│ │⦁│ │\n\
//  ┌──────────────────────────────────────────────────────────────────────────┐  │█│⦁│█│⦁│ │⦁│ │\n\
//  │ box                                                                      │  │█│⦁│█│⦁│ │⦁│ │\n\
//  │                                                                          │  │█│⦁│█│⦁│ │⦁│ │\n\
//  └──────────────────────────────────────────────────────────────────────────┘  │█│⦁│█│⦁│ │⦁│▒│";
// }

/*
█ ▓ ▒ ░
| Caractère | Nom Unicode                       | Usage spécial / mixte           |
| --------- | --------------------------------- | ------------------------------- |
| ╞         | LEFT SINGLE AND RIGHT DOUBLE      | Jonction gauche mixte           |
| ╡         | RIGHT SINGLE AND LEFT DOUBLE      | Jonction droite mixte           |
| ╤         | DOWN SINGLE AND HORIZONTAL DOUBLE | Jonction haut mixte             |
| ╧         | UP SINGLE AND HORIZONTAL DOUBLE   | Jonction bas mixte              |
| ╦         | DOWN DOUBLE AND HORIZONTAL SINGLE | Jonction haut mixte (inverse)   |
| ╩         | UP DOUBLE AND HORIZONTAL SINGLE   | Jonction bas mixte (inverse)    |
| ╠         | VERTICAL DOUBLE AND RIGHT SINGLE  | Jonction gauche mixte (inverse) |
| ╣         | VERTICAL DOUBLE AND LEFT SINGLE   | Jonction droite mixte (inverse) |
| Caractère | Nom Unicode                   | Usage principal          |
| --------- | ----------------------------- | ------------------------ |
| ─         | LIGHT HORIZONTAL              | Ligne horizontale simple |
|  │         | LIGHT VERTICAL                | Ligne verticale simple   |
| ┌         | LIGHT DOWN AND RIGHT          | Coin haut gauche simple  |
| ┐         | LIGHT DOWN AND LEFT           | Coin haut droit simple   |
| └         | LIGHT UP AND RIGHT            | Coin bas gauche simple   |
| ┘         | LIGHT UP AND LEFT             | Coin bas droit simple    |
| ├         | LIGHT VERTICAL AND RIGHT      | Jonction gauche simple   |
| ┤         | LIGHT VERTICAL AND LEFT       | Jonction droite simple   |
| ┬         | LIGHT DOWN AND HORIZONTAL     | Jonction haut simple     |
| ┴         | LIGHT UP AND HORIZONTAL       | Jonction bas simple      |
| ┼         | LIGHT VERTICAL AND HORIZONTAL | Croisement simple        |
| Caractère | Nom Unicode                    | Usage principal          |
| --------- | ------------------------------ | ------------------------ |
| ═         | DOUBLE HORIZONTAL              | Ligne horizontale double |
| ║         | DOUBLE VERTICAL                | Ligne verticale double   |
| ╔         | DOUBLE DOWN AND RIGHT          | Coin haut gauche double  |
| ╗         | DOUBLE DOWN AND LEFT           | Coin haut droit double   |
| ╚         | DOUBLE UP AND RIGHT            | Coin bas gauche double   |
| ╝         | DOUBLE UP AND LEFT             | Coin bas droit double    |
| ╠         | DOUBLE VERTICAL AND RIGHT      | Jonction gauche double   |
| ╣         | DOUBLE VERTICAL AND LEFT       | Jonction droite double   |
| ╦         | DOUBLE DOWN AND HORIZONTAL     | Jonction haut double     |
| ╩         | DOUBLE UP AND HORIZONTAL       | Jonction bas double      |
| ╬         | DOUBLE VERTICAL AND HORIZONTAL | Croisement double        |
| Représentation visuelle       | Usage principal          |emoji associé         |
| --------- | ----------------------------- | ------------------------ |
| Dagger                        | Arme de mêlée légère| 🗡                     |
| buche d'arbre                 | Ressource de bois         | 🪵|
| Coffre                        | Conteneur                 | 📦                     |
| Épée                          | Arme de mêlée lourde      | 🗡                     |
| Arc                           | Arme à distance           | 🏹                     |

━ ┃ ┏ ┓ ┗ ┛ ┣ ┫ ┳ ┻ ╋

┌───────┐
│ box   │
└───────┘
╔═════════╗
║ nyabox  ║
╚═════════╝

┏━━━━┳────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬────┐
┃ 🪵  ┃ 🗡️  │ 🗡️  │ 🗡️  │ 🗡️  │ 🗡️  │ 🗡️  │ 🗡️  │ 🗡️️  │ 🗡️  │ 🗡️  │ 🗡️  │ 🗡️  │ 🗡️  │ 🗡️  │ 🗡️  │
┗━━━━┻────┴────┴────┴────┴────┴────┴────┴────┴────┴────┴────┴────┴────┴────┴────┘
┌────┬────┳━━━━┳────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬────┐
│ 🪵  │ 🗡️  ┃ 🗡️  ┃ 🗡️  │ 🗡️  │ 🗡️  │ 🗡️  │ 🗡️  │ 🗡️️  │ 🗡️  │ 🗡️  │ 🗡️  │ 🗡️  │ 🗡️  │ 🗡️  │ 🗡️  │
└────┴────┻━━━━┻────┴────┴────┴────┴────┴────┴────┴────┴────┴────┴────┴────┴────┘
┌───────────────────────────────────────────────────────────────────────────────┐
│ box                                                                           │
│                                                                               │
│                                                                               │
│                                                                               │
└───────────────────────────────────────────────────────────────────────────────┘
*/