# Projet RPG en C++

Un système de jeu de rôle (RPG) orienté objet développé en C++.

## CONTROLE
"z" "q" "s" "d": deplacement
"," ":" : changer de casse d'inventaire
";" : utiliser l'objet en main
"l" : selectionner monstre / attaquer
"o" : desélectionner monstre
"k" "m" : switch de monstre cible

## 🏗️ Structure du Projet

```
rpg/
├── main.cpp              # Programme de test principal
├── Makefile             # Fichier de compilation
├── README.md            # Cette documentation
├── math.h               # Structures mathématiques (Vector2)
├── attackConst.h        # Constantes d'attaque et de combat
├── entity/              # Système d'entités
│   ├── Entity.h/cpp     # Classe de base Entity
│   ├── lootTable.h      # Système de butin
│   ├── Player/          # Classes du joueur
│   │   ├── Player.h
│   │   └── Player.cpp
│   ├── Monstre/         # Classes des monstres
│   │   ├── Monstre.h
│   │   └── Monstre.cpp
│   └── TileEntity/      # Entités de tuiles
│       └── TileEntity.h
└── Item/                # Système d'objets
    ├── Item.h           # Classe de base Item
    └── Weapon/          # Système d'armes
        ├── Weapon.h
        └── Sword/
            └── Sword.h
```

## ⚙️ Fonctionnalités

### 🗡️ Système d'Objets
- **Item** : Classe de base pour tous les objets
- **Weapon** : Armes avec durabilité et force d'attaque
- **Sword** : Épées spécialisées héritant de Weapon

### 👥 Système d'Entités
- **Entity** : Classe de base avec vie, mana, position
- **Player** : Joueur avec inventaire, expérience, niveaux
- **Monstre** : Monstres avec IA et table de butin
- **TileEntity** : Entités statiques liées aux tuiles

### ⚔️ Système de Combat
- **3 types d'attaque** : Mêlée, Distance, Magie
- **Multiplicateurs d'armes** selon le type d'attaque
- **Durabilité des armes** qui se dégradent à l'usage
- **Calculs de dégâts** basés sur la force et l'équipement

## 🚀 Compilation et Exécution

### Prérequis
- Compilateur C++11 ou plus récent (g++, clang++)
- Make (optionnel, pour utiliser le Makefile)

### Compilation avec Make
```bash
# Compilation simple
make

# Compilation et exécution
make run

# Compilation en mode debug
make debug

# Compilation optimisée
make release

# Nettoyage
make clean

# Recompilation complète
make rebuild

# Aide
make help
```

### Compilation manuelle
```bash
g++ -std=c++11 -Wall -Wextra -I. main.cpp entity/Entity.cpp entity/Player/Player.cpp entity/Monstre/Monstre.cpp -o test_rpg
./test_rpg
```

## 🧪 Tests Inclus

Le fichier `main.cpp` contient une suite de tests complète :

1. **Test des Items** : Création et utilisation d'objets et d'armes
2. **Test des Entités** : Fonctionnalités de base des entités
3. **Test du Joueur** : Inventaire, expérience, soins
4. **Test des Monstres** : Équipement et progression
5. **Test de Combat** : Combat entre joueur et monstre
6. **Test des Types d'Attaque** : Mêlée, distance, magie

## 📊 Exemple de Sortie

```
=== TEST DU SYSTÈME RPG ===

=== Test des Items ===
Item créé: Potion de soin (ID: 1, Mana: 50)
Type d'objet: item

Arme créée: Arc en bois (Force: 15, Durabilité: 100)
Type d'arme: weapon
Arme cassée? Non

=== Test de Combat ===
=== Combat entre Héros et Orc ===
Héros attaque en mêlée!
Orc contre-attaque!

=== TOUS LES TESTS TERMINÉS AVEC SUCCÈS! ===
```

## 🎯 Fonctionnalités du Système

### Calculs de Combat
- **Attaque Mêlée** : Force + bonus d'arme
- **Attaque Distance** : Force réduite + bonus spécialisé
- **Attaque Magie** : Utilise le mana et les bâtons magiques

### Progression du Joueur
- **Système d'expérience** : 100 XP par niveau
- **Inventaire dynamique** : Stockage illimité d'objets
- **Soins et régénération** : Mécanismes de récupération

### Durabilité des Équipements
- Les armes se dégradent à l'usage
- Vérification automatique de l'état des armes
- Système d'armes cassées

## 🔧 Architecture Technique

- **Héritage C++** : Hiérarchie claire des classes
- **Polymorphisme** : Méthodes virtuelles pour les spécialisations
- **Encapsulation** : Attributs privés avec getters/setters
- **Headers guards** : Protection contre les inclusions multiples
- **Séparation interface/implémentation** : Headers (.h) et sources (.cpp)

## 🚧 Extensions Possibles

- Système de sorts et magie avancée
- IA plus sophistiquée pour les monstres
- Système de carte et de déplacement
- Sauvegarde/chargement de parties
- Interface graphique
- Multijoueur en réseau

## 📝 Notes de Développement

- Code compilé et testé avec g++ et C++11
- Warnings résolus pour les paramètres non utilisés
- Gestion mémoire sécurisée avec destructeurs virtuels
- Structure modulaire permettant l'extension facile