# Projet RPG en C++

Un système de jeu de rôle (RPG) orienté objet développé en C++ avec système de rendu ASCII et gestion UTF-8.

## 🎮 CONTRÔLES

**Déplacement :**
- `z` `q` `s` `d` : Déplacement (haut, gauche, bas, droite)

**Inventaire :**
- `,` `:` : Changer de case d'inventaire (précédent/suivant)

**Combat :**
- `k` : Passer en mode combat / Changer de monstre ciblé
- `o` : Désélectionner le monstre
- ` ` (espace) : Attaquer le monstre sélectionné

**Système :**
- `c` : Quitter le jeu

## 🏗️ Structure du Projet

```
rpg/
├── main.cpp                # Programme principal du jeu
├── README.md               # Cette documentation
├── attackConst.h           # Constantes d'attaque et de combat
├── clearscreen.h           # Utilitaires de nettoyage d'écran
├── term_getchar.h          # Gestion des entrées clavier non-bloquantes
├── utilities.h/cpp         # Fonctions utilitaires (rendu, math)
├── UTF8.h/cpp              # Gestion des chaînes de caractères UTF-8
├── Entity/                 # Système d'entités
│   ├── Entity.h/cpp        # Classe de base Entity
│   ├── lootTable.h         # Système de butin
│   ├── Player/             # Classes du joueur
│   │   ├── Player.h
│   │   └── Player.cpp
│   └── Monster/            # Classes des monstres
│       ├── Monster.h
│       └── Monster.cpp
├── Item/                   # Système d'objets
│   ├── Item.h              # Classe de base Item
│   └── Weapon/             # Système d'armes
│       ├── Weapon.h        # Classe de base des armes
│       ├── Sword.h         # Épées
│       ├── Bow.h           # Arcs
│       └── Staff.h         # Bâtons magiques
├── Map/                    # Système de carte
│   ├── Map.h
│   └── Map.cpp
└── assets/                 # Ressources du jeu
    └── ground.map          # Fichiers de carte
```

## ⚙️ Fonctionnalités

### 🎨 Système de Rendu
- **Rendu ASCII avancé** avec support UTF-8
- **Système de couches (layers)** pour l'affichage
- **Éclairage et couleurs** avec codes ANSI
- **Animations** : arrière-plan animé, effets d'attaque
- **Interface utilisateur** : inventaire graphique, barres de progression

### 🗡️ Système d'Objets
- **Item** : Classe de base pour tous les objets
- **Weapon** : Armes avec durabilité, force d'attaque et temps de rechargement
- **Sword** : Épées pour combat de mêlée
- **Bow** : Arcs pour combat à distance avec système de rechargement
- **Staff** : Bâtons magiques pour sorts avec système de mana

### 👥 Système d'Entités
- **Entity** : Classe de base avec vie, mana, position, z-index
- **Player** : Joueur avec inventaire, expérience, niveaux, sélection d'arme
- **Monster** : Monstres avec IA, déplacement automatique, attaque intelligente
- **Polymorphisme** : takeDamage() virtuel pour comportements spécialisés

### 🗺️ Système de Carte
- **Map** : Gestion des couches, entités, collisions
- **Chargement depuis fichier** : Format de carte personnalisé
- **Détection de collision** : Obstacles et limites de carte
- **Rendu multi-couches** : Arrière-plan, objets, entités, éclairage

### ⚔️ Système de Combat
- **3 types d'attaque** : Mêlée, Distance, Magie
- **Portées d'attaque** : Différentes selon le type d'arme
- **IA des monstres** : Se rapprochent/éloignent selon leur arme
- **Animations d'attaque** : Effets visuels lors des combats
- **Calculs de dégâts** : Basés sur la force, l'arme et le type d'attaque

## 🚀 Compilation et Exécution

