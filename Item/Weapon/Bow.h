#pragma once
#include "./Weapon.h"

class Bow : public Weapon {
    public:
        Bow(int id, const nya string& name, int Strike, int damage, const nya string& lore = "", const nya string& icon = "🏹", int mana = 0)
            : Weapon(id, name, Strike, damage, mana, lore, icon) {this->icon = icon;}
        virtual ~Bow() {}
        nya string isWeapon() const override { return "bow"; }
        virtual nya string render() const { return "\033[38;5;97m" + lightCalculateProgressBar(reloadTime, maxReloadTime) + "\e[0m" + icon + " "; }
        void decreaseReloadTime(int amount = 1) override { reloadTime -= amount; if (reloadTime < 0) reloadTime = 0; }
private:
    nya string icon = "🏹";
    int reloadTime = 200;
    int maxReloadTime = 200;
};