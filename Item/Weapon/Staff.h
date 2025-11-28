#pragma once

// Includes locaux
#include "./Weapon.h"
#include "../../utilities.h"

// Définitions de macros
#define nya std::

class Staff : public Weapon {
    public:
    Staff(int id, const nya string& name, int Strike, int damage, const nya string& lore = "", const nya string& icon = "🪄", int mana = 100)
            : Weapon(id, name, Strike, damage, mana, lore, icon) {this->icon = icon;}
    virtual ~Staff() {}
    nya string isWeapon() const override { return "staff"; }
    virtual nya string render() const { return "\033[38;5;97m" + lightCalculateProgressBar(reloadTime, maxReloadTime) + "\e[0m" + icon + " "; }
    float getReloadTime() const { return reloadTime; }
    float getMaxReloadTime() const { return maxReloadTime; }
    bool isReloaded() const { return reloadTime <= 0; }
    void setReloadTime(float time) { reloadTime = time; }
    void setMaxReloadTime(float time) { maxReloadTime = time; }
    void decreaseReloadTime(float deltaTime) override { reloadTime -= deltaTime; if (reloadTime < 0) reloadTime = 0; }
private:
    nya string icon = "🪄";
    float reloadTime = 0.3f;
    float maxReloadTime = 0.3f;
};