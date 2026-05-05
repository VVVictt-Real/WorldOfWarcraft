#include "Warrior.h"
#include "Clock.h"
#include "Defs.h"
#include "Headquarter.h"
#include "Weapon.h"
#include <cstdio>
#include <vector>

Warrior::Warrior(int id, int hp, Headquarter *owner, Warrior_Type type)
    : id(id), hp(hp), owner(owner), myType(type), weapons(WEAPON_NUM, nullptr) {
  owner->addWarriorNum(type);
}
int Warrior::get_warrior_num() { return owner->getWarriorNum(myType); }
void Warrior::add_hp(int delta) {
  hp += delta;
  owner->addLifeUnit(-delta);
}

Dragon::Dragon(int id, Headquarter *owner)
    : Warrior(id, initial_hp[DRAGON], owner, DRAGON) {
  ap = attackPower[DRAGON];
  Weapon *initialWeapon = nullptr;
  switch (id % WEAPON_NUM) {
  case SWORD:
    initialWeapon = new Sword(this, ap);
    break;
  case BOMB:
    initialWeapon = new Bomb(this);
    break;
  case ARROW:
    initialWeapon = new Arrow(this);
    break;
  }
  weapons[id % WEAPON_NUM] = initialWeapon;
  hasWeapons[id % WEAPON_NUM] = true;
  morale = static_cast<double>(owner->getLifeUnit()) /
           static_cast<double>(initial_hp[DRAGON]);
}

// void Dragon::cheer() {
//     if(morale > 0.8) {
//         clocktime.printTime();
//         ;
//     }
// }

Ninja::Ninja(int id, Headquarter *owner)
    : Warrior(id, initial_hp[NINJA], owner, NINJA) {
  ap = attackPower[NINJA];
  switch (id % WEAPON_NUM) {
  case SWORD:
    weapons[SWORD] = new Sword(this, ap);
    weapons[BOMB] = new Bomb(this);
    break;
  case BOMB:
    weapons[BOMB] = new Bomb(this);
    weapons[ARROW] = new Arrow(this);
    break;
  case ARROW:
    weapons[ARROW] = new Arrow(this);
    weapons[SWORD] = new Sword(this, ap);
    break;
  }
  hasWeapons[id % WEAPON_NUM] = true;
  hasWeapons[id % WEAPON_NUM + 1] = true;
}

Iceman::Iceman(int id, Headquarter *owner)
    : Warrior(id, initial_hp[ICEMAN], owner, ICEMAN) {
  ap = attackPower[ICEMAN];
  switch (id % WEAPON_NUM) {
  case SWORD:
    weapons[SWORD] = new Sword(this, ap);
    break;
  case BOMB:
    weapons[BOMB] = new Bomb(this);
    break;
  case ARROW:
    weapons[ARROW] = new Arrow(this);
    break;
  }
  hasWeapons[id % WEAPON_NUM] = true;
}

void Iceman::moveStep() {
  stepTime = !stepTime;
  if (stepTime) {
    if (hp > 9)
      hp -= hp / 9;
    else
      hp = 1;
    ap += 20;
  }
}

Lion::Lion(int id, Headquarter *owner)
    : Warrior(id, initial_hp[LION], owner, LION) {
  ap = attackPower[LION];
  loyalty = owner->getLifeUnit();
}

// void Warrior::sortWeapons() {
//   std::sort(weapons.begin(), weapons.end(),
//             [](Weapon *a, Weapon *b) { return (*a) < (*b); });
//   currentWeapon = 0;
// }

string Warrior::getColor() { return owner->getName(); }

void Warrior::attack(Warrior *other) {
  int attackPower = ap;
  if (hasWeapons[SWORD]) {
    attackPower += weapons[SWORD]->attackOther();
    weapons[SWORD]->used();
    if (weapons[SWORD]->isDamaged()) {
      delete weapons[SWORD];
      weapons[SWORD] = nullptr;
      hasWeapons[SWORD] = false;
    }
  }
  other->beHurt(attackPower);
}

void Warrior::counterAttack(Warrior *other) {
  int attackPower = ap / 2;
  if (hasWeapons[SWORD]) {
    attackPower += weapons[SWORD]->attackOther();
    weapons[SWORD]->used();
    if (weapons[SWORD]->isDamaged()) {
      delete weapons[SWORD];
      weapons[SWORD] = nullptr;
      hasWeapons[SWORD] = false;
    }
  }
  other->beHurt(attackPower);
}

