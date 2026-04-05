#ifndef WEAPON
#define WEAPON

#include "Defs.h"

class Warrior;

class Weapon {
protected:
  Weapon_Type myType;
  Warrior *owner;
  int attackPower;

public:
  Weapon(Weapon_Type type, Warrior *owner, int ap)
      : myType(type), owner(owner), attackPower(ap) {}
  int get_ap() { return attackPower; }
  Weapon_Type getType() { return myType; }
  void change_owner(Warrior *newOwner) { owner = newOwner; }
  virtual void used() {}
  virtual bool isDamaged() = 0;
  virtual int attackOther() { return attackPower; }
  virtual int attackSelf() { return 0; }
  virtual bool compare(const Weapon *) const { return false; }
  bool operator<(const Weapon &other) const {
    if (this->myType != other.myType) {
      return this->myType < other.myType;
    }
    return this->compare(&other);
  }
  virtual ~Weapon() {}
};

class Sword : public Weapon {
private:
public:
  Sword(Warrior *owner, int ap) : Weapon(SWORD, owner, ap / 5){};
  bool isDamaged() override { return false; }
};

class Bomb : public Weapon {
private:
  bool isUsed = false;

public:
  Bomb(Warrior *owner, int ap) : Weapon(BOMB, owner, ap * 2 / 5){};
  bool isDamaged() override { return isUsed; }
  int attackSelf() override { return attackPower / 2; }
  void used() override { isUsed = true; }
};

class Arrow : public Weapon {
private:
  int durability;

public:
  Arrow(Warrior *owner, int ap)
      : Weapon(ARROW, owner, ap * 3 / 10), durability(2){};
  void used() override { durability--; }
  bool isDamaged() override { return durability <= 0; }
  int getDurability() const { return durability; }
  bool compare(const Weapon *other) const override {
    return this->durability <
           static_cast<const Arrow *>(other)->getDurability();
  }
};

#endif // !WEAPON
