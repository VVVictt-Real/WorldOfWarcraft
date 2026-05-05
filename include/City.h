#ifndef CITY
#define CITY

#include <utility>

class Warrior;
class Weapon;

class City {
private:
  int id;
  int lifeUnit = 0;
  int redLast = 0;
  int blueLast = 0;

public:
  int flag; // 1表示红，2表示蓝
  Warrior *redWarrior;
  Warrior *blueWarrior;
  City() {
    redWarrior = nullptr;
    blueWarrior = nullptr;
    flag = -1;
  }
  City(int id) : id(id) {
    redWarrior = nullptr;
    blueWarrior = nullptr;
    flag = -1;
  }
  bool checkRedFirst();
  bool hasBattle() { return redWarrior && blueWarrior; }
  void runBattle();
  void afterBattle(Warrior *attacker, Warrior *target, bool isFirst);
  std::pair<int, int> preRunBattle();
  int getID() { return id; }
  void checkLionEscape();
  void checkWolf();
  void checkWarrior();
  void generateLifeUnit() { lifeUnit += 10; }
  int getLifeUnit() { return lifeUnit; }
  void takenLifeUnit() { lifeUnit = 0; }
  void redShootNext(City *next);
  void blueShootNext(City *next);
};

#endif // !CITY
