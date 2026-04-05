// Combined Source Code for OpenJudge
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <string>
#include <utility>
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
inline int attackPower[5] = {0}; // 考虑使用全局数组来存储武士攻击力

#endif

/* --- Start of Clock.h --- */
#ifndef CLOCK
#define CLOCK


class Clock {
private:
  int minutes;

public:
  Clock() : minutes(0) {}
  Clock(int t) : minutes(t) {}
  bool operator<(const Clock &other) const { return minutes < other.minutes; }
  void printTime() {
    int hour = minutes / 60;
    int minute = minutes % 60;
    printf("%03d:%02d ", hour, minute);
  }
  void addTime(int t) { minutes += t; }
};

inline Clock clocktime;

#endif // !CLOCK

/* --- Start of Warrior.h --- */
#ifndef WARRIOR_H
#define WARRIOR_H


class Headquarter;
class Weapon;

class Warrior {
protected:
  int id;
  int hp, ap;
  Headquarter *owner;
  Warrior_Type myType;
  vector<Weapon *> weapons;
  int currentWeapon;

public:
  Warrior(int id, int hp, Headquarter *owner, Warrior_Type type);
  int get_id() { return id; }
  int get_hp() { return hp; }
  int get_ap() { return ap; }
  int get_warrior_num();
  void beHurt(int damage) { hp -= damage; }
  bool isDead() { return hp <= 0; }
  void attack(Warrior *other);
  void sortWeapons();
  bool hasWeapons();
  Warrior_Type getType() { return myType; }
  vector<Weapon *> stolenWeapon();
  vector<Weapon *> beenSeizedWeapon();
  void seizeWeapon(Warrior *other);
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
};

class Ninja : public Warrior {
private:
public:
  Ninja(int id, Headquarter *owner);
};

class Iceman : public Warrior {
private:
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
};

#endif // !WARRIOR_H

/* --- Start of Headquarter.h --- */
#ifndef HEADQUARTER_H
#define HEADQUARTER_H


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
  Headquarter(const string &name, int lifeUnit, const Warrior_Type *order)
      : name(name), lifeUnit(lifeUnit), buildIndex(0), order(order) {}
  bool buildWarrior();
  int getWarriorNum(Warrior_Type type) { return warriorNum[type]; }
  void addWarriorNum(Warrior_Type type) { warriorNum[type]++; }
  bool is_empty() { return isEmpty; }
  int getLifeUnit() { return lifeUnit; }
  void setBase(City *city) { base_city = city; }
  string getName() { return name; }
  ~Headquarter();
};

#endif // !HEADQUARTER_H

/* --- Start of Game.h --- */
#ifndef GAME_H
#define GAME_H


class Game {
private:
  Headquarter red_headquarter;
  Headquarter blue_headquarter;
  vector<City> cities;
  int cityNum;
  Clock endtime;

public:
  Game(int lifeUnit, int N, int T);
  void run();
  bool move();
  ~Game();
};
void game();

#endif // !GAME_H

/* --- Start of Weapon.h --- */
#ifndef WEAPON
#define WEAPON


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
  virtual void change_owner(Warrior *newOwner) { owner = newOwner; }
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
  void change_owner(Warrior *newOwner) override;
};

class Bomb : public Weapon {
private:
  bool isUsed = false;

public:
  Bomb(Warrior *owner, int ap) : Weapon(BOMB, owner, ap * 2 / 5){};
  bool isDamaged() override { return isUsed; }
  int attackSelf() override { return attackPower / 2; }
  void used() override { isUsed = true; }
  void change_owner(Warrior *newOwner) override;
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
  void change_owner(Warrior *newOwner) override;
};

#endif // !WEAPON

/* --- Start of City.h --- */
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

/* --- Start of Warrior.cpp --- */

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
  for (auto &weapon : weapons)
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
  sortWeapons();
  int num[WEAPON_NUM] = {0};
  for (auto &weapon : weapons) {
    num[weapon->getType()]++;
  }
  clocktime.printTime();
  printf("%s %s %d has %d sword %d bomb %d arrow and %d elements\n",
         this->getColor().c_str(), WARRIOR_NAMES[this->getType()].c_str(), id,
         num[0], num[1], num[2], hp);
}

