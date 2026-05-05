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
  bool hasWeapons[WEAPON_NUM] = {false};
  vector<Weapon *> weapons;
  // int currentWeapon;

public:
  Warrior(int id, int hp, Headquarter *owner, Warrior_Type type);
  int get_id() { return id; }
  int get_hp() { return hp; }
  int get_ap() { return ap; }
  int get_warrior_num();
  void beHurt(int damage) { hp -= damage; }
  bool isDead() { return hp <= 0; }
  void add_hp(int delta);
  virtual void attack(Warrior *other);
  virtual void counterAttack(Warrior *other);
  bool shoot(Warrior *other);
  // void sortWeapons();
  // bool hasWeapons();
  Warrior_Type getType() { return myType; }
  // vector<Weapon *> stolenWeapon();
  virtual vector<Weapon *> beenSeizedWeapon();
  virtual void seizeWeapon(Warrior *other) {};
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
  void changeMorale(double delta) { morale += delta; }
  // void cheer();
};

class Ninja : public Warrior {
private:
public:
  Ninja(int id, Headquarter *owner);
  void counterAttack(Warrior *other) override {}
};

class Iceman : public Warrior {
private:
  bool stepTime = true;

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
  void seizeWeapon(Warrior *other) override;
};

#endif // !WARRIOR_H
