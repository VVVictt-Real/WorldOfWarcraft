#ifndef HEADQUARTER_H
#define HEADQUARTER_H

#include "Defs.h"
#include <vector>

class Warrior;

class Headquarter {
private:
  string name;
  int lifeUnit;
  int buildIndex; // 直接取0-5，总数暂时由warrior.size()提供
  const Warrior_Type *order;
  int warriorNum[5] = {0};
  vector<Warrior *> warrior;
  bool isEmpty = false;

public:
  Headquarter(const string &name, int lifeUnit, const Warrior_Type *order)
      : name(name), lifeUnit(lifeUnit), buildIndex(0), order(order) {}
  bool buildWarrior();
  int getWarriorNum(Warrior_Type type) { return warriorNum[type]; }
  void addWarriorNum(Warrior_Type type) { warriorNum[type]++; }
  bool is_empty() { return isEmpty; }
  int getTotalNum() { return warrior.size(); }
  int getLifeUnit() { return lifeUnit; }
};

#endif // !HEADQUARTER_H
