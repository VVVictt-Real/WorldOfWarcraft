#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

enum Warrior_Type {
  DRAGON = 0,
  NINJA = 1,
  ICEMAN = 2,
  LION = 3,
  WOLF = 4,
  WARRIOR_NUM = 5
};
const string WARRIOR_NAMES[5] = {"dragon", "ninja", "iceman", "lion", "wolf"};
const Warrior_Type RED_ORDER[5] = {ICEMAN, LION, WOLF, NINJA, DRAGON};
const Warrior_Type BLUE_ORDER[5] = {LION, DRAGON, NINJA, ICEMAN, WOLF};
int initial_hp[5] = {0}; // 考虑用全局数组来存储初始血量

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
  int get_total_num();
  void write_hp(int new_hp) { hp = new_hp; }
};

class Dragon : public Warrior {
private:
public:
  Dragon(int id, Headquarter *owner)
      : Warrior(id, initial_hp[DRAGON], owner, DRAGON) {}
};

class Ninja : public Warrior {
private:
public:
  Ninja(int id, Headquarter *owner)
      : Warrior(id, initial_hp[NINJA], owner, NINJA) {}
};

class Iceman : public Warrior {
private:
public:
  Iceman(int id, Headquarter *owner)
      : Warrior(id, initial_hp[ICEMAN], owner, ICEMAN) {}
};

class Lion : public Warrior {
private:
public:
  Lion(int id, Headquarter *owner)
      : Warrior(id, initial_hp[LION], owner, LION) {}
};

class Wolf : public Warrior {
private:
public:
  Wolf(int id, Headquarter *owner)
      : Warrior(id, initial_hp[LION], owner, WOLF) {}
};

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
  bool buildWarrior() {
    for (int i = 0; i < 5; i++) {
      Warrior_Type current_warrior = order[buildIndex];
      int consume_hp = initial_hp[current_warrior];
      if (lifeUnit >= consume_hp) {
        lifeUnit -= consume_hp;
        Warrior *newWarrior = nullptr;
        switch (current_warrior) {
        case DRAGON:
          newWarrior = new Dragon(warrior.size(), this);
          break;
        case NINJA:
          newWarrior = new Ninja(warrior.size(), this);
          break;
        case ICEMAN:
          newWarrior = new Iceman(warrior.size(), this);
          break;
        case LION:
          newWarrior = new Lion(warrior.size(), this);
          break;
        case WOLF:
          newWarrior = new Wolf(warrior.size(), this);
          break;
        case WARRIOR_NUM:
          cout << "Error\n";
          break;
        }
        warrior.push_back(newWarrior);
        printf("%s %s %d born with strength %d,%d %s in %s headquarter\n",
               name.c_str(), WARRIOR_NAMES[current_warrior].c_str(),
               int(warrior.size()), initial_hp[current_warrior],
               newWarrior->get_total_num(),
               WARRIOR_NAMES[current_warrior].c_str(), name.c_str());
        buildIndex = (buildIndex + 1) % WARRIOR_NUM;
        return true;
      }
      buildIndex = (buildIndex + 1) % WARRIOR_NUM;
    }
    isEmpty = true;
    return false;
  }
  int getWarriorNum(Warrior_Type type) { return warriorNum[type]; }
  void addWarriorNum(Warrior_Type type) { warriorNum[type]++; }
  bool is_empty() { return isEmpty; }
};

Warrior::Warrior(int id, int hp, Headquarter *owner, Warrior_Type type)
    : id(id), hp(hp), owner(owner), myType(type) {
  owner->addWarriorNum(type);
}
int Warrior::get_total_num() { return owner->getWarriorNum(myType); }

class Game {
private:
  int clocktime;
  Headquarter red_headquarter;
  Headquarter blue_headquarter;

public:
  Game(int lifeUnit)
      : red_headquarter("red", lifeUnit, RED_ORDER),
        blue_headquarter("blue", lifeUnit, BLUE_ORDER) {
    clocktime = 0;
  }
  void run() {
    while (!(red_headquarter.is_empty() && blue_headquarter.is_empty())) {
      if (!red_headquarter.is_empty()) {
        printf("%03d ", clocktime);
        if (!red_headquarter.buildWarrior()) {
          printf("red headquarter stops making warriors\n");
        }
      }
      if (!blue_headquarter.is_empty()) {
        printf("%03d ", clocktime);
        if (!blue_headquarter.buildWarrior()) {
          printf("blue headquarter stops making warriors\n");
        }
      }
      clocktime++;
    }
  }
};

void game() {
  int M;
  cin >> M;
  for (int i = 0; i < 5; i++) {
    cin >> initial_hp[i];
  }
  Game new_game(M);
  new_game.run();
}

int K;

int main() {
  cin >> K;
  for (int i = 1; i <= K; i++) {
    printf("Case:%d\n", i);
    game();
  }
  return 0;
}