### Prérequis
- terminal compatible UTF-8 et avec support des codes ANSI 256 couleurs (tester sur gnome-terminal. les emojis peuvent casser l'inventaire sur certains terminaux, du au largeur variable de ces caractères)
- compilateur C++ (g++ recommandé)
### Compilation manuelle
utiliser -Ofast pour desactiver les sécurités de debug, et optimiser les performances, nécessaire pour atteindre 60 FPS avec le rendu ascii dans le terminal. sinon le jeu sera très lent.
```bash
g++ -Wall -Wextra -std=c++17 main.cpp UTF8.cpp utilities.cpp \
    Entity/Entity.cpp Entity/Monster/Monster.cpp Entity/Player/Player.cpp \
    Map/Map.cpp -o rpg_game -lncurses -Ofast
./rpg_game
```

## 🎯 Fonctionnalités du Système

### 📊 Calculs de Combat
- **Attaque Mêlée** : Force + bonus d'arme (épées optimisées)
- **Attaque Distance** : Force réduite + bonus spécialisé (arcs)
- **Attaque Magie** : Utilise le mana et les bâtons magiques

### 📈 Progression du Joueur
- **Système d'expérience** : 100 XP par niveau (1 niveau actuellement)
- **Inventaire graphique** : 15 emplacements avec interface visuelle
- **Barres de progression** : Vie, mana, expérience, force
- **Système de sélection d'arme** : Changement dynamique

### 🔧 Durabilité des Équipements
- **Dégradation** : Les armes se cassent à l'usage (non implémenté)
- **Temps de rechargement** : Arcs et bâtons ont des cooldowns 
- **Vérification automatique** : Gestion des armes cassées (non implémenté)

### 🤖 Intelligence Artificielle
- **Pathfinding simple** : Les monstres se déplacent vers le joueur
- **Gestion des portées** : Se rapprochent/éloignent selon leur arme
- **Attaque automatique** : Quand le joueur est à portée
- **Cooldowns d'attaque** : Empêchent le spam d'attaques

## 🔧 Architecture Technique

### 📐 Programmation Orientée Objet
- **Héritage C++** : Hiérarchie claire Entity → Player/Monster
- **Polymorphisme virtuel** : takeDamage() redéfinie dans Player
- **Encapsulation** : Attributs privés avec getters/setters appropriés
- **Composition** : Items dans inventaires, Map contient Entities

### 🗂️ Organisation du Code
- **Headers guards** : `#pragma once` partout
- **Séparation claire** : Headers (.h) et implémentations (.cpp)
- **Includes organisés** : Standard, système, locaux séparés
- **Constantes centralisées** : attackConst.h pour les valeurs de jeu

### 🎮 Gestion du Jeu
- **Boucle principale** : 60 FPS avec delta time (deltaTime n'est pas encore utilisé partout, certaines parties du code restent en frame-rate dépendant)
- **Gestion d'état** : Animations, sélections, combat
- **Input non-bloquant** : Jeu fluide avec term_getchar
- **Cross-platform** : Support Emscripten pour le web (pas encore fonctionnel)

## 🚧 Extensions Possibles

- **Système de sorts avancé** : Sorts multiples, effets spéciaux
- **IA sophistiquée** : Algorithmes de pathfinding complexes
- **Sauvegarde/chargement** : Persistance des parties
- **Multijoueur** : Support réseau (bon, petit ambitieux pour l'instant)
- **Sons** : Effets sonores et musique

## 📝 Notes Techniques

- **Performance** : -Ofast nécessaire pour 60 FPS
- **Encodage** : Support UTF-8 complet pour caractères spéciaux (bibliothèque UTF8.cpp custom)
- **Mémoire** : Gestion RAII, destructeurs virtuels
- **Compatibilité** : C++17, ncurses, terminal moderne requis
- **Debug** : Compilation avec warnings activés (-Wall -Wextra)
- **Nya Namespace** : Utilisation de `nya` comme alias pour `std::` pour LE FUN