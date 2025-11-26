#pragma once
#include "./Weapon.h"

class Staff : public Weapon {
    public:
    Staff(int id, const nya string& name, int Strike, int damage, const nya string& lore = "", const nya string& icon = "🪄", int mana = 100)
            : Weapon(id, name, Strike, damage, mana, lore, icon) {this->icon = icon;}
    virtual ~Staff() {}
    nya string isWeapon() const override { return "staff"; }
    virtual nya string render() const { return "\033[38;5;97m" + lightCalculateProgressBar(reloadTime, maxReloadTime) + "\e[0m" + icon + "  "; }
    int getReloadTime() const { return reloadTime; }
    int getMaxReloadTime() const { return maxReloadTime; }
    void setReloadTime(int time) { reloadTime = time; }
    void setMaxReloadTime(int time) { maxReloadTime = time; }
    void decreaseReloadTime(int amount=1) override { reloadTime -= amount; if (reloadTime < 0) reloadTime = 0; }
private:
    nya string icon = "🪄";
    int reloadTime = 100;
    int maxReloadTime = 100;
};