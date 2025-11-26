#include "Monstre.h"
#include "../../Item/Weapon/Weapon.h"
#include "../../UTF8.h"
#include "../../utilitise.h"

#define nya std::
nya string stringMor(int number, nya string c) {
    nya string result;
    for (int i = 0; i < number; i++) {
        result += c;
    }
    return result;
}
RenderElements RenderElementsSelected(RenderElements elements, nya string selectColorCode = "┎", int life = 100, int maxLife = 100) {
    //add a red box around the elements
    RenderElements newElements;
    int width = elements.size.x + 2;
    int height = elements.size.y + 2;
    newElements.size = elements.size;
    newElements.position = elements.position;
    newElements.zIndex = elements.zIndex;
    newElements.localPosition = { elements.localPosition.x + 1, elements.localPosition.y + 1 };
    //life bar
    UTF8 lifeBar("");
    UTF8 lightBarColor("");
    for (int i = 0; i < width; i++) {
        lifeBar.appendData(calculeteCharProgressBar(life, maxLife, width, i+1, true));
        if (lifeBar.getCharAt(i) != " ") {
            lightBarColor.appendData(selectColorCode);
        } else {
            lightBarColor.appendData(" ");
        }
    }
    newElements.content.push_back(UTF8(lifeBar));
    newElements.lights.push_back(UTF8(lightBarColor));

    //top border
    newElements.content.push_back(UTF8("┌" + stringMor(width - 2, "─") + "┐"));
    newElements.lights.push_back(UTF8(selectColorCode + stringMor(width - 2, selectColorCode) + selectColorCode));
    //middle content
    if (elements.content.size() != elements.size.y || elements.lights.size() != elements.size.y) {
        std::cerr << "Erreur : elements.content ou elements.lights taille incorrecte !\n";
        return elements; // ou un RenderElements vide
    }
    for (int i = 0; i < elements.size.y; i++) {
        if (i >= elements.content.size() || i >= elements.lights.size()) {
            std::cerr << "Index out of bounds: " << i << "\n";
            break;
        }
        nya string contentLine = "│";
        nya string lightLine = selectColorCode;
        contentLine += elements.content[i].getData();
        lightLine += elements.lights[i].getData();
        contentLine += "│";
        lightLine += selectColorCode;
        newElements.content.push_back(UTF8(contentLine));
        newElements.lights.push_back(UTF8(lightLine));
    }
    //bottom border
    newElements.content.push_back(UTF8("└" + stringMor(width - 2, "─") + "┘"));
    newElements.lights.push_back(UTF8(selectColorCode + stringMor(width - 2, selectColorCode) + selectColorCode));
    return newElements;
}

Monstre::Monstre(Map* currentMap, const int id, int life, int mana, const nya string& name)
    : Entity(currentMap, id, life, mana), name(name), experience(0), force(10), handItem(nullptr) {
    // Initialisation des attributs spécifiques au monstre
}

Monstre::~Monstre() {
}

nya string Monstre::getName() const {
    return name;
}

void Monstre::attack(Entity& target, int method) {
    // Implementation of attack method
    // This would use the method parameter to determine attack type
    target.takeDamage(force);
}

int Monstre::getForce() const { 
    return force; 
}

int Monstre::getExperience() const { 
    return experience; 
}

int Monstre::getLevel() const {
    // Simple level calculation based on experience
    return experience / 100 + 1;
}

Item* Monstre::getItem(int index) const { 
    return handItem; 
}

void Monstre::setExperience(int amount) { 
    experience += amount; 
}

void Monstre::setForce(int amount) { 
    force = amount; 
}

void Monstre::setHandItem(Item* item) {
    handItem = item;
}

void Monstre::setLife(int amount) { 
    if (amount >= 0) 
        life = amount; 
    else 
        life = 10; 
}
bool Monstre::isSelected() const {
    return selected;
}
void Monstre::setSelected(bool state, nya string colorCode) {
    selected = state;
    selectColorCode = colorCode;
}

RenderElements Monstre::render() const  {
    RenderElements elements;
    elements.zIndex = this->getZIndex();
    elements.position = this->getPosition();
    elements.size = { 2,2 };
    if (handItem->getId() != 0) {
        const Weapon* weapon = dynamic_cast<const Weapon*>(handItem);
        if (weapon) {
            if (weapon->isWeapon() == "sword") {
                elements.content.push_back(UTF8("🗡" + monstreHead));
                elements.content.push_back(UTF8(" 🗥"));
                elements.lights.push_back(UTF8("┉┏"));
                elements.lights.push_back(UTF8(" ┋"));
                elements.localPosition = { 2, 2 }; // Set the local position for a foot of the monster
                return selected ? RenderElementsSelected(elements, selectColorCode, life) : elements;
            } else if (weapon->isWeapon() == "bow") {
                elements.content.push_back(UTF8("{|" + monstreHead));
                elements.content.push_back(UTF8("  🗥"));
                elements.lights.push_back(UTF8("┏┏┏"));
                elements.lights.push_back(UTF8("  ┋"));
                elements.size = { 3, 2 };
                elements.localPosition = {1, 3}; // Set the local position for a foot of the monster
                return selected ? RenderElementsSelected(elements, selectColorCode, life) : elements;;
            } else if (weapon->isWeapon() == "staff") {
                elements.content.push_back(UTF8("❖"+monstreHead));
                elements.content.push_back(UTF8("|🗥"));
                elements.lights.push_back(UTF8("┎┏"));
                elements.lights.push_back(UTF8("┏┋"));
                elements.localPosition = { 2, 2 }; // Set the local position for a foot of the monster
                return selected ? RenderElementsSelected(elements, selectColorCode, life) : elements;;
            }
        }
    }
    elements.content.push_back(UTF8(monstreHead));
    elements.content.push_back(UTF8("🗥"));
    elements.lights.push_back(UTF8("┏"));
    elements.lights.push_back(UTF8("┋"));
    elements.size = { 1,2 };
    elements.localPosition = { 0, 0 }; // Set the local position for a foot of the monster
    return selected ? RenderElementsSelected(elements, selectColorCode, life) : elements;
};
