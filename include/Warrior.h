#ifndef WARRIOR_H
#define WARRIOR_H

#include "Defs.h"

class Headquarter;

class Warrior {
private:
  int id;
  int hp, ap;
  Headquarter *owner;
  Warrior_Type myType;

public:
  Warrior(int id, int hp, Headquarter *owner, Warrior_Type type);
  int get_id() { return id; }
  int get_hp() { return hp; }
  int get_warrior_num();
  void write_hp(int new_hp) { hp = new_hp; }
};

class Dragon : public Warrior {
private:
  Weapon_Type weapon;
  double morale;

public:
  Dragon(int id, Headquarter *owner);
  Weapon_Type getWeapon() { return weapon; }
  double getMorale() { return morale; }
};

class Ninja : public Warrior {
private:
  Weapon_Type weapon_1, weapon_2;

public:
  Ninja(int id, Headquarter *owner);
  Weapon_Type getWeapon1() { return weapon_1; }
  Weapon_Type getWeapon2() { return weapon_2; }
};

class Iceman : public Warrior {
private:
  Weapon_Type weapon;

public:
  Iceman(int id, Headquarter *owner);
  Weapon_Type getWeapon() { return weapon; }
};

class Lion : public Warrior {
private:
  int loyalty;

public:
  Lion(int id, Headquarter *owner);
  int getLoyalty() { return loyalty; }
};

class Wolf : public Warrior {
private:
public:
  Wolf(int id, Headquarter *owner)
      : Warrior(id, initial_hp[WOLF], owner, WOLF) {}
};

#endif // !WARRIOR_H