bool Warrior::shoot(Warrior *other) {
  int flag = false;
  if (hasWeapons[ARROW]) {
    other->beHurt(weapons[ARROW]->attackOther());
    weapons[ARROW]->used();
    if (weapons[ARROW]->isDamaged()) {
      delete weapons[ARROW];
      weapons[ARROW] = nullptr;
      hasWeapons[ARROW] = false;
    }
    if (other->get_hp() <= 0) {
      flag = true;
      printf("%s %s %d shot and killed %s %s %d\n", getColor().c_str(),
             WARRIOR_NAMES[getType()].c_str(), id, other->getColor().c_str(),
             WARRIOR_NAMES[other->getType()].c_str(), other->get_id());
      delete other;
    } else {
      printf("%s %s %d shot\n", getColor().c_str(),
             WARRIOR_NAMES[getType()].c_str(), id);
    }
  }
  return flag;
}

// bool Warrior::hasWeapons() {
//   if (weapons.empty())
//     return false;
//   for (auto &weapon : weapons)
//     if (weapon->getType() == BOMB || weapon->getType() == ARROW ||
//         weapon->attackOther() || weapon->attackSelf())
//       return true;
//   return false;
// }

// vector<Weapon *> Warrior::stolenWeapon() {
//   vector<Weapon *> stolen;
//   if (weapons.empty()) {
//     return stolen;
//   }
//   sortWeapons();
//   Weapon_Type type = weapons[0]->getType();
//   for (auto it = weapons.begin(); it != weapons.end();) {
//     if ((*it)->getType() == type) {
//       stolen.push_back(*it);
//       it = weapons.erase(it);
//     } else {
//       ++it;
//     }
//   }
//   return stolen;
// }

vector<Weapon *> Warrior::beenSeizedWeapon() {
  vector<Weapon *> tmp = weapons;
  weapons.clear();
  return tmp;
}
void Wolf::seizeWeapon(Warrior *other) {
  vector<Weapon *> seized = other->beenSeizedWeapon();
  // sort(seized.begin(), seized.end(), [](Weapon *a, Weapon *b) {
  //   if (a->getType() != b->getType())
  //     return a->getType() < b->getType();
  //   if (a->getType() == ARROW)
  //     return static_cast<Arrow *>(a)->getDurability() >
  //            static_cast<Arrow *>(b)->getDurability();
  //   return false;
  // });
  for (int i = 0; i < WEAPON_NUM; ++i) {
    if (!hasWeapons[i]) {
      weapons[i] = seized[i];
    } else {
      delete seized[i];
    }
  }
  for (auto &weapon : seized) {
    if (weapons.size() < 10) {
      weapon->change_owner(this);
      weapons.push_back(weapon);
    } else {
      delete weapon;
    }
  }
}

void Warrior::report() {
  // sortWeapons();
  // int num[WEAPON_NUM] = {0};
  // for (auto &weapon : weapons) {
  //   num[weapon->getType()]++;
  // }
  clocktime.printTime();
  printf("%s %s %d has ", this->getColor().c_str(),
         WARRIOR_NAMES[this->getType()].c_str(), id);
  bool flag = 0;
  for (int i = WEAPON_NUM - 1; i >= 0; --i) {
    if (hasWeapons[i]) {
      if (flag) {
        printf(",");
        weapons[i]->printWeapon();
      } else
        weapons[i]->printWeapon();
      flag = 1;
    }
  }
  if (!flag)
    printf("no weapon\n");
  else
    printf("\n");
}

Warrior::~Warrior() {
  for (auto &weapon : weapons) {
    if (weapon) {
      delete weapon;
    }
  }
  weapons.clear();
}

// void Wolf::getWeapon(Warrior *other, int city_id) {
//   vector<Weapon *> stolen = other->stolenWeapon();
//   if (stolen.empty()) {
//     return;
//   }
//   sort(stolen.begin(), stolen.end(), [](Weapon *a, Weapon *b) {
//     if (a->getType() != b->getType())
//       return a->getType() < b->getType();
//     if (a->getType() == ARROW)
//       return static_cast<Arrow *>(a)->getDurability() >
//              static_cast<Arrow *>(b)->getDurability();
//     return false;
//   });
//   int num = 0;
//   Weapon_Type type = stolen[0]->getType();
//   for (auto &weapon : stolen) {
//     if (weapons.size() < 10) {
//       weapon->change_owner(this);
//       weapons.push_back(weapon);
//       num++;
//     } else {
//       delete weapon;
//     }
//   }
//   clocktime.printTime();
//   printf("%s wolf %d took %d %s from %s %s %d in city %d\n",
//          this->getColor().c_str(), id, num, WEAPON_NAMES[type].c_str(),
//          other->getColor().c_str(), WARRIOR_NAMES[other->getType()].c_str(),
//          other->get_id(), city_id);
//   sortWeapons();
// }
