#ifndef HEADQUARTER_H
#define HEADQUARTER_H

#include "Defs.h"
#include <vector>

class Warrior;
class City;

class Headquarter {
private:
  string name;
  int lifeUnit;
  int buildIndex; // 直接取0-5，总数暂时由warrior.size()提供
  const Warrior_Type *order;
  int warriorNum[5] = {0};
  bool isEmpty = false;
  City *base_city;
  int totalWarriorNum = 0;

public:
  bool takenTime = false;
  Headquarter(const string &name, int lifeUnit, const Warrior_Type *order)
      : name(name), lifeUnit(lifeUnit), buildIndex(0), order(order) {}
  bool buildWarrior();
  int getWarriorNum(Warrior_Type type) { return warriorNum[type]; }
  void addWarriorNum(Warrior_Type type) { warriorNum[type]++; }
  bool is_empty() { return isEmpty; }
  int getLifeUnit() { return lifeUnit; }
  void addLifeUnit(int delta) { lifeUnit += delta; }
  void setBase(City *city) { base_city = city; }
  string getName() { return name; }
  ~Headquarter();
};

#endif // !HEADQUARTER_H
