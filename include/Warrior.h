#ifndef WARRIOR_H
#define WARRIOR_H

#include "Defs.h"
#include <vector>

class Headquarter;
class Weapon;

class Warrior {
protected:
  int id;
  int hp, ap;
  Headquarter *owner;
  Warrior_Type myType;
  vector<Weapon *> weapons;
  int currentWeapon;

public:
  Warrior(int id, int hp, Headquarter *owner, Warrior_Type type);
  int get_id() { return id; }
  int get_hp() { return hp; }
  int get_ap() { return ap; }
  int get_warrior_num();
  void beHurt(int damage) { hp -= damage; }
  bool isDead() { return hp <= 0; }
  void attack(Warrior *other);
  void sortWeapons();
  bool hasWeapons();
  Warrior_Type getType() { return myType; }
  vector<Weapon *> stolenWeapon();
  vector<Weapon *> beenSeizedWeapon();
  void seizeWeapon(Warrior *other);
  string getColor();
  void report();
  virtual ~Warrior();
};

class Dragon : public Warrior {
private:
  double morale;

public:
  Dragon(int id, Headquarter *owner);
  double getMorale() { return morale; }
};

class Ninja : public Warrior {
private:
public:
  Ninja(int id, Headquarter *owner);
};

class Iceman : public Warrior {
private:
public:
  Iceman(int id, Headquarter *owner);
  void moveStep();
};

class Lion : public Warrior {
private:
  int loyalty;
  inline static int loseEachStep = 0;

public:
  Lion(int id, Headquarter *owner);
  int getLoyalty() { return loyalty; }
  void loseLoyalty() { loyalty -= loseEachStep; }
  static void writeLoseEachStep(int K) { loseEachStep = K; }
};

class Wolf : public Warrior {
private:
public:
  Wolf(int id, Headquarter *owner)
      : Warrior(id, initial_hp[WOLF], owner, WOLF) {
    ap = attackPower[WOLF];
  }
  void getWeapon(Warrior *other, int city_id);
};

#endif // !WARRIOR_H
