#include "Warrior.h"
#include "Clock.h"
#include "Defs.h"
#include "Headquarter.h"
#include "Weapon.h"
#include <algorithm>
#include <cstdio>
#include <vector>

Warrior::Warrior(int id, int hp, Headquarter *owner, Warrior_Type type)
    : id(id), hp(hp), owner(owner), myType(type) {
  owner->addWarriorNum(type);
}
int Warrior::get_warrior_num() { return owner->getWarriorNum(myType); }

Dragon::Dragon(int id, Headquarter *owner)
    : Warrior(id, initial_hp[DRAGON], owner, DRAGON) {
  ap = attackPower[DRAGON];
  Weapon *initialWeapon = nullptr;
  switch (id % WEAPON_NUM) {
  case SWORD:
    initialWeapon = new Sword(this, ap);
    break;
  case BOMB:
    initialWeapon = new Bomb(this, ap);
    break;
  case ARROW:
    initialWeapon = new Arrow(this, ap);
    break;
  }
  weapons.push_back(initialWeapon);
  currentWeapon = 0;
  morale = static_cast<double>(owner->getLifeUnit()) /
           static_cast<double>(initial_hp[DRAGON]);
}

Ninja::Ninja(int id, Headquarter *owner)
    : Warrior(id, initial_hp[NINJA], owner, NINJA) {
  ap = attackPower[NINJA];
  Weapon *firstWeapon = nullptr;
  Weapon *secondWeapon = nullptr;
  switch (id % WEAPON_NUM) {
  case SWORD:
    firstWeapon = new Sword(this, ap);
    secondWeapon = new Bomb(this, ap);
    break;
  case BOMB:
    firstWeapon = new Bomb(this, ap);
    secondWeapon = new Arrow(this, ap);
    break;
  case ARROW:
    firstWeapon = new Arrow(this, ap);
    secondWeapon = new Sword(this, ap);
    break;
  }
  weapons.push_back(firstWeapon);
  weapons.push_back(secondWeapon);
  currentWeapon = 0;
}

Iceman::Iceman(int id, Headquarter *owner)
    : Warrior(id, initial_hp[ICEMAN], owner, ICEMAN) {
  ap = attackPower[ICEMAN];
  Weapon *initialWeapon = nullptr;
  switch (id % WEAPON_NUM) {
  case SWORD:
    initialWeapon = new Sword(this, ap);
    break;
  case BOMB:
    initialWeapon = new Bomb(this, ap);
    break;
  case ARROW:
    initialWeapon = new Arrow(this, ap);
    break;
  }
  weapons.push_back(initialWeapon);
  currentWeapon = 0;
}

void Iceman::moveStep() { hp -= hp / 10; }

Lion::Lion(int id, Headquarter *owner)
    : Warrior(id, initial_hp[LION], owner, LION) {
  ap = attackPower[LION];
  Weapon *initialWeapon = nullptr;
  switch (id % WEAPON_NUM) {
  case SWORD:
    initialWeapon = new Sword(this, ap);
    break;
  case BOMB:
    initialWeapon = new Bomb(this, ap);
    break;
  case ARROW:
    initialWeapon = new Arrow(this, ap);
    break;
  }
  weapons.push_back(initialWeapon);
  currentWeapon = 0;
  loyalty = owner->getLifeUnit();
}

void Warrior::sortWeapons() {
  std::sort(weapons.begin(), weapons.end(),
            [](Weapon *a, Weapon *b) { return (*a) < (*b); });
  currentWeapon = 0;
}

string Warrior::getColor() { return owner->getName(); }

