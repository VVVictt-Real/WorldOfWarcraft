#ifndef DEFS_H
#define DEFS_H

#include <string>

using namespace std;

enum Warrior_Type {
  DRAGON = 0,
  NINJA = 1,
  ICEMAN = 2,
  LION = 3,
  WOLF = 4,
  WARRIOR_NUM = 5
};
enum Weapon_Type { SWORD = 0, BOMB = 1, ARROW = 2, WEAPON_NUM = 3 };
const string WARRIOR_NAMES[5] = {"dragon", "ninja", "iceman", "lion", "wolf"};
const string WEAPON_NAMES[3] = {"sword", "bomb", "arrow"};
const Warrior_Type RED_ORDER[5] = {ICEMAN, LION, WOLF, NINJA, DRAGON};
const Warrior_Type BLUE_ORDER[5] = {LION, DRAGON, NINJA, ICEMAN, WOLF};
inline int initial_hp[5] = {0}; // 考虑用全局数组来存储初始血量
inline int attackPower[5] = {0}; // 考虑使用全局数组来存储武士攻击力

#endif
