#pragma once

// Includes standard C++
#include <string>

// Définitions de macros
#define nya std::

class Item {
    public:
        Item(int id, const nya string& name, int mana = 0, const nya string& icon = "🪵", const nya string& lore = "")
            : id(id), name(name), mana(mana), icon(icon), lore(lore) {}
        virtual ~Item() {}
        nya string getName() const { return name; }
        nya string getLore() const { return lore; }
        int getId() const { return id; }
        int getMana() const { return mana; }
        nya string getIcon() const { return icon; } // Placeholder icon, should be overridden in derived classes
        virtual nya string isWeapon() const { return "item"; }
        virtual nya string render() const { return " "+icon+"  "; }
private:
        int id = 0;
        nya string name;
        int mana;
        nya string icon;
        nya string lore;
};