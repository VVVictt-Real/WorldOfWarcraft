#include "Game.h"
#include "Clock.h"
#include "Defs.h"
#include "Warrior.h"
#include <cstdio>
#include <iostream>

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
      // if (!red_headquarter.buildWarrior()) {
      //   printf("red headquarter stops making warriors\n");
      // }
    }
    if (!blue_headquarter.is_empty()) {
      blue_headquarter.buildWarrior();
      // if (!blue_headquarter.buildWarrior()) {
      //   printf("blue headquarter stops making warriors\n");
      // }
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
