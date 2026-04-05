#ifndef CITY
#define CITY

class Warrior;
class Weapon;

class City {
private:
  int id;

public:
  int flag;
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
  bool hasBattle() { return redWarrior && blueWarrior; }
  void runBattle();
  int getID() { return id; }
  void checkLionEscape();
  void checkWolf();
  void checkWarrior();
};

#endif // !CITY