Warrior::~Warrior() {
  for (auto &weapon : weapons) {
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
  for (auto &weapon : stolen) {
    if (weapons.size() < 10) {
      weapon->change_owner(this);
      weapons.push_back(weapon);
      num++;
    } else {
      delete weapon;
    }
  }
  clocktime.printTime();
  printf("%s wolf %d took %d %s from %s %s %d in city %d\n",
         this->getColor().c_str(), id, num, WEAPON_NAMES[type].c_str(),
         other->getColor().c_str(), WARRIOR_NAMES[other->getType()].c_str(),
         other->get_id(), city_id);
  sortWeapons();
}

/* --- Start of Headquarter.cpp --- */

bool Headquarter::buildWarrior() {
  Warrior_Type current_warrior = order[buildIndex];
  int consume_hp = initial_hp[current_warrior];
  if (lifeUnit >= consume_hp) {
    lifeUnit -= consume_hp;
    Warrior *newWarrior = nullptr;
    switch (current_warrior) {
    case DRAGON:
      newWarrior = new Dragon(totalWarriorNum + 1, this);
      break;
    case NINJA:
      newWarrior = new Ninja(totalWarriorNum + 1, this);
      break;
    case ICEMAN:
      newWarrior = new Iceman(totalWarriorNum + 1, this);
      break;
    case LION:
      newWarrior = new Lion(totalWarriorNum + 1, this);
      break;
    case WOLF:
      newWarrior = new Wolf(totalWarriorNum + 1, this);
      break;
    case WARRIOR_NUM:
      cout << "Error\n";
      break;
    }
    totalWarriorNum++;
    clocktime.printTime();
    printf("%s %s %d born\n", name.c_str(),
           WARRIOR_NAMES[current_warrior].c_str(), totalWarriorNum);
    if (current_warrior == LION) {
      printf("Its loyalty is %d\n",
             static_cast<Lion *>(newWarrior)->getLoyalty());
    }
    if (name == "red") {
      base_city->redWarrior = newWarrior;
    }
    if (name == "blue") {
      base_city->blueWarrior = newWarrior;
    }
    buildIndex = (buildIndex + 1) % WARRIOR_NUM;
    return true;
  }
  isEmpty = true;
  return false;
}
Headquarter::~Headquarter() {}

/* --- Start of Game.cpp --- */

void game() {
  int M, N, K, T;
  cin >> M >> N >> K >> T;
  Lion::writeLoseEachStep(K);
  clocktime = Clock(0);
  for (int i = 0; i < 5; i++) {
    cin >> initial_hp[i];
  }
  for (int i = 0; i < 5; i++) {
    cin >> attackPower[i];
  }
  Game new_game(M, N, T);
  new_game.run();
}

Game::Game(int lifeUnit, int N, int T)
    : red_headquarter("red", lifeUnit, RED_ORDER),
      blue_headquarter("blue", lifeUnit, BLUE_ORDER), cityNum(N), endtime(T) {
  for (int i = 0; i <= N + 1; i++) {
    cities.push_back(City(i));
  }
  red_headquarter.setBase(&cities[0]);
  blue_headquarter.setBase(&cities[N + 1]);
}

void Game::run() {
  while (true) {
    if (!red_headquarter.is_empty()) {
      red_headquarter.buildWarrior();
    }
    if (!blue_headquarter.is_empty()) {
      blue_headquarter.buildWarrior();
    }

    clocktime.addTime(5);
    if (endtime < clocktime)
      break;
    for (auto &city : cities) {
      city.checkLionEscape();
    }

    clocktime.addTime(5);
    if (endtime < clocktime)
      break;
    if (move())
      break;

    clocktime.addTime(25);
    if (endtime < clocktime)
      break;
    for (auto &city : cities) {
      if (city.hasBattle())
        city.checkWolf();
    }

    clocktime.addTime(5);
    if (endtime < clocktime)
      break;
    for (auto &city : cities) {
      if (city.hasBattle()) {
        city.runBattle();
      }
    }

    clocktime.addTime(10);
    if (endtime < clocktime)
      break;
    clocktime.printTime();
    printf("%d elements in red headquarter\n", red_headquarter.getLifeUnit());
    clocktime.printTime();
    printf("%d elements in blue headquarter\n", blue_headquarter.getLifeUnit());

    clocktime.addTime(5);
    if (endtime < clocktime)
      break;
    for (auto &city : cities) {
      city.checkWarrior();
    }
    clocktime.addTime(5);
    if (endtime < clocktime)
      break;
  }
}

bool Game::move() {
  bool flag = false;
  for (int i = cityNum; i >= 0; i--) {
    if (cities[i].redWarrior == nullptr)
      continue;
    if (cities[i].redWarrior->getType() == ICEMAN) {
      static_cast<Iceman *>(cities[i].redWarrior)->moveStep();
    }
    if (cities[i].redWarrior->getType() == LION) {
      static_cast<Lion *>(cities[i].redWarrior)->loseLoyalty();
    }
    if (cities[i].redWarrior == nullptr)
      continue;
    cities[i + 1].redWarrior = cities[i].redWarrior;
    cities[i].redWarrior = nullptr;
  }
  for (int i = 0; i <= cityNum; i++) {
    if (cities[i + 1].blueWarrior == nullptr)
      continue;
    if (cities[i + 1].blueWarrior->getType() == ICEMAN) {
      static_cast<Iceman *>(cities[i + 1].blueWarrior)->moveStep();
    }
    if (cities[i + 1].blueWarrior->getType() == LION) {
      static_cast<Lion *>(cities[i + 1].blueWarrior)->loseLoyalty();
    }
    if (cities[i + 1].blueWarrior == nullptr)
      continue;
    cities[i].blueWarrior = cities[i + 1].blueWarrior;
    cities[i + 1].blueWarrior = nullptr;
  }
  if (cities[0].blueWarrior != nullptr) {
    Warrior *tmp = cities[0].blueWarrior;
    clocktime.printTime();
    printf("blue %s %d reached red headquarter with %d elements and force %d\n",
           WARRIOR_NAMES[tmp->getType()].c_str(), tmp->get_id(), tmp->get_hp(),
           tmp->get_ap());
    clocktime.printTime();
    printf("red headquarter was taken\n");
    flag = true;
  }
  for (int i = 1; i <= cityNum; i++) {
    if (cities[i].redWarrior != nullptr) {
      Warrior *tmp = cities[i].redWarrior;
      clocktime.printTime();
      printf("red %s %d marched to city %d with %d elements and force %d\n",
             WARRIOR_NAMES[tmp->getType()].c_str(), tmp->get_id(), i,
             tmp->get_hp(), tmp->get_ap());
    }
    if (cities[i].blueWarrior != nullptr) {
      Warrior *tmp = cities[i].blueWarrior;
      clocktime.printTime();
      printf("blue %s %d marched to city %d with %d elements and force %d\n",
             WARRIOR_NAMES[tmp->getType()].c_str(), tmp->get_id(), i,
             tmp->get_hp(), tmp->get_ap());
    }
  }
  if (cities[cityNum + 1].redWarrior != nullptr) {
    Warrior *tmp = cities[cityNum + 1].redWarrior;
    clocktime.printTime();
    printf("red %s %d reached blue headquarter with %d elements and force %d\n",
           WARRIOR_NAMES[tmp->getType()].c_str(), tmp->get_id(), tmp->get_hp(),
           tmp->get_ap());
    clocktime.printTime();
    printf("blue headquarter was taken\n");
    flag = true;
  }
  return flag;
}
Game::~Game() {
  for (auto &city : cities) {
    if (city.redWarrior) {
      delete city.redWarrior;
      city.redWarrior = nullptr;
    }
    if (city.blueWarrior) {
      delete city.blueWarrior;
      city.blueWarrior = nullptr;
    }
  }
}

/* --- Start of Weapon.cpp --- */

void Sword::change_owner(Warrior *newOwner) {
  attackPower = newOwner->get_ap() / 5;
}

void Bomb::change_owner(Warrior *newOwner) {
  attackPower = newOwner->get_ap() * 2 / 5;
}

void Arrow::change_owner(Warrior *newOwner) {
  attackPower = newOwner->get_ap() * 3 / 10;
}

/* --- Start of City.cpp --- */

void City::runBattle() {
  Warrior *attacker = (id % 2 == 1) ? redWarrior : blueWarrior;
  Warrior *target = (id % 2 == 1) ? blueWarrior : redWarrior;
  attacker->sortWeapons();
  target->sortWeapons();
  while (true) {
    attacker->attack(target);
    if (attacker->isDead() || target->isDead())
      break;
    if (!attacker->hasWeapons() && !target->hasWeapons())
      break;
    swap(attacker, target);
  }
  if (redWarrior->isDead() && blueWarrior->isDead()) {
    clocktime.printTime();
    printf("both red %s %d and blue %s %d died in city %d\n",
           WARRIOR_NAMES[redWarrior->getType()].c_str(), redWarrior->get_id(),
           WARRIOR_NAMES[blueWarrior->getType()].c_str(), blueWarrior->get_id(),
           id);
    delete redWarrior;
    delete blueWarrior;
    redWarrior = nullptr;
    blueWarrior = nullptr;
    return;
  }
  if (redWarrior->isDead()) {
    blueWarrior->seizeWeapon(redWarrior);
    clocktime.printTime();
    printf("blue %s %d killed red %s %d in city %d remaining %d elements\n",
           WARRIOR_NAMES[blueWarrior->getType()].c_str(), blueWarrior->get_id(),
           WARRIOR_NAMES[redWarrior->getType()].c_str(), redWarrior->get_id(),
           id, blueWarrior->get_hp());
    if (blueWarrior->getType() == DRAGON) {
      clocktime.printTime();
      printf("blue dragon %d yelled in city %d\n", blueWarrior->get_id(), id);
    }
    delete redWarrior;
    redWarrior = nullptr;
    return;
  }
  if (blueWarrior->isDead()) {
    redWarrior->seizeWeapon(blueWarrior);
    clocktime.printTime();
    printf("red %s %d killed blue %s %d in city %d remaining %d elements\n",
           WARRIOR_NAMES[redWarrior->getType()].c_str(), redWarrior->get_id(),
           WARRIOR_NAMES[blueWarrior->getType()].c_str(), blueWarrior->get_id(),
           id, redWarrior->get_hp());
    if (redWarrior->getType() == DRAGON) {
      clocktime.printTime();
      printf("red dragon %d yelled in city %d\n", redWarrior->get_id(), id);
    }
    delete blueWarrior;
    blueWarrior = nullptr;
    return;
  }
  clocktime.printTime();
  printf("both red %s %d and blue %s %d were alive in city %d\n",
         WARRIOR_NAMES[redWarrior->getType()].c_str(), redWarrior->get_id(),
         WARRIOR_NAMES[blueWarrior->getType()].c_str(), blueWarrior->get_id(),
         id);
  if (redWarrior->getType() == DRAGON) {
    clocktime.printTime();
    printf("red dragon %d yelled in city %d\n", redWarrior->get_id(), id);
  }
  if (blueWarrior->getType() == DRAGON) {
    clocktime.printTime();
    printf("blue dragon %d yelled in city %d\n", blueWarrior->get_id(), id);
  }
}

void City::checkLionEscape() {
  if (redWarrior != nullptr && redWarrior->getType() == LION) {
    if (static_cast<Lion *>(redWarrior)->getLoyalty() <= 0) {
      clocktime.printTime();
      printf("red lion %d ran away\n", redWarrior->get_id());
      delete redWarrior;
      redWarrior = nullptr;
    }
  }
  if (blueWarrior != nullptr && blueWarrior->getType() == LION) {
    if (static_cast<Lion *>(blueWarrior)->getLoyalty() <= 0) {
      clocktime.printTime();
      printf("blue lion %d ran away\n", blueWarrior->get_id());
      delete blueWarrior;
      blueWarrior = nullptr;
    }
  }
}

void City::checkWolf() {
  if (redWarrior->getType() == WOLF && blueWarrior->getType() == WOLF) {
    return;
  }
  if (redWarrior->getType() == WOLF) {
    static_cast<Wolf *>(redWarrior)->getWeapon(blueWarrior, id);
  }
  if (blueWarrior->getType() == WOLF) {
    static_cast<Wolf *>(blueWarrior)->getWeapon(redWarrior, id);
  }
}
void City::checkWarrior() {
  if (redWarrior)
    redWarrior->report();
  if (blueWarrior)
    blueWarrior->report();
}

/* --- Start of main.cpp --- */

int K;

int main() {
  cin >> K;
  for (int i = 1; i <= K; i++) {
    printf("Case %d:\n", i);
    game();
  }
  return 0;
}
