// Combined Source Code for OpenJudge
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>


/* --- Start of Defs.h --- */
#ifndef DEFS_H
#define DEFS_H


using namespace std;

enum Warrior_Type {
  DRAGON = 0,
  NINJA = 1,
  ICEMAN = 2,
  LION = 3,
  WOLF = 4,
  WARRIOR_NUM = 5
};
enum Weapon_Type { SWORD = 0, BOMB = 1, ARROW = 2, WEAPON_NUM = 3 };
const string WARRIOR_NAMES[5] = {"dragon", "ninja", "iceman", "lion", "wolf"};
const string WEAPON_NAMES[3] = {"sword", "bomb", "arrow"};
const Warrior_Type RED_ORDER[5] = {ICEMAN, LION, WOLF, NINJA, DRAGON};
const Warrior_Type BLUE_ORDER[5] = {LION, DRAGON, NINJA, ICEMAN, WOLF};
inline int initial_hp[5] = {0}; // 考虑用全局数组来存储初始血量

#endif

/* --- Start of Warrior.h --- */
#ifndef WARRIOR_H
#define WARRIOR_H


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
  int get_warrior_num();
  void write_hp(int new_hp) { hp = new_hp; }
};

class Dragon : public Warrior {
private:
  Weapon_Type weapon;
  double morale;

public:
  Dragon(int id, Headquarter *owner);
  Weapon_Type getWeapon() { return weapon; }
  double getMorale() { return morale; }
};

class Ninja : public Warrior {
private:
  Weapon_Type weapon_1, weapon_2;

public:
  Ninja(int id, Headquarter *owner);
  Weapon_Type getWeapon1() { return weapon_1; }
  Weapon_Type getWeapon2() { return weapon_2; }
};

class Iceman : public Warrior {
private:
  Weapon_Type weapon;

public:
  Iceman(int id, Headquarter *owner);
  Weapon_Type getWeapon() { return weapon; }
};

class Lion : public Warrior {
private:
  int loyalty;

public:
  Lion(int id, Headquarter *owner);
  int getLoyalty() { return loyalty; }
};

class Wolf : public Warrior {
private:
public:
  Wolf(int id, Headquarter *owner)
      : Warrior(id, initial_hp[WOLF], owner, WOLF) {}
};

#endif // !WARRIOR_H

/* --- Start of Headquarter.h --- */
#ifndef HEADQUARTER_H
#define HEADQUARTER_H


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

/* --- Start of Game.h --- */
#ifndef GAME_H
#define GAME_H


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
void game();

#endif // !GAME_H

/* --- Start of Warrior.cpp --- */

Warrior::Warrior(int id, int hp, Headquarter *owner, Warrior_Type type)
    : id(id), hp(hp), owner(owner), myType(type) {
  owner->addWarriorNum(type);
}
int Warrior::get_warrior_num() { return owner->getWarriorNum(myType); }

Dragon::Dragon(int id, Headquarter *owner)
    : Warrior(id, initial_hp[DRAGON], owner, DRAGON) {
  weapon = static_cast<Weapon_Type>(id % WEAPON_NUM);
  morale = static_cast<double>(owner->getLifeUnit()) /
           static_cast<double>(initial_hp[DRAGON]);
}

Ninja::Ninja(int id, Headquarter *owner)
    : Warrior(id, initial_hp[NINJA], owner, NINJA) {
  weapon_1 = static_cast<Weapon_Type>(id % WEAPON_NUM);
  weapon_2 = static_cast<Weapon_Type>((id + 1) % WEAPON_NUM);
}

Iceman::Iceman(int id, Headquarter *owner)
    : Warrior(id, initial_hp[ICEMAN], owner, ICEMAN) {
  weapon = static_cast<Weapon_Type>(id % WEAPON_NUM);
}

Lion::Lion(int id, Headquarter *owner)
    : Warrior(id, initial_hp[LION], owner, LION) {
  loyalty = owner->getLifeUnit();
}

/* --- Start of Headquarter.cpp --- */

bool Headquarter::buildWarrior() {
  for (int i = 0; i < 5; i++) {
    Warrior_Type current_warrior = order[buildIndex];
    int consume_hp = initial_hp[current_warrior];
    if (lifeUnit >= consume_hp) {
      lifeUnit -= consume_hp;
      Warrior *newWarrior = nullptr;
      switch (current_warrior) {
      case DRAGON:
        newWarrior = new Dragon(warrior.size() + 1, this);
        break;
      case NINJA:
        newWarrior = new Ninja(warrior.size() + 1, this);
        break;
      case ICEMAN:
        newWarrior = new Iceman(warrior.size() + 1, this);
        break;
      case LION:
        newWarrior = new Lion(warrior.size() + 1, this);
        break;
      case WOLF:
        newWarrior = new Wolf(warrior.size() + 1, this);
        break;
      case WARRIOR_NUM:
        cout << "Error\n";
        break;
      }
      warrior.push_back(newWarrior);
      printf("%s %s %d born with strength %d,%d %s in %s headquarter\n",
             name.c_str(), WARRIOR_NAMES[current_warrior].c_str(),
             int(warrior.size()), initial_hp[current_warrior],
             newWarrior->get_warrior_num(),
             WARRIOR_NAMES[current_warrior].c_str(), name.c_str());
      switch (current_warrior) {
      case DRAGON:
        printf("It has a %s,and it's morale is %.2lf\n",
               WEAPON_NAMES[(static_cast<Dragon *>(newWarrior))->getWeapon()]
                   .c_str(),
               static_cast<Dragon *>(newWarrior)->getMorale());
        break;
      case NINJA:
        printf("It has a %s and a %s\n",
               WEAPON_NAMES[static_cast<Ninja *>(newWarrior)->getWeapon1()]
                   .c_str(),
               WEAPON_NAMES[static_cast<Ninja *>(newWarrior)->getWeapon2()]
                   .c_str());
        break;
      case ICEMAN:
        printf("It has a %s\n",
               WEAPON_NAMES[static_cast<Iceman *>(newWarrior)->getWeapon()]
                   .c_str());
        break;
      case LION:
        printf("It's loyalty is %d\n",
               static_cast<Lion *>(newWarrior)->getLoyalty());
        break;
      case WOLF:
        break;
      case WARRIOR_NUM:
        cout << "Error\n";
        break;
      }

      buildIndex = (buildIndex + 1) % WARRIOR_NUM;
      return true;
    }
    buildIndex = (buildIndex + 1) % WARRIOR_NUM;
  }
  isEmpty = true;
  return false;
}

/* --- Start of Game.cpp --- */

void game() {
  int M;
  cin >> M;
  for (int i = 0; i < 5; i++) {
    cin >> initial_hp[i];
  }
  Game new_game(M);
  new_game.run();
}

/* --- Start of main.cpp --- */

int K;

int main() {
  cin >> K;
  for (int i = 1; i <= K; i++) {
    printf("Case:%d\n", i);
    game();
  }
  return 0;
}
