#include "../Item.h"
#define nya std::
#pragma once

class Weapon : public Item {
    public:
        Weapon(int id, const nya string& name, int Strike, int damage, int mana = 0, const nya string& lore = "", const nya string& icon = "🗡️")
            : Item(id, name, mana, icon, lore), Strike(Strike), damage(damage) {this->icon = icon;}
        virtual ~Weapon() {}
        int use(int damageToWeapon = 1) { damage -= damageToWeapon; return Strike; }
        int getStrike() const { return Strike; }
        int getdamage() const { return damage; }
        bool isBroken() const { return damage <= 0; }
        virtual nya string isWeapon() const override { return "weapon"; }
        virtual nya string render() const { return " " + icon + "  "; }
        virtual void decreaseReloadTime(int amount = 1) {}
private:
        nya string icon = "🗡️";
        int Strike;
        int damage;
};
