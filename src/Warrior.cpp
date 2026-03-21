#include "Warrior.h"
#include "Defs.h"
#include "Headquarter.h"

Warrior::Warrior(int id, int hp, Headquarter *owner, Warrior_Type type)
    : id(id), hp(hp), owner(owner), myType(type) {
  owner->addWarriorNum(type);
}
int Warrior::get_warrior_num() { return owner->getWarriorNum(myType); }

Dragon::Dragon(int id, Headquarter *owner)
    : Warrior(id, initial_hp[DRAGON], owner, DRAGON) {
  weapon = static_cast<Weapon_Type>(id % WEAPON_NUM);
  morale = static_cast<double>(owner->getLifeUnit()) /
           static_cast<double>(initial_hp[DRAGON]);
}

Ninja::Ninja(int id, Headquarter *owner)
    : Warrior(id, initial_hp[NINJA], owner, NINJA) {
  weapon_1 = static_cast<Weapon_Type>(id % WEAPON_NUM);
  weapon_2 = static_cast<Weapon_Type>((id + 1) % WEAPON_NUM);
}

Iceman::Iceman(int id, Headquarter *owner)
    : Warrior(id, initial_hp[ICEMAN], owner, ICEMAN) {
  weapon = static_cast<Weapon_Type>(id % WEAPON_NUM);
}

Lion::Lion(int id, Headquarter *owner)
    : Warrior(id, initial_hp[LION], owner, LION) {
  loyalty = owner->getLifeUnit();
}