void Warrior::attack(Warrior *other) {
  if (weapons.empty())
    return;
  other->beHurt(weapons[currentWeapon]->attackOther());
  if (myType == NINJA && weapons[currentWeapon]->getType() == BOMB) {
  } else
    this->beHurt(weapons[currentWeapon]->attackSelf());
  weapons[currentWeapon]->used();
  if (weapons[currentWeapon]->isDamaged()) {
    delete weapons[currentWeapon];
    weapons.erase(weapons.begin() + currentWeapon);
  } else {
    currentWeapon++;
  }
  if (!weapons.empty()) {
    currentWeapon = currentWeapon % weapons.size();
  }
}
bool Warrior::hasWeapons() {
  if (weapons.empty())
    return false;
  for (auto weapon : weapons)
    if (weapon->getType() == BOMB || weapon->getType() == ARROW ||
        weapon->attackOther() || weapon->attackSelf())
      return true;
  return false;
}

vector<Weapon *> Warrior::stolenWeapon() {
  vector<Weapon *> stolen;
  if (weapons.empty()) {
    return stolen;
  }
  sortWeapons();
  Weapon_Type type = weapons[0]->getType();
  for (auto it = weapons.begin(); it != weapons.end();) {
    if ((*it)->getType() == type) {
      stolen.push_back(*it);
      it = weapons.erase(it);
    } else {
      ++it;
    }
  }
  return stolen;
}

vector<Weapon *> Warrior::beenSeizedWeapon() {
  sortWeapons();
  vector<Weapon *> tmp = weapons;
  weapons.clear();
  return tmp;
}
void Warrior::seizeWeapon(Warrior *other) {
  vector<Weapon *> seized = other->beenSeizedWeapon();
  sort(seized.begin(), seized.end(), [](Weapon *a, Weapon *b) {
    if (a->getType() != b->getType())
      return a->getType() < b->getType();
    if (a->getType() == ARROW)
      return static_cast<Arrow *>(a)->getDurability() >
             static_cast<Arrow *>(b)->getDurability();
    return false;
  });
  for (auto weapon : seized) {
    if (weapons.size() < 10) {
      weapons.push_back(weapon);
    } else {
      delete weapon;
    }
  }
}

void Warrior::report() {
  sortWeapons();
  int num[WEAPON_NUM] = {0};
  for (auto weapon : weapons) {
    num[weapon->getType()]++;
  }
  clocktime.printTime();
  printf("%s %s %d has %d sword %d bomb %d arrow and %d elements\n",
         this->getColor().c_str(), WARRIOR_NAMES[this->getType()].c_str(), id,
         num[0], num[1], num[2], hp);
}

Warrior::~Warrior() {
  for (auto weapon : weapons) {
    if (weapon) {
      delete weapon;
    }
  }
  weapons.clear();
}

void Wolf::getWeapon(Warrior *other, int city_id) {
  vector<Weapon *> stolen = other->stolenWeapon();
  if (stolen.empty()) {
    return;
  }
  sort(stolen.begin(), stolen.end(), [](Weapon *a, Weapon *b) {
    if (a->getType() != b->getType())
      return a->getType() < b->getType();
    if (a->getType() == ARROW)
      return static_cast<Arrow *>(a)->getDurability() >
             static_cast<Arrow *>(b)->getDurability();
    return false;
  });
  int num = 0;
  Weapon_Type type = stolen[0]->getType();
  if (type == ARROW) {
    for (int i = stolen.size() - 1; i >= 0; i--) {
      if (weapons.size() < 10) {
        weapons.push_back(stolen[i]);
        num++;
      } else {
        delete stolen[i];
      }
    }
  } else {
    for (auto weapon : stolen) {
      if (weapons.size() < 10) {
        weapons.push_back(weapon);
        num++;
      } else {
        delete weapon;
      }
    }
  }
  clocktime.printTime();
  printf("%s wolf %d took %d %s from %s %s %d in city %d\n",
         this->getColor().c_str(), id, num, WEAPON_NAMES[type].c_str(),
         other->getColor().c_str(), WARRIOR_NAMES[other->getType()].c_str(),
         other->get_id(), city_id);
  sortWeapons();
}
