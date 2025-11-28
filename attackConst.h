#pragma once
// Attack methods
#define ATTACK_MELEE 1
#define RANGE_MELEE 10
#define ATTACK_RANGED 2
#define RANGE_RANGED 40
#define ATTACK_MAGIC 3
#define RANGE_MAGIC 60


// Weapon usage for MELEE
#define MELEE_DAMAGE_SWORD 1
#define MELEE_STRIKE_SWORD 1// extra strike for sword in melee
#define MELEE_DAMAGE_BOW 4
#define MELEE_STRIKE_BOW 1/10 // very low strike for bow in melee
#define MELEE_DAMAGE_STAFF 0
#define MELEE_STRIKE_STAFF 1 // staff basic strike for melee
// Weapon usage for RANGED
#define RANGED_DAMAGE_BASE 2 // diviser la force par ce nombre pour les attaques à distance
#define RANGED_DAMAGE_BOW 1
#define RANGED_STRIKE_BOW 1 // basic strike for bow in ranged
// Weapon usage for MAGIC
#define MAGIC_DAMAGE_STAFF 1
// Attack ranges
#define ATTACK_RANGE_SWORD 10