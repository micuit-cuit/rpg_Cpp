#pragma once
#include "./Weapon.h"
#define nya std::

class Sword : public Weapon {
    public:
        Sword(int id, const nya string& name, int Strike, int damage, const nya string& lore = "", const nya string& icon = "🗡️",int mana = 0)
            : Weapon(id, name, Strike, damage, mana, lore, icon) {}
        virtual ~Sword() {}
        nya string isWeapon() const override { return "sword"; }
};

